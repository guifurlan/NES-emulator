// PHA  Push Accumulator on Stack
//
//      push A                           N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      implied       PHA           48    1     3

// PHA instruction
InstructionRunnerReturn pha(CPU& cpu, const Instruction& instruction) {
  const auto stackAddress = cpu.stackPointer + 0x0100;

  cpu.pushToStack(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    stackAddress                                      // accessed memory
  };
}
