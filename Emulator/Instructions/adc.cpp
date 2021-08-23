InstructionRunnerReturn adcImmediate(CPU& cpu, const Instruction& instruction) {
  const auto immediate = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  int sum = cpu.registerA + immediate + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ immediate) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    -1
  };
}

InstructionRunnerReturn adcZeroPage(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  int m = cpu.memory.getByteAbsolute(addr);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    addr
  };
}

InstructionRunnerReturn adcZeroPageX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  int m = cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    (addr + cpu.registerX) % 256
  };
}

InstructionRunnerReturn adcAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    addr
  };
}

InstructionRunnerReturn adcAbsoluteX(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr + cpu.registerX);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

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

InstructionRunnerReturn adcAbsoluteY(CPU& cpu, const Instruction& instruction) {
  const auto addrL = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addrH = cpu.memory.getByteAbsolute(cpu.programCounter + 2);
  uint16_t addr = (uint16_t)addrL + ((uint16_t)addrH << 8);
  int m = cpu.memory.getByteAbsolute(addr + cpu.registerY);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

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

InstructionRunnerReturn adcIndirectX(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr + cpu.registerX) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + cpu.registerX + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);

  int m = cpu.memory.getByteAbsolute(effective_addr);
  
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

  cpu.setN(cpu.registerA);
  cpu.setZ(cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize,
    instruction.numberOfCycles,
    effective_addr
  };
}

InstructionRunnerReturn adcIndirectY(CPU& cpu, const Instruction& instruction) {
  const auto addr = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  uint16_t effective_addrL = cpu.memory.getByteAbsolute((addr) % 256);
  uint16_t effective_addrH = cpu.memory.getByteAbsolute((addr + 1) % 256);
  uint16_t effective_addr = effective_addrL + (effective_addrH << 8);

  int m = cpu.memory.getByteAbsolute(effective_addr + cpu.registerY);
  int sum = cpu.registerA + m + cpu.status.test(static_cast<int>(StatusBit::C));
  
  cpu.status.set(static_cast<int>(StatusBit::C), sum > 255);

  int overflow = ~(cpu.registerA ^ m) & (cpu.registerA ^ sum) & 0x80;
  cpu.status.set(static_cast<int>(StatusBit::V), overflow);
  
  cpu.registerA = static_cast<uint8_t>(sum);

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









