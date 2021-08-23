InstructionRunnerReturn cmpImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= immediate);
    cpu.setN(cpu.registerA - immediate);
    cpu.status.set(static_cast<int>(StatusBit::Z), immediate==cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn cmpZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn cmpZeroPageX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    const auto m = cpu.memory.getByteAbsolute((address+cpu.registerX)%256);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerX)%256
    };
}

InstructionRunnerReturn cmpAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn cmpAbsoluteX(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    const auto m = cpu.memory.getByteAbsolute(address + cpu.registerX);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

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

InstructionRunnerReturn cmpAbsoluteY(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    const auto m = cpu.memory.getByteAbsolute(address + cpu.registerY);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

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

InstructionRunnerReturn cmpIndirectX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute(address + cpu.registerX)%256;
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute(address + cpu.registerX + 1)%256;
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    const auto m = cpu.memory.getByteAbsolute(effective_address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            effective_address
    };
}

InstructionRunnerReturn cmpIndirectY(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    uint8_t effective_address_lowbyte = cpu.memory.getByteAbsolute(address)%256;
    uint8_t effective_address_hibyte = cpu.memory.getByteAbsolute(address+1)%256;
    uint16_t effective_address = ((uint16_t)effective_address_hibyte << 8) + (uint16_t)effective_address_lowbyte;
    const auto m = cpu.memory.getByteAbsolute(effective_address + cpu.registerY);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerA >= m);
    cpu.setN(cpu.registerA - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerA);

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
