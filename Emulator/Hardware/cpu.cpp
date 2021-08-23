// Main file header
#include "cpu.hpp"

// C headers
#include <cstdint>

// Cpp headers
#include <bitset>
#include <iostream>
#include <iomanip>
#include <string>

#include "instructions.hpp"

using namespace std;

namespace Hardware {

CPU::CPU(Memory& cpuMemory, bool enableLog) :
    registerA(0),
    registerX(0),
    registerY(0),
    programCounter(0),
    stackPointer(0xFD),
    status(0x34),
    clockCounter(0),
    memory(cpuMemory),
    enableLog(enableLog)
{
    Instructions::initInstructionVector();
    triggerReset();
}

bool CPU::runInstruction() {
    if (clockCounter-- > 0) {
        return true;
    }

    auto instructionOpcode = memory.getByteAbsolute(programCounter);

    if (instructionOpcode == 0) {
        return false;
    }

    auto& instruction = Instructions::instructionVector[instructionOpcode];
    const auto instructionResult = instruction.run(*this);
    log(instructionResult);

    const auto& [ignored1, cycles, ignored2] = instructionResult;
    clockCounter = cycles - 1;

    return true;
}

void CPU::setZ(uint8_t value) {
    status.set(static_cast<int>(StatusBit::Z), !(value));
}

void CPU::setN(uint8_t value) {
    status.set(static_cast<int>(StatusBit::N), value & 0x80);
}

void CPU::setStatusBit(StatusBit bitName, bool bitValue) {
    status.set(static_cast<int>(bitName), bitValue);
}

bool CPU::getStatusBit(StatusBit bitName) {
    return status.test(static_cast<int>(bitName));
}

void CPU::pushToStack(uint8_t value) {
    // Get the stack item at the current stack position
    const auto stackAddress = 0x0100 | static_cast<uint16_t>(stackPointer);
    auto stackItem = memory.getByteAbsolute(stackAddress);

    // Set the stack item to value
    stackItem = value;

    // Advance the stack (Empty stack: set then increment)
    stackPointer--;
}

uint8_t CPU::pullFromStack() {
    // Retreat the stack (Empty stack: decrement then get)
    stackPointer++;

    // Get the stack value
    const auto stackAddress = 0x0100 | static_cast<uint16_t>(stackPointer);
    return memory.getByteAbsolute(stackAddress);
}

void CPU::triggerReset() {
    // Set PC to RESET address
    constexpr auto resetAddressPtr = 0xFFFC;
    programCounter = memory.readWordAbsolute(resetAddressPtr);
}

void CPU::log(InstructionRunnerReturn instructionResult) {
    if (enableLog) {
        const auto& [newProgramCounter, cycles, address] = instructionResult;
        if (address >= 0 && address <= 0x1FFF) {
            const auto address16 = static_cast<uint16_t>(address);
            cout << setfill('0')
                << "| pc = 0x" << hex << setw(4) << programCounter
                << " | a = 0x" << hex << setw(2) << (unsigned) registerA
                << " | x = 0x" << hex << setw(2) << (unsigned) registerX
                << " | y = 0x" << hex << setw(2) << (unsigned) registerY
                << " | sp = 0x01" << hex << setw(2) << (unsigned) stackPointer
                << " | p[NV-BDIZC] = " << bitset<8>(status)
                << " | MEM[0x" << hex << setw(4) << Memory::getRealMemoryAddress(address16)
                << "] = 0x" << hex << setw(2) << (unsigned) memory.getByteAbsolute(address16) << " |" << endl;
        }
        else {
            cout << setfill('0')
                << "| pc = 0x" << hex << setw(4) << programCounter
                << " | a = 0x" << hex << setw(2) << (unsigned) registerA
                << " | x = 0x" << hex << setw(2) << (unsigned) registerX
                << " | y = 0x" << hex << setw(2) << (unsigned) registerY
                << " | sp = 0x01" << hex << setw(2) << (unsigned) stackPointer
                << " | p[NV-BDIZC] = " << bitset<8>(status) << " |" << endl;
        }
    }
}

bool CPU::isSamePage(uint16_t v1, uint16_t v2) {
    return (v1 >> 8) == (v2 >> 8);
}

void CPU::interruption() {
    const auto& address_high = static_cast<uint8_t>(programCounter >> 8);
    pushToStack(address_high);

    const auto& address_low = static_cast<uint8_t>(programCounter & 0x00FF);
    pushToStack(address_low);

    constexpr uint8_t statusMask = 0xCF;
    constexpr uint8_t nmiTag = 0x30;
    pushToStack(static_cast<uint8_t>(status.to_ulong() & statusMask | nmiTag));

    constexpr auto nmiAddressPtr = 0xFFFA;
    programCounter = memory.readWordAbsolute(nmiAddressPtr);
}

}
