// PHP  Push Processor Status on Stack
//
//      push SR                          N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cycles
//      --------------------------------------------
//      implied       PHP           08    1     3

// PHP instruction
InstructionRunnerReturn php(CPU& cpu, const Instruction& instruction) {
  const auto stackAddress = cpu.stackPointer + 0x0100;

  constexpr uint8_t statusMask = 0x30;
  cpu.pushToStack(static_cast<uint8_t>(cpu.status.to_ulong() | statusMask));

  return {
    cpu.programCounter + instruction.instructionSize, // new PC
    instruction.numberOfCycles,                       // # cycles
    stackAddress                                      // accessed memory
  };
}
