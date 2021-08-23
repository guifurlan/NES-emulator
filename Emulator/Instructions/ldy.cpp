InstructionRunnerReturn ldyImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerY = immediate;

    cpu.setN(cpu.registerY);
    cpu.setZ(cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn ldyZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerY = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerY);
    cpu.setZ(cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
InstructionRunnerReturn ldyZeroPageX(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerY = cpu.memory.getByteAbsolute((address + cpu.registerX)%256);

    cpu.setN(cpu.registerY);
    cpu.setZ(cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerX)%256
    };
}

InstructionRunnerReturn ldyAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerY = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerY);
    cpu.setZ(cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn ldyAbsoluteX(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerY = cpu.memory.getByteAbsolute(address + cpu.registerX);

    cpu.setN(cpu.registerY);
    cpu.setZ(cpu.registerY);

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
