InstructionRunnerReturn cpyImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerY >= immediate);
    cpu.setN(cpu.registerY - immediate);
    cpu.status.set(static_cast<int>(StatusBit::Z), immediate==cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn cpyZeroPage(CPU& cpu, const Instruction& instruction) {

    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerY >= m);
    cpu.setN(cpu.registerY - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn cpyAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerY >= m);
    cpu.setN(cpu.registerY - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerY);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}