// ROL  Rotate One Bit Left (Memory or Accumulator)
//
//      C <- [76543210] <- C             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      accumulator   ROL A         2A    1     2
//      zeropage      ROL oper      26    2     5
//      zeropage,X    ROL oper,X    36    2     6
//      absolute      ROL oper      2E    3     6
//      absolute,X    ROL oper,X    3E    3     7

// ROL instruction core
void rol(CPU& cpu, Memory::MemoryData& value) {
  // Get carry before rol
  const bool carry = value & 0x80;

  // Shift the value logically
  value <<= 1;

  // Set rotated bit with carry
  const bool oldCarry = cpu.getStatusBit(StatusBit::C);
  value |= static_cast<uint8_t>(oldCarry);

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

void rol(CPU& cpu, uint8_t& value) {
  // Get carry before rol
  const bool carry = value & 0x80;

  // Shift the value logically
  value <<= 1;

  // Set rotated bit with carry
  const bool oldCarry = cpu.getStatusBit(StatusBit::C);
  value |= static_cast<uint8_t>(oldCarry);

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

// ROL variation
InstructionRunnerReturn rolAccumulator(CPU& cpu, const Instruction& instruction) {
  rol(cpu, cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    -1                                                // accessed memory
  };
}

InstructionRunnerReturn rolZeropage(CPU& cpu, const Instruction& instruction) {
  const uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  rol(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn rolZeropageX(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addressX = (address + cpu.registerX) % 256;

  auto value = cpu.memory.getByteAbsolute(addressX);

  rol(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    addressX                                          // accessed memory
  };
}

InstructionRunnerReturn rolAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  rol(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn rolAbsoluteX(CPU& cpu, const Instruction& instruction) {
  auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);
  auto addressX = address + cpu.registerX;

  const auto pageChange = (address >> 8) != (addressX >> 8);

  auto value = cpu.memory.getByteAbsolute(addressX);

  rol(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles + pageChange,          // # cycles
    addressX                                          // accessed memory
  };
}
