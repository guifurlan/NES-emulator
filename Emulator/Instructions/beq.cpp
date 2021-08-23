InstructionRunnerReturn beq(CPU& cpu, const Instruction& instruction) {
    const int8_t& jump = cpu.memory.getByteAbsolute(cpu.programCounter + 1);

    if(cpu.status.test(static_cast<int>(StatusBit::Z))){
        auto returnCycles = instruction.numberOfCycles;
        if(!cpu.isSamePage(cpu.programCounter + instruction.instructionSize + jump ,
                           cpu.programCounter + instruction.instructionSize)){
            returnCycles += 2;
        }
        else{
            returnCycles += 1;
        }
        return {
                cpu.programCounter + instruction.instructionSize + jump ,
                returnCycles,
                -1
        };
    }

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };
}
