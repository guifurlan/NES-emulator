InstructionRunnerReturn inx(CPU& cpu, const Instruction& instruction) {

    cpu.registerX++;

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
