InstructionRunnerReturn clv(CPU& cpu, const Instruction& instruction) {
    cpu.status.set(static_cast<int>(StatusBit::V), false);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
