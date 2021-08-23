InstructionRunnerReturn txs(CPU& cpu, const Instruction& instruction) {
    cpu.stackPointer = cpu.registerX;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
