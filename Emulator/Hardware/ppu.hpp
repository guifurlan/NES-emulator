#ifndef HEADER_PPU_HPP
#define HEADER_PPU_HPP

// C headers
#include <cstdint>

// Cpp headers
#include <array>
#include <chrono>
#include <string>
#include <bitset>

// Project headers
#include "memory.hpp"
#include "instructions.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"

namespace Hardware {

    // Forward declaration
    class Window;

    constexpr uint32_t nesToRGB[64] = {
        0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC,
        0x940084, 0xA80020, 0xA81000, 0x881400,
        0x503000, 0x007800, 0x006800, 0x005800,
        0x004058, 0x000000, 0x000000, 0x000000,
        0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC,
        0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
        0xAC7C00, 0x00B800, 0x00A800, 0x00A844,
        0x008888, 0x000000, 0x000000, 0x000000,
        0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8,
        0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
        0xF8B800, 0xB8F818, 0x58D854, 0x58F898,
        0x00E8D8, 0x787878, 0x000000, 0x000000,
        0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8,
        0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
        0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8,
        0x00FCFC, 0xF8D8F8, 0x000000, 0x000000,
    };


    class PPU {

        uint8_t nametable[2][1024];
        uint8_t patterntable[8][4096];
        uint8_t	palette[32];
        uint8_t oam[256];
        uint8_t spritesToDraw[8];

    int cycles;
    int scanline;

    Cartridge& cart;
    Window& window;
    CPU& cpu;

  union {
    struct {
      uint8_t unused : 5;
      uint8_t spriteOverflow : 1;
      uint8_t spriteZeroHit : 1;
      uint8_t verticalBlank : 1;
    };

    uint8_t reg;
  } status;


  union {
    struct {
      uint8_t grayscale : 1;
      uint8_t renderBackgroundLeft : 1;
      uint8_t renderSpritesLeft : 1;
      uint8_t renderBackground : 1;
      uint8_t renderSprites : 1;
      uint8_t enhanceRed : 1;
      uint8_t enhanceGreen : 1;
      uint8_t enhanceBlue : 1;
    };

    uint8_t reg;
  } mask;

  union {
    struct {
      uint8_t nametableX : 1;
      uint8_t nametableY : 1;
      uint8_t incrementMode : 1;
      uint8_t patternSprite : 1;
      uint8_t patternBackground : 1;
      uint8_t spriteSize : 1;
      uint8_t slaveMasterSelect : 1;
      uint8_t enableNmi : 1;
    };

    uint8_t reg;
  } control;

  union RenderingPositions {
    struct {

      uint16_t tile_x_offset : 5;
      uint16_t tile_y_offset : 5;
      uint16_t nametable_x : 1;
      uint16_t nametable_y : 1;
      uint16_t fine_y : 3;
      uint16_t unused : 1;
    };

    uint16_t reg = 0x0000;
  };

  struct ScrollPosition {
    uint8_t x, y;
  };

  uint8_t ppu_data_buffer = 0x00;
  RenderingPositions vram_addr;
  RenderingPositions tram_addr;
  uint8_t oam_addr = 0x00;
  bool latch_address = false;
  ScrollPosition scroll_position = {0,0};

    public:
        PPU(Cartridge& cart, Window& window, CPU& cpu);
        void cycle();
        
        // Comm with the cpu
        uint8_t readCPU(uint16_t address);
        void writeCPU(uint16_t address, uint8_t data);
        
        void copyOAM(std::uint8_t* oam_start);

    
    private:
        uint8_t readPPU(uint16_t address);
        void writePPU(uint16_t address, uint8_t data);
        uint32_t getPixel(uint8_t x, uint8_t y);
        void spriteEvaluation();
    };

}

#endif // HEADER_PPU_HPP
