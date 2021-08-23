InstructionRunnerReturn ldaImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerA = immediate;

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn ldaZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerA = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
InstructionRunnerReturn ldaZeroPageX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerA = cpu.memory.getByteAbsolute((address + cpu.registerX)%256);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerX)%256
    };
}

InstructionRunnerReturn ldaAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerA = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn ldaAbsoluteX(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerA = cpu.memory.getByteAbsolute(address + cpu.registerX);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    auto returnCycles = instruction.numberOfCycles;

    if(!cpu.isSamePage(address, address + cpu.registerX)){
        returnCycles++;
    }

    return {
            cpu.programCounter + instruction.instructionSize,
            returnCycles,
            address + cpu.registerX
    };
}

InstructionRunnerReturn ldaAbsoluteY(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerA = cpu.memory.getByteAbsolute(address + cpu.registerY);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);
    
    auto returnCycles = instruction.numberOfCycles;

    if(!cpu.isSamePage(address, address + cpu.registerY)){
        returnCycles++;
    }

    return {
            cpu.programCounter + instruction.instructionSize,
            returnCycles,
            address + cpu.registerY
    };
}

InstructionRunnerReturn ldaIndirectX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute((address + cpu.registerX)%256);
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute((address + cpu.registerX + 1)%256);
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    cpu.registerA = cpu.memory.getByteAbsolute(effective_address);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            effective_address
    };
}

InstructionRunnerReturn ldaIndirectY(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute(address);
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute((address+1)%256);
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    cpu.registerA = cpu.memory.getByteAbsolute(effective_address + cpu.registerY);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    auto returnCycles = instruction.numberOfCycles;

    if(!cpu.isSamePage(effective_address, effective_address + cpu.registerY)){
        returnCycles++;
    }

    return {
            cpu.programCounter + instruction.instructionSize,
            returnCycles,
            effective_address + cpu.registerY
    };
}
