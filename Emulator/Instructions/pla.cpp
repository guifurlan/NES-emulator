// PLA  Pull Accumulator from Stack
//
//      pull A                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      implied       PLA           68    1     4

// PLA instruction
InstructionRunnerReturn pla(CPU& cpu, const Instruction& instruction) {
  cpu.registerA = cpu.pullFromStack();

  const auto stackAddress = cpu.stackPointer + 0x0100;

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    stackAddress                                      // accessed memory
  };
}
