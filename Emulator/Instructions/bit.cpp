InstructionRunnerReturn bitZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    uint8_t m = cpu.memory.getByteAbsolute(address);
    int aux = cpu.registerA & m;

    uint8_t bit6_mask = (1 << 6);
    uint8_t bit7_mask = (1 << 7);
    
    cpu.setN(m & bit7_mask);
    cpu.setZ(aux);

    cpu.status.set(static_cast<int>(StatusBit::V), m & bit6_mask);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn bitAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    uint8_t m = cpu.memory.getByteAbsolute(address);
    int aux = cpu.registerA & m;

    uint8_t bit6_mask = (1 << 6);
    uint8_t bit7_mask = (1 << 7);

    cpu.setN(m & bit7_mask);
    cpu.setZ(aux);

    cpu.status.set(static_cast<int>(StatusBit::V), m & bit6_mask);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
