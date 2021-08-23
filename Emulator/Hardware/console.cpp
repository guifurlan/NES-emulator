#include "console.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "window.hpp"

using namespace std;
using namespace chrono;

namespace Hardware
{

Console::Console(std::string& nesFilename, bool isTest) :
    gameCartridge(nesFilename),
    window("screen", 426, 400, 256, 240, !isTest),
    cpuMemory(gameCartridge, ppu, window),
    cpu(cpuMemory, isTest),
    ppu(gameCartridge, window, cpu)
{
}

void Console::run() {
    using namespace chrono_literals;
    constexpr auto targetFrametime = 16ms;

    auto systemCycle = 0;

    // Loop until the user closes the screen or a brk instruction is executed
    while (!window.shouldStop()) {
        // Get the time before the current frame
        const auto beforeVerticalBlank = high_resolution_clock::now();

        // Runs until a new frame is displayed
        while (!window.clearVerticalBlank()) {
            // Only cycle the cpu once every 3 cycles
            if (systemCycle % 3 == 0) {
                if (!cpu.runInstruction()) {
                    break;
                }
            }
            // Cycle the PPU every cycle
            ppu.cycle();
            // TODO: Call APU "Instruction/Cycle" run method
            systemCycle++;
        }

        // Get the current frametime
        const auto currentFrametime = duration_cast<milliseconds>(high_resolution_clock::now() - beforeVerticalBlank);

        // Time synchronization
        cout << "Frametime: " << currentFrametime.count() << "ms" << endl;
        if (currentFrametime.count() < 16) {
            this_thread::sleep_for(targetFrametime - currentFrametime);
        }
    }
}

void Console::CpuNmiInterruption() {
    cpu.interruption();
}

} // namespace Hardware