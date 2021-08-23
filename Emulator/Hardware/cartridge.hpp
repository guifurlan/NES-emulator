#ifndef HEADER_CARTRIDGE_HPP
#define HEADER_CARTRIDGE_HPP

#include <array>
#include <vector>
#include <string>

namespace Hardware
{
    
struct Cartridge {
  // Header
  std::array<char, 16> iNesHeader;

  // Memory
  std::array<char, 256> trainerMemory;
  std::vector<char> prgMemory;
  std::vector<char> chrMemory;
  std::vector<char> miscellaneousMemory;

  Cartridge(std::string cartridgeFilename);
};

} // namespace Hardware

#endif // HEADER_CARTRIDGE_HPP