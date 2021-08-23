// ASL  Shift Left One Bit (Memory or Accumulator)
//
//      C <- [76543210] <- 0             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   ASL A         0A    1     2
//      zeropage      ASL oper      06    2     5
//      zeropage,X    ASL oper,X    16    2     6
//      absolute      ASL oper      0E    3     6
//      absolute,X    ASL oper,X    1E    3     7

// ASL instruction core
void asl(CPU& cpu, Memory::MemoryData& value) {
  // Get carry before asl
  const bool carry = value & 0x80;

  // Shift the value logically
  value <<= 1;

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

void asl(CPU& cpu, uint8_t& value) {
  // Get carry before asl
  const bool carry = value & 0x80;

  // Shift the value logically
  value <<= 1;

  // Set the CPU status
  cpu.setStatusBit(StatusBit::C, carry);
  cpu.setZ(value);
  cpu.setN(value);
}

// ASL variation
InstructionRunnerReturn aslAccumulator(CPU& cpu, const Instruction& instruction) {
  asl(cpu, cpu.registerA);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    -1                                                // accessed memory
  };
}

InstructionRunnerReturn aslZeropage(CPU& cpu, const Instruction& instruction) {
  const uint16_t address = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  asl(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn aslZeropageX(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.getByteAbsolute(cpu.programCounter + 1);
  const auto addressX = (address + cpu.registerX) % 256;

  auto value = cpu.memory.getByteAbsolute(addressX);

  asl(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    addressX                                          // accessed memory
  };
}

InstructionRunnerReturn aslAbsolute(CPU& cpu, const Instruction& instruction) {
  const auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);

  auto value = cpu.memory.getByteAbsolute(address);

  asl(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    address                                           // accessed memory
  };
}

InstructionRunnerReturn aslAbsoluteX(CPU& cpu, const Instruction& instruction) {
  auto address  = cpu.memory.readWordAbsolute(cpu.programCounter + 1);
  auto addressX = address + cpu.registerX;

  const auto pageChange = (address >> 8) != (addressX >> 8);

  auto value = cpu.memory.getByteAbsolute(addressX);

  asl(cpu, value);

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles + pageChange,          // # cycles
    addressX                                          // accessed memory
  };
}
