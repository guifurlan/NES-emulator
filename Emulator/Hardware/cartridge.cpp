#include "cartridge.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Hardware
{
    
Cartridge::Cartridge(std::string cartridgeFilename) :
  iNesHeader(),
  trainerMemory(),
  prgMemory(),
  chrMemory(),
  miscellaneousMemory()
{
  // Open binary file
  ifstream binaryFile(cartridgeFilename, ios::in | ios::binary);
  if (!binaryFile.good()) {
    cerr << "Could not open the binary file" << endl;
    exit(-1);
  }

  // Read header data
  binaryFile.read(iNesHeader.data(), 16);
  // TODO: Check for ines signature
  if (string_view(iNesHeader.data(), 4) != "NES\x1A") {
    cerr << "The binary file is not iNes compatible" << endl;
    exit(-1);
  }

  // Read trainer data if it is enabled
  if (iNesHeader[6] & 0x04) {
    binaryFile.read(trainerMemory.data(), trainerMemory.size());
  }

  // Read program data
  const auto prgSectionSize = [&] {
    auto lowByte    = iNesHeader[4];
    auto highNibble = iNesHeader[9] & 0x0F;

    if (highNibble == 0xF) {
      const auto multiplier =  lowByte &  0x03;
      const auto exponent   = (lowByte & ~0x03) >> 2;
      return (1 << exponent) * (2 * multiplier + 1);
    }
    else {
      return ((highNibble << 8) | lowByte) * 0x4000;
    }
  }();

  prgMemory.resize(prgSectionSize);
  binaryFile.read(prgMemory.data(), prgSectionSize);

  // Read sprite data
  const auto chrSectionSize = [&] {
    auto lowByte    =  iNesHeader[5];
    auto highNibble = (iNesHeader[9] & 0xF0) >> 4;

    if (highNibble == 0xF) {
      const auto multiplier =  lowByte &  0x03;
      const auto exponent   = (lowByte & ~0x03) >> 2;
      return (1 << exponent) * (2 * multiplier + 1);
    }
    else {
      return ((highNibble << 8) | lowByte) * 0x2000;
    }
  }();

  chrMemory.resize(chrSectionSize);
  binaryFile.read(chrMemory.data(), chrSectionSize);

  // Read miscellaneous data
  while (!binaryFile.eof()) {
    char miscData;
    binaryFile.get(miscData);
    miscellaneousMemory.push_back(miscData);
  }
}

} // namespace Hardware