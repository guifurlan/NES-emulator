InstructionRunnerReturn andImmediate(CPU& cpu, const Instruction& instruction) {
  const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  cpu.registerA &= immediate;

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    -1
  };
}

InstructionRunnerReturn andZeroPage(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  cpu.registerA &= cpu.memory.getByteAbsolute(addr);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    addr
  };
}

InstructionRunnerReturn andZeroPageX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  cpu.registerA &= cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    (addr + cpu.registerX) % 256
  };
}

InstructionRunnerReturn andAbsolute(CPU& cpu, const Instruction& instruction) {
    const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
    const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
    uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);

    cpu.registerA &= cpu.memory.getByteAbsolute(addr);

    cpu.setN(cpu.registerA);
    cpu.setZ(cpu.registerA);

    return {
      cpu.programCounter + instruction.instructionSize,
      instruction.numberOfCycles,
      addr
    };
}

InstructionRunnerReturn andAbsoluteX(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);

  cpu.registerA &= cpu.memory.getByteAbsolute(addr + cpu.registerX);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  auto returnCycles = instruction.numberOfCycles;

  if(!cpu.isSamePage(addr, addr + cpu.registerX)){
      returnCycles++;
  }

  return {
    cpu.programCounter + instruction.instructionSize,
    returnCycles,
    addr + cpu.registerX
  };
}

InstructionRunnerReturn andAbsoluteY(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);

  cpu.registerA &= cpu.memory.getByteAbsolute(addr + cpu.registerY);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  auto returnCycles = instruction.numberOfCycles;

  if(!cpu.isSamePage(addr, addr + cpu.registerY)){
      returnCycles++;
  }

  return {
    cpu.programCounter + instruction.instructionSize,
    returnCycles,
    addr + cpu.registerY
  };
}

InstructionRunnerReturn andIndirectX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + cpu.registerX + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);

  cpu.registerA &= cpu.memory.getByteAbsolute(effective_addr);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    effective_addr
  };
}

InstructionRunnerReturn andIndirectY(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);

  cpu.registerA &= cpu.memory.getByteAbsolute(effective_addr + cpu.registerY);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  auto returnCycles = instruction.numberOfCycles;

  if(!cpu.isSamePage(effective_addr, effective_addr + cpu.registerY)){
      returnCycles++;
  }

  return {
    cpu.programCounter + instruction.instructionSize,
    returnCycles,
    effective_addr + cpu.registerY
  };
}
