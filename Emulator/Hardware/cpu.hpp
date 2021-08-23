#ifndef HEADER_CPU_HPP
#define HEADER_CPU_HPP

// C headers
#include <cstdint>

// Cpp headers
#include <array>
#include <chrono>
#include <string>
#include <bitset>

// Project headers
#include "memory.hpp"
#include "instructions.hpp"

namespace Hardware {

// The CPU intrinsic hardware specifications
namespace Specifications {
    constexpr std::chrono::nanoseconds cpuClockPeriod(559);
    constexpr std::size_t cpuAccesableMemorySize = 0x10000;
}

enum class StatusBit{
    C = 0, Z = 1, I = 2, D = 3, B = 4, V = 6, N = 7
};

struct CPU {

    public:
        
        // CPU registers
        std::uint8_t registerA;
        std::uint8_t registerX;
        std::uint8_t registerY;
        std::uint16_t programCounter;
        std::uint8_t stackPointer;
        std::bitset<8> status;
        short clockCounter;
        
        // System memory
        Memory memory;

        // Log mode
        bool enableLog;

    public:

        CPU(Memory& cpuMemory, bool enableLog);

        bool runInstruction();

        void setZ(uint8_t value);
        void setN(uint8_t value);
        void setStatusBit(StatusBit bitName, bool bitValue);
        bool getStatusBit(StatusBit bitName);

        void pushToStack(uint8_t value);
        uint8_t pullFromStack();

        bool isSamePage(uint16_t v1, uint16_t v2);

        void interruption();

    private:

        void triggerReset();

        void log(InstructionRunnerReturn instructionResult);

};

}

#endif // HEADER_CPU_HPP