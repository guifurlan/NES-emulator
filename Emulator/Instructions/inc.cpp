InstructionRunnerReturn incZeroPage(CPU& cpu, const Instruction& instruction) {

    uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    auto mem = cpu.memory.getByteAbsolute(address);

    mem++;

    cpu.setN(mem);
    cpu.setZ(mem);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };

}

InstructionRunnerReturn incZeroPageX(CPU& cpu, const Instruction& instruction) {

    uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    auto mem = cpu.memory.getByteAbsolute((address + cpu.registerX)%256);

    mem++;

    cpu.setN(mem);
    cpu.setZ(mem);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerX)%256
    };

}

InstructionRunnerReturn incAbsolute(CPU& cpu, const Instruction& instruction) {

    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);

    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    auto mem = cpu.memory.getByteAbsolute(address);

    mem++;

    cpu.setN(mem);
    cpu.setZ(mem);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };

}

InstructionRunnerReturn incAbsoluteX(CPU& cpu, const Instruction& instruction) {

    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);

    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    auto mem = cpu.memory.getByteAbsolute(address + cpu.registerX);

    mem++;

    cpu.setN(mem);
    cpu.setZ(mem);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address + cpu.registerX
    };

}
