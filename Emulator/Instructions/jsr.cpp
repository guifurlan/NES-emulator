InstructionRunnerReturn jsr(CPU& cpu, const Instruction& instruction) {
    cpu.pushToStack(((cpu.programCounter + 2) >> 8) & 0xFF);
    cpu.pushToStack((cpu.programCounter + 2) & 0xFF);

    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.programCounter = address;

    return {
        cpu.programCounter,
        instruction.numberOfCycles,
        -1
    };
}
