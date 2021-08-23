InstructionRunnerReturn sei(CPU& cpu, const Instruction& instruction) {
    cpu.status.set(static_cast<int>(StatusBit::I), true);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
