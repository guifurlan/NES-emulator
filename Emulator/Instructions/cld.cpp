InstructionRunnerReturn cld(CPU& cpu, const Instruction& instruction) {
    cpu.status.set(static_cast<int>(StatusBit::D), false);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
