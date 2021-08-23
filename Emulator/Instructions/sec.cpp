InstructionRunnerReturn sec(CPU& cpu, const Instruction& instruction) {
    cpu.status.set(static_cast<int>(StatusBit::C), true);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
