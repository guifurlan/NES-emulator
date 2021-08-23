InstructionRunnerReturn tya(CPU& cpu, const Instruction& instruction) {
    cpu.registerA = cpu.registerY;

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
        cpu.programCounter + instruction.instructionSize,
        instruction.numberOfCycles,
        -1
    };
}
