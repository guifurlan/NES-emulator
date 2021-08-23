InstructionRunnerReturn tsx(CPU& cpu, const Instruction& instruction) {
    cpu.registerX = cpu.stackPointer;

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
