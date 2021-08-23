InstructionRunnerReturn nop(CPU& cpu, const Instruction& instruction) {

    return {
            cpu.programCounter + instruction.instructionSize,
            instruction.numberOfCycles,
            -1
    };

}
