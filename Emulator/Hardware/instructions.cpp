// Main file header
#include "instructions.hpp"

// C headers
#include <cassert>
#include <cstdint>

// Cpp headers
#include <array>
#include <iostream>

// Project headers
#include "Hardware/cpu.hpp"

using namespace std;

namespace Hardware {

Instruction::Instruction() :
    runnerPtr(),
    opcode(0xFF),
    instructionSize(),
    numberOfCycles(),
    isImplemented(false)
{

}

Instruction::Instruction(uint8_t opcode) :
    runnerPtr(),
    opcode(opcode),
    instructionSize(),
    numberOfCycles(),
    isImplemented(false)
{

}

Instruction::Instruction(uint8_t opcode,
                         uint16_t instructionSize,
                         int numberOfCycles,
                         InstructionRunnerPtr runnerPtr) :
    runnerPtr(runnerPtr),
    opcode(opcode),
    instructionSize(instructionSize),
    numberOfCycles(numberOfCycles),
    isImplemented(true)
{

}

InstructionRunnerReturn Instruction::run(CPU& cpu) {
    if (!isImplemented) {
        cerr << "Instruction 0x"
             << uppercase << std::hex << static_cast<int>(opcode)
             << " not implemented" << endl;
        exit(-1);
    }

    // Run the instruction
    const auto instructionResult = runnerPtr(cpu, *this);
    const auto& [newProgramCounter, cycles, ignored] = instructionResult;

    // Advance the program counter
    cpu.programCounter = newProgramCounter;

    return instructionResult;
}

namespace Instructions {

#include "Instructions/allInstructions.cpp"

Instruction instructionVector[256];

void initInstructionVector() {
    for (int i = 0; i < 256; ++i) {
        instructionVector[i] = Instruction(i);
    }
    instructionVector[0x29] = Instruction(0x29, 2, 2, andImmediate);
    instructionVector[0x25] = Instruction(0x25, 2, 3, andZeroPage);
    instructionVector[0x35] = Instruction(0x35, 2, 4, andZeroPageX);
    instructionVector[0x2D] = Instruction(0x2D, 3, 4, andAbsolute);
    instructionVector[0x3D] = Instruction(0x3D, 3, 4, andAbsoluteX);
    instructionVector[0x39] = Instruction(0x39, 3, 4, andAbsoluteY);
    instructionVector[0x21] = Instruction(0x21, 2, 6, andIndirectX);
    instructionVector[0x31] = Instruction(0x31, 2, 5, andIndirectY);
    instructionVector[0xD0] = Instruction(0xD0, 2, 2, bne);
    instructionVector[0xB0] = Instruction(0xB0, 2, 2, bcs);
    instructionVector[0x50] = Instruction(0x50, 2, 2, bvc);
    instructionVector[0xA0] = Instruction(0xA0, 2, 2, ldyImmediate);
    instructionVector[0xA4] = Instruction(0xA4, 2, 3, ldyZeroPage);
    instructionVector[0xB4] = Instruction(0xB4, 2, 4, ldyZeroPageX);
    instructionVector[0xAC] = Instruction(0xAC, 3, 4, ldyAbsolute);
    instructionVector[0xBC] = Instruction(0xBC, 3, 4, ldyAbsoluteX);
    instructionVector[0xA8] = Instruction(0xA8, 1, 2, tay);
    instructionVector[0xF8] = Instruction(0xF8, 1, 2, sed);
    instructionVector[0x4C] = Instruction(0x4C, 3, 3, jmpAbsolute);
    instructionVector[0x6C] = Instruction(0x6C, 3, 5, jmpIndirect);
    instructionVector[0xAA] = Instruction(0xAA, 1, 2, tax);
    instructionVector[0x38] = Instruction(0x38, 1, 2, sec);
    instructionVector[0xC8] = Instruction(0xC8, 1, 2, iny);
    instructionVector[0x88] = Instruction(0x88, 1, 2, dey);
    instructionVector[0xCA] = Instruction(0xCA, 1, 2, dex);
    instructionVector[0xE8] = Instruction(0xE8, 1, 2, inx);
    instructionVector[0x90] = Instruction(0x90, 2, 2, bcc);
    instructionVector[0xE6] = Instruction(0xE6, 2, 5, incZeroPage);
    instructionVector[0xF6] = Instruction(0xF6, 2, 6, incZeroPageX);
    instructionVector[0xEE] = Instruction(0xEE, 3, 3, incAbsolute);
    instructionVector[0xFE] = Instruction(0xFE, 3, 7, incAbsoluteX);
    instructionVector[0x68] = Instruction(0x68, 1, 4, pla);
    instructionVector[0x28] = Instruction(0x28, 1, 4, plp);
    instructionVector[0x24] = Instruction(0x24, 2, 3, bitZeroPage);
    instructionVector[0x2C] = Instruction(0x2C, 3, 4, bitAbsolute);
    instructionVector[0x69] = Instruction(0x69, 2, 2, adcImmediate);
    instructionVector[0x65] = Instruction(0x65, 2, 3, adcZeroPage);
    instructionVector[0x75] = Instruction(0x75, 2, 4, adcZeroPageX);
    instructionVector[0x6D] = Instruction(0x6D, 3, 4, adcAbsolute);
    instructionVector[0x7D] = Instruction(0x7D, 3, 4, adcAbsoluteX);
    instructionVector[0x79] = Instruction(0x79, 3, 4, adcAbsoluteY);
    instructionVector[0x61] = Instruction(0x61, 2, 6, adcIndirectX);
    instructionVector[0x71] = Instruction(0x71, 2, 5, adcIndirectY);
    instructionVector[0x10] = Instruction(0x10, 2, 2, bpl);
    instructionVector[0xC9] = Instruction(0xC9, 2, 2, cmpImmediate);
    instructionVector[0xC5] = Instruction(0xC5, 2, 3, cmpZeroPage);
    instructionVector[0xD5] = Instruction(0xD5, 2, 4, cmpZeroPageX);
    instructionVector[0xCD] = Instruction(0xCD, 3, 4, cmpAbsolute);
    instructionVector[0xDD] = Instruction(0xDD, 3, 4, cmpAbsoluteX);
    instructionVector[0xD9] = Instruction(0xD9, 3, 4, cmpAbsoluteY);
    instructionVector[0xC1] = Instruction(0xC1, 2, 6, cmpIndirectX);
    instructionVector[0xD1] = Instruction(0xD1, 2, 5, cmpIndirectY);
    instructionVector[0x4A] = Instruction(0x4A, 1, 2, lsrAccumulator);
    instructionVector[0x46] = Instruction(0x46, 2, 5, lsrZeropage);
    instructionVector[0x56] = Instruction(0x56, 2, 6, lsrZeropageX);
    instructionVector[0x4E] = Instruction(0x4E, 3, 6, lsrAbsolute);
    instructionVector[0x5E] = Instruction(0x5E, 3, 7, lsrAbsoluteX);
    instructionVector[0x86] = Instruction(0x86, 2, 3, stxZeroPage);
    instructionVector[0x96] = Instruction(0x96, 2, 4, stxZeroPageY);
    instructionVector[0x8E] = Instruction(0x8E, 3, 4, stxAbsolute);
    instructionVector[0xBA] = Instruction(0xBA, 1, 2, tsx);
    instructionVector[0xA2] = Instruction(0xA2, 2, 2, ldxImmediate);
    instructionVector[0xA6] = Instruction(0xA6, 2, 3, ldxZeroPage);
    instructionVector[0xB6] = Instruction(0xB6, 2, 4, ldxZeroPageY);
    instructionVector[0xAE] = Instruction(0xAE, 3, 4, ldxAbsolute);
    instructionVector[0xBE] = Instruction(0xBE, 3, 4, ldxAbsoluteY);
    instructionVector[0xD8] = Instruction(0xD8, 1, 2, cld);
    instructionVector[0x08] = Instruction(0x08, 1, 3, php);
    instructionVector[0xA9] = Instruction(0xA9, 2, 2, ldaImmediate);
    instructionVector[0xA5] = Instruction(0xA5, 2, 3, ldaZeroPage);
    instructionVector[0xB5] = Instruction(0xB5, 2, 4, ldaZeroPageX);
    instructionVector[0xAD] = Instruction(0xAD, 3, 4, ldaAbsolute);
    instructionVector[0xBD] = Instruction(0xBD, 3, 4, ldaAbsoluteX);
    instructionVector[0xB9] = Instruction(0xB9, 3, 4, ldaAbsoluteY);
    instructionVector[0xA1] = Instruction(0xA1, 2, 6, ldaIndirectX);
    instructionVector[0xB1] = Instruction(0xB1, 2, 5, ldaIndirectY);
    instructionVector[0xC0] = Instruction(0xC0, 2, 2, cpyImmediate);
    instructionVector[0xC4] = Instruction(0xC4, 2, 3, cpyZeroPage);
    instructionVector[0xCC] = Instruction(0xCC, 3, 4, cpyAbsolute);
    instructionVector[0x9A] = Instruction(0x9A, 1, 2, txs);
    instructionVector[0x84] = Instruction(0x84, 2, 3, styZeroPage);
    instructionVector[0x94] = Instruction(0x94, 2, 4, styZeroPageX);
    instructionVector[0x8C] = Instruction(0x8C, 3, 4, styAbsolute);
    instructionVector[0xB8] = Instruction(0xB8, 1, 2, clv);
    instructionVector[0x30] = Instruction(0x30, 2, 2, bmi);
    instructionVector[0xC6] = Instruction(0xC6, 2, 5, decZeroPage);
    instructionVector[0xD6] = Instruction(0xD6, 2, 6, decZeroPageX);
    instructionVector[0xCE] = Instruction(0xCE, 3, 3, decAbsolute);
    instructionVector[0xDE] = Instruction(0xDE, 3, 7, decAbsoluteX);
    instructionVector[0x2A] = Instruction(0x2A, 1, 2, rolAccumulator);
    instructionVector[0x26] = Instruction(0x26, 2, 5, rolZeropage);
    instructionVector[0x36] = Instruction(0x36, 2, 6, rolZeropageX);
    instructionVector[0x2E] = Instruction(0x2E, 3, 6, rolAbsolute);
    instructionVector[0x3E] = Instruction(0x3E, 3, 7, rolAbsoluteX);
    instructionVector[0x58] = Instruction(0x58, 1, 2, cli);
    instructionVector[0x98] = Instruction(0x98, 1, 2, tya);
    instructionVector[0x49] = Instruction(0x49, 2, 2, eorImmediate);
    instructionVector[0x45] = Instruction(0x45, 2, 3, eorZeroPage);
    instructionVector[0x55] = Instruction(0x55, 2, 4, eorZeroPageX);
    instructionVector[0x4D] = Instruction(0x4D, 3, 4, eorAbsolute);
    instructionVector[0x5D] = Instruction(0x5D, 3, 4, eorAbsoluteX);
    instructionVector[0x59] = Instruction(0x59, 3, 4, eorAbsoluteY);
    instructionVector[0x41] = Instruction(0x41, 2, 6, eorIndirectX);
    instructionVector[0x51] = Instruction(0x51, 2, 5, eorIndirectY);
    instructionVector[0x85] = Instruction(0x85, 2, 3, staZeroPage);
    instructionVector[0x95] = Instruction(0x95, 2, 4, staZeroPageX);
    instructionVector[0x8D] = Instruction(0x8D, 3, 4, staAbsolute);
    instructionVector[0x9D] = Instruction(0x9D, 3, 5, staAbsoluteX);
    instructionVector[0x99] = Instruction(0x99, 3, 5, staAbsoluteY);
    instructionVector[0x81] = Instruction(0x81, 2, 6, staIndirectX);
    instructionVector[0x91] = Instruction(0x91, 2, 6, staIndirectY);
    instructionVector[0x70] = Instruction(0x70, 2, 2, bvs);
    instructionVector[0xE0] = Instruction(0xE0, 2, 2, cpxImmediate);
    instructionVector[0xE4] = Instruction(0xE4, 2, 3, cpxZeroPage);
    instructionVector[0xEC] = Instruction(0xEC, 3, 4, cpxAbsolute);
    instructionVector[0x8A] = Instruction(0x8A, 1, 2, txa);
    instructionVector[0xF0] = Instruction(0xF0, 2, 2, beq);
    instructionVector[0xE9] = Instruction(0xE9, 2, 2, sbcImmediate);
    instructionVector[0xE5] = Instruction(0xE5, 2, 3, sbcZeroPage);
    instructionVector[0xF5] = Instruction(0xF5, 2, 4, sbcZeroPageX);
    instructionVector[0xED] = Instruction(0xED, 3, 4, sbcAbsolute);
    instructionVector[0xFD] = Instruction(0xFD, 3, 4, sbcAbsoluteX);
    instructionVector[0xF9] = Instruction(0xF9, 3, 4, sbcAbsoluteY);
    instructionVector[0xE1] = Instruction(0xE1, 2, 6, sbcIndirectX);
    instructionVector[0xF1] = Instruction(0xF1, 2, 5, sbcIndirectY);
    instructionVector[0x40] = Instruction(0x40, 1, 6, rti);
    instructionVector[0xEA] = Instruction(0xEA, 1, 2, nop);
    instructionVector[0x48] = Instruction(0x48, 1, 3, pha);
    instructionVector[0x18] = Instruction(0x18, 1, 2, clc);
    instructionVector[0x0A] = Instruction(0x0A, 1, 2, aslAccumulator);
    instructionVector[0x06] = Instruction(0x06, 2, 5, aslZeropage);
    instructionVector[0x16] = Instruction(0x16, 2, 6, aslZeropageX);
    instructionVector[0x0E] = Instruction(0x0E, 3, 6, aslAbsolute);
    instructionVector[0x1E] = Instruction(0x1E, 3, 7, aslAbsoluteX);
    instructionVector[0x09] = Instruction(0x09, 2, 2, oraImmediate);
    instructionVector[0x05] = Instruction(0x05, 2, 3, oraZeroPage);
    instructionVector[0x15] = Instruction(0x15, 2, 4, oraZeroPageX);
    instructionVector[0x0D] = Instruction(0x0D, 3, 4, oraAbsolute);
    instructionVector[0x1D] = Instruction(0x1D, 3, 4, oraAbsoluteX);
    instructionVector[0x19] = Instruction(0x19, 3, 4, oraAbsoluteY);
    instructionVector[0x01] = Instruction(0x01, 2, 6, oraIndirectX);
    instructionVector[0x11] = Instruction(0x11, 2, 5, oraIndirectY);
    instructionVector[0x20] = Instruction(0x20, 3, 6, jsr);
    instructionVector[0x78] = Instruction(0x78, 1, 2, sei);
    instructionVector[0x6A] = Instruction(0x6A, 1, 2, rorAccumulator);
    instructionVector[0x66] = Instruction(0x66, 2, 5, rorZeropage);
    instructionVector[0x76] = Instruction(0x76, 2, 6, rorZeropageX);
    instructionVector[0x6E] = Instruction(0x6E, 3, 6, rorAbsolute);
    instructionVector[0x7E] = Instruction(0x7E, 3, 7, rorAbsoluteX);
    instructionVector[0x60] = Instruction(0x60, 1, 6, rts);
}

}

} // namespace Hardware