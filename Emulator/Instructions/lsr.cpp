// LSR  Shift One Bit Right (Memory or Accumulator)
//
//      0 -> [76543210] -> C             N Z C I D V
//                                       0 + + - - -
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      accumulator   LSR A         4A    1     2
//      zeropage      LSR oper      46    2     5
//      zeropage,X    LSR oper,X    56    2     6
//      absolute      LSR oper      4E    3     6
//      absolute,X    LSR oper,X    5E    3     7

// LSR instruction core
void lsr(CPU& cpu, Memory::MemoryData& value) {
  // Get carry before lsr
  const bool carry = value & 0x01;

  // Shift the value logically
  value >>= 1;

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setStatusBit(StatusBit::N, false);
}

void lsr(CPU& cpu, uint8_t& value) {
  // Get carry before lsr
  const bool carry = value & 0x01;

  // Shift the value logically
  value >>= 1;

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setStatusBit(StatusBit::N, false);
}

// LSR variation
InstructionRunnerReturn lsrAccumulator(CPU& cpu, const Instruction& instruction) {
  lsr(cpu, cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    -1                                                // accessed memory
  };
}

InstructionRunnerReturn lsrZeropage(CPU& cpu, const Instruction& instruction) {
  const uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  lsr(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn lsrZeropageX(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addressX = (address + cpu.registerX) % 256;

  auto value = cpu.memory.getByteAbsolute(addressX);

  lsr(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    addressX                                          // accessed memory
  };
}

InstructionRunnerReturn lsrAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  lsr(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn lsrAbsoluteX(CPU& cpu, const Instruction& instruction) {
  auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);
  auto addressX = address + cpu.registerX;

  const auto pageChange = (address >> 8) != (addressX >> 8);

  auto value = cpu.memory.getByteAbsolute(addressX);

  lsr(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles + pageChange,          // # cycles
    addressX                                          // accessed memory
  };
}
