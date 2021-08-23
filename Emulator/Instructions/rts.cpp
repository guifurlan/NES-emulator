InstructionRunnerReturn rts(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.pullFromStack();
    const auto hibyte = cpu.pullFromStack();
    cpu.programCounter = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.programCounter++;

    return {
            cpu.programCounter,
            instruction.numberOfCycles,
            -1
    };
}
