// Main header
#include "memory.hpp"

// C headers
#include <cassert>
#include <cstdint>

// Cpp headers
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

#include "ppu.hpp"
#include "window.hpp"

using namespace std;

namespace Hardware {


Memory::Memory(Cartridge cartridge, PPU& ppu, Window& window) :
  memoryBuffer(),
  ppu(ppu),
  window(window),
  controller{0, 0}
{
  // Copy the trainer data
  const auto& trainer = cartridge.trainerMemory;
  copy_n(trainer.data(), trainer.size(), &memoryBuffer[0x7000]);

  // Copy the prg rom data
  const auto& prgRom = cartridge.prgMemory;
  assert(cartridge.prgMemory.size() == 0x4000 ||
         cartridge.prgMemory.size() == 0x8000);
  copy_n(prgRom.data(), prgRom.size(), &memoryBuffer[0x10000 - cartridge.prgMemory.size()]);
}

Memory::MemoryData Memory::getByteAbsolute(uint16_t absolute_address)
{
  return {absolute_address, *this};
}

uint16_t Memory::readWordAbsolute(uint16_t absolute_address)
{
  return (memoryBuffer[absolute_address + 1] << 8) |
          memoryBuffer[absolute_address];
}

uint8_t Memory::readByteAbsolute(uint16_t absolute_address)
{
  uint8_t data = 0x00;

  auto realAddress = getRealMemoryAddress(absolute_address);
  if (realAddress <= 0x1FFF) {
    data = memoryBuffer[realAddress];
  } else if (realAddress <= 0x3FFF) {
    data = ppu.readCPU(realAddress);
  } else if (realAddress >= 0x4016 && realAddress <= 0x4017) {
    // Return controller data and shift the controller register
    data = controller[realAddress & 1];
    controller[realAddress & 1] >>= 1;
  } else {
    data = memoryBuffer[realAddress];
  }

  return data;
}

void Memory::writeByteAbsolute(uint16_t absolute_address, uint8_t new_value)
{
  auto realAddress = getRealMemoryAddress(absolute_address);
  memoryBuffer[realAddress] = new_value;
  if (realAddress <= 0x1FFF) {
    memoryBuffer[realAddress] = new_value;
  } else if (realAddress <= 0x3FFF) {
    ppu.writeCPU(realAddress, new_value);
  } else if (realAddress == 0x4014) {
    ppu.copyOAM(&memoryBuffer[new_value << 8]);
  } else if (realAddress >= 0x4016 && realAddress <= 0x4017) {
    controller[realAddress & 1] = window.getController(realAddress & 1);
  }
}

uint16_t Memory::getRealMemoryAddress(uint16_t address)
{
  if (address <= 0x1FFF) {
    address %= 0x0800;
  } else if (address <= 0x3FFF) {
    address = (address & 7) + 0x2000;
  }

  return address;
}

}

