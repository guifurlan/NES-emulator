InstructionRunnerReturn tay(CPU& cpu, const Instruction& instruction) {
  cpu.registerY = cpu.registerA;

  cpu.setN(cpu.registerY);
  cpu.setZ(cpu.registerY);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    -1
  };
}
