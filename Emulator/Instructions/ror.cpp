// ROR  Rotate One Bit Right (Memory or Accumulator)
//
//      C -> [76543210] -> C             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   ROR A         6A    1     2
//      zeropage      ROR oper      66    2     5
//      zeropage,X    ROR oper,X    76    2     6
//      absolute      ROR oper      6E    3     6
//      absolute,X    ROR oper,X    7E    3     7

// ROR instruction core
void ror(CPU& cpu, Memory::MemoryData& value) {
  // Get carry before ror
  const bool carry = value & 0x01;

  // Shift the value logically
  value >>= 1;

  // Set rotated bit with carry
  const bool oldCarry = cpu.getStatusBit(StatusBit::C);
  value |= static_cast<uint8_t>(oldCarry << 7);

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

void ror(CPU& cpu, uint8_t& value) {
  // Get carry before ror
  const bool carry = value & 0x01;

  // Shift the value logically
  value >>= 1;

  // Set rotated bit with carry
  const bool oldCarry = cpu.getStatusBit(StatusBit::C);
  value |= static_cast<uint8_t>(oldCarry << 7);

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

// ROR variation
InstructionRunnerReturn rorAccumulator(CPU& cpu, const Instruction& instruction) {
  ror(cpu, cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    -1                                                // accessed memory
  };
}

InstructionRunnerReturn rorZeropage(CPU& cpu, const Instruction& instruction) {
  const uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  ror(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn rorZeropageX(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addressX = (address + cpu.registerX) % 256;

  auto value = cpu.memory.getByteAbsolute(addressX);

  ror(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    addressX                                          // accessed memory
  };
}

InstructionRunnerReturn rorAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  ror(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn rorAbsoluteX(CPU& cpu, const Instruction& instruction) {
  auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);
  auto addressX = address + cpu.registerX;

  const auto pageChange = (address >> 8) != (addressX >> 8);

  auto value = cpu.memory.getByteAbsolute(addressX);

  ror(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles + pageChange,          // # cycles
    addressX                                          // accessed memory
  };
}
