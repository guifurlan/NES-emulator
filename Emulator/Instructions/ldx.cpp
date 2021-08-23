InstructionRunnerReturn ldxImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerX = immediate;

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn ldxZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerX = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
InstructionRunnerReturn ldxZeroPageY(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.registerX = cpu.memory.getByteAbsolute((address + cpu.registerY)%256);

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerY)%256
    };
}

InstructionRunnerReturn ldxAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerX = cpu.memory.getByteAbsolute(address);

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn ldxAbsoluteY(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.registerX = cpu.memory.getByteAbsolute(address + cpu.registerY);

    cpu.setN(cpu.registerX);
    cpu.setZ(cpu.registerX);

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
