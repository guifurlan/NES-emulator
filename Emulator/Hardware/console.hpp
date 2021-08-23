#ifndef HEADER_CONSOLE_HPP
#define HEADER_CONSOLE_HPP

#include <string>

#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "window.hpp"
#include "ppu.hpp"

namespace Hardware
{

class Console {
private:
    Cartridge gameCartridge;
    Hardware::Window window;
    Memory cpuMemory;
    CPU cpu;
    PPU ppu;

public:
    Console(std::string& nesFilename, bool enable_window = true);

    void run();

    void CpuNmiInterruption();
};

} // namespace Hardware

#endif // HEADER_CONSOLE_HPP