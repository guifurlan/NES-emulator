InstructionRunnerReturn tax(CPU& cpu, const Instruction& instruction) {
    cpu.registerX = cpu.registerA;

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
