#ifndef GUARD_MEMORY_HPP
#define GUARD_MEMORY_HPP

// C headers
#include <cstdint>

// Cpp headers
#include <string>
#include <array>

#include "cartridge.hpp"

namespace Hardware {

// Forward declaration
class PPU;
class Window;

struct Memory {

struct MemoryData {
private:
  Memory& memory;
  uint16_t address;

public:
  MemoryData(uint16_t address, Memory& memory) :
    memory(memory),
    address(address)
  {
  }

  operator uint8_t() const {
    return memory.readByteAbsolute(address);
  }

  MemoryData& operator=(uint8_t newValue) {
    memory.writeByteAbsolute(address, newValue);
    return *this;
  }

  MemoryData& operator<<=(int shift_count) {
    return (*this = *this << shift_count);
  }

  MemoryData& operator>>=(int shift_count) {
    return (*this = *this >> shift_count);
  }

  MemoryData& operator|=(uint8_t value) {
    return (*this = *this | value);
  }

  MemoryData& operator++() {
    return (*this = *this + 1);
  }

  MemoryData operator++(int) {
    MemoryData tmp(*this);
    operator++();
    return tmp;
  }

  MemoryData& operator--() {
    return (*this = *this - 1);
  }

  MemoryData operator--(int) {
    MemoryData tmp(*this);
    operator--();
    return tmp;
  }
};

private:

    // The CPU accessable memory
    std::array<std::uint8_t, 0x10000> memoryBuffer;

    // Reference to IO hardware
    PPU& ppu;
    Window& window;

    // IO states
    uint8_t controller[2];

public:

    Memory(Cartridge cartridge, PPU& ppu, Window& window);

    // Memory access
    MemoryData getByteAbsolute(std::uint16_t absolute_address);
    std::uint16_t readWordAbsolute(std::uint16_t absolute_address);

    // Internal memory address
    std::uint8_t readByteAbsolute(std::uint16_t absolute_address);
    void writeByteAbsolute(std::uint16_t absolute_address, uint8_t new_value);

    // Memory mirroring
    static std::uint16_t getRealMemoryAddress(std::uint16_t absolute_address);
};

}

#endif // GUARD_MEMORY_HPP