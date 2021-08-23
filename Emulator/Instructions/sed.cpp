InstructionRunnerReturn sed(CPU& cpu, const Instruction& instruction) {
    cpu.status.set(static_cast<int>(StatusBit::D), true);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
