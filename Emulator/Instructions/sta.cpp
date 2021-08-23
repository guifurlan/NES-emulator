InstructionRunnerReturn staZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.memory.getByteAbsolute(address) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
InstructionRunnerReturn staZeroPageX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.memory.getByteAbsolute((address + cpu.registerX)%256) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerX)%256
    };
}

InstructionRunnerReturn staAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.memory.getByteAbsolute(address) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn staAbsoluteX(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.memory.getByteAbsolute(address + cpu.registerX) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address + cpu.registerX
    };
}

InstructionRunnerReturn staAbsoluteY(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.memory.getByteAbsolute(address + cpu.registerY) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address + cpu.registerY
    };
}

InstructionRunnerReturn staIndirectX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute((address + cpu.registerX)%256);
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute((address + cpu.registerX + 1)%256);
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    cpu.memory.getByteAbsolute(effective_address) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            effective_address
    };
}

InstructionRunnerReturn staIndirectY(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute(address);
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute((address+1)%256);
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    cpu.memory.getByteAbsolute(effective_address + cpu.registerY) = cpu.registerA;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            effective_address + cpu.registerY
    };
}
