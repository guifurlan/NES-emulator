#ifndef HEADER_INSTRUCTION_HPP
#define HEADER_INSTRUCTION_HPP

// Cpp headers
#include <array>
#include <tuple>
#include <type_traits>

// C headers
#include <cstdint>

namespace Hardware {

// Forward declaration of the CPU, Instruction classes
class CPU;
class Instruction;

// Declare the instruction runner function type.
// This is the only function signature allow to be executed by an instruction.
using InstructionRunnerReturn = std::tuple<std::uint16_t, int, int>;
using InstructionRunnerPtr = std::add_pointer<InstructionRunnerReturn(CPU&, const Instruction&)>::type;

// The main instruction class. Each instance of it will store the instruction
// parameters and instruction runner that will execute it. The execution method.
class Instruction {

private:
    
  // Execution method
  InstructionRunnerPtr runnerPtr;

public:

  // Instruction parameters
  std::uint8_t opcode;
  std::size_t instructionSize;
  std::size_t numberOfCycles;

  // The instruction status
  bool isImplemented;

public:

  Instruction();

  Instruction(std::uint8_t opcode);

  Instruction (std::uint8_t opcode,
               std::uint16_t instructionSize,
               int numberOfCycles,
               InstructionRunnerPtr runnerPtr);

  InstructionRunnerReturn run(CPU& cpu);

};

namespace Instructions {

extern Instruction instructionVector[256];

void initInstructionVector();

} // namespace Instructions

} // namespace Hardware

#endif // HEADER_INSTRUCTION_HPP