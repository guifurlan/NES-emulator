InstructionRunnerReturn cpxImmediate(CPU& cpu, const Instruction& instruction) {
    const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerX >= immediate);
    cpu.setN(cpu.registerX - immediate);
    cpu.status.set(static_cast<int>(StatusBit::Z), immediate==cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}

InstructionRunnerReturn cpxZeroPage(CPU& cpu, const Instruction& instruction) {

    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerX >= m);
    cpu.setN(cpu.registerX - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}

InstructionRunnerReturn cpxAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    const auto m = cpu.memory.getByteAbsolute(address);

    cpu.status.set(static_cast<int>(StatusBit::C), cpu.registerX >= m);
    cpu.setN(cpu.registerX - m);
    cpu.status.set(static_cast<int>(StatusBit::Z), m==cpu.registerX);

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
