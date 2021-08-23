// PLP  Pull Processor Status from Stack
//
//      pull SR                          N Z C I D V
//                                       from stack
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      implied       PLP           28    1     4

// PLP instruction
InstructionRunnerReturn plp(CPU& cpu, const Instruction& instruction) {
  constexpr uint8_t statusMask = 0xCF;
  cpu.status &= ~statusMask;
  cpu.status |= cpu.pullFromStack() & statusMask;

  const auto stackAddress = cpu.stackPointer + 0x0100;

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    stackAddress                                      // accessed memory
  };
}
