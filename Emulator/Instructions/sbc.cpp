void calcSub(CPU& cpu, uint8_t value) {
  uint16_t carryExtended = cpu.getStatusBit(StatusBit::C);
  uint16_t regAExt = static_cast<uint16_t>(cpu.registerA);
  uint16_t valueExt = static_cast<uint16_t>(value) ^ 0x00FF;
  uint16_t sub = regAExt + valueExt + carryExtended;
  
  auto overflow = ~(regAExt ^ valueExt) & (regAExt ^ sub) & 0x0080;
  cpu.setStatusBit(StatusBit::C, sub > 255);
  cpu.setStatusBit(StatusBit::V, overflow);
  
  cpu.registerA = static_cast<uint8_t>(sub);
  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);
}

InstructionRunnerReturn sbcImmediate(CPU& cpu, const Instruction& instruction) {
  const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  calcSub(cpu, immediate);
  
  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    -1
  };
}

InstructionRunnerReturn sbcZeroPage(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  int m = cpu.memory.getByteAbsolute(addr);
  
  calcSub(cpu, m);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    addr
  };
}

InstructionRunnerReturn sbcZeroPageX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  int m = cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);
  
  calcSub(cpu, m);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    (addr + cpu.registerX) % 256
  };
}

InstructionRunnerReturn sbcAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr);

  calcSub(cpu, m);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    addr
  };
}

InstructionRunnerReturn sbcAbsoluteX(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr + cpu.registerX);
  
  calcSub(cpu, m);

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

InstructionRunnerReturn sbcAbsoluteY(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr + cpu.registerY);
  
  calcSub(cpu, m);

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

InstructionRunnerReturn sbcIndirectX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + cpu.registerX + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);
  int m = cpu.memory.getByteAbsolute(effective_addr);
  
  calcSub(cpu, m);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    effective_addr
  };
}

InstructionRunnerReturn sbcIndirectY(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);
  int m = cpu.memory.getByteAbsolute(effective_addr + cpu.registerY);

  calcSub(cpu, m);

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
