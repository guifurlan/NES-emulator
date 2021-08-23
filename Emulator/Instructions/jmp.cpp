InstructionRunnerReturn jmpAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    cpu.programCounter = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;

    return {
            cpu.programCounter,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn jmpIndirect(CPU& cpu, const Instruction& instruction) {
    const auto address1 = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto address2 = cpu.memory.getByteAbsolute(cpu.programCounter + 2);

    uint16_t indirect_address1 = ((uint16_t)address2 << 8) + (uint16_t)address1;
    uint16_t  indirect_address2 =indirect_address1 +1;

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute(indirect_address1);
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute(indirect_address2);
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;

    cpu.programCounter = effective_address;

    return {
            cpu.programCounter,
            instruction.numberOfCycles,
            -1
    };
}