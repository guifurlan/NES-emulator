InstructionRunnerReturn stxZeroPage(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.memory.getByteAbsolute(address) = cpu.registerX;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
InstructionRunnerReturn stxZeroPageY(CPU& cpu, const Instruction& instruction) {
    const auto address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    cpu.memory.getByteAbsolute((address + cpu.registerY)%256) = cpu.registerX;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            (address + cpu.registerY)%256
    };
}

InstructionRunnerReturn stxAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto lowbyte = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto hibyte = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t address = ((uint16_t)hibyte << 8) + (uint16_t)lowbyte;
    cpu.memory.getByteAbsolute(address) = cpu.registerX;

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            address
    };
}
