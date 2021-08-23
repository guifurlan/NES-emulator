InstructionRunnerReturn rti(CPU& cpu, const Instruction& instruction) {
    constexpr uint8_t statusMask = 0xCF;
    cpu.status = cpu.pullFromStack() & statusMask;

    const auto lowbyte = cpu.pullFromStack();
    const auto hibyte = cpu.pullFromStack();
    cpu.programCounter = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;

    return {
            cpu.programCounter,
            instruction.numberOfCycles,
            -1
    };
}
