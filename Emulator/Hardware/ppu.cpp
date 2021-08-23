// Main file header
#include "ppu.hpp"
#include "window.hpp"

#include <algorithm>

using namespace std;

namespace Hardware {

uint8_t byteFlip(uint8_t byte){
    uint8_t flipped = 0;
    for (size_t i = 0; i < 7; i++) {
        flipped |= (1 & byte);
        flipped <<= 1;
        byte >>= 1;
    }
    flipped |= (1 & byte);

    return flipped;
}

PPU::PPU(Cartridge& cart, Window& window, CPU& cpu): nametable(), 
patterntable(), 
palette(),
oam(),
spritesToDraw(),
cycles(0),
scanline(0),
cart(cart),
window(window),
cpu(cpu)
{
     // Read pattern tables (0, 1)
    for (size_t i = 0; i < cart.chrMemory.size(); i++){
        patterntable[i/4096][i%4096] = cart.chrMemory[i];
    }

    // Horizontal flip (2 = 0 fliped, 3 = 1 flipped)
    for (size_t i = 0; i < 4096; i++){
        patterntable[2][i] = byteFlip(patterntable[0][i]);
        patterntable[3][i] = byteFlip(patterntable[1][i]);
    }
    
    //  Vertical flip (4 = 0 fliped, 5 = 1 flipped)
    for (size_t i = 0; i < 4096; i++){
        patterntable[4][i] = patterntable[0][(i/8 + 1)*8 -(i%8) -1];
        patterntable[5][i] = patterntable[1][(i/8 + 1)*8 -(i%8) -1];
    }


    //  Double flip (6 = 2 fliped vert, 7 = 3 flipped vert)
    for (size_t i = 0; i < 4096; i++){
        patterntable[6][i] = patterntable[2][(i/8 + 1)*8 -(i%8) -1];
        patterntable[7][i] = patterntable[3][(i/8 + 1)*8 -(i%8) -1];
    }
    
    for (size_t i = 0; i < 8; i++)
    {
        spritesToDraw[i] =-1;
    }
    
    // control.patternBackground = 0;
    // nametable[0][4] = 2;
    // palette[1] = 17;
    // palette[2] = 18;
    // palette[3] = 20;
    
}

uint8_t PPU::readPPU(uint16_t address){
    // Palette
    if(address >= 0x3F00){
        if(!(address - 0x3f00)%4){
            return palette[0];
        }
        return palette[address - 0x3f00];
    }
    // Nametable
    else if(address >= 0x2000){
        auto nametable_num = (address - 0x2000)/1024;
        auto nametable_offset = (address - 0x2000)%1024;

        // Vertical mirroring
        if(cart.iNesHeader[6] & 1){
            return nametable[nametable_num/2][nametable_offset];
        }
        // Horizontal mirroring
        else
        {
            return nametable[nametable_num%2][nametable_offset];
        }
        
    }
    // Patterntable
    else{
        return patterntable[address/0x1000][address%0x1000];
    }
}


void PPU::writePPU(uint16_t address, uint8_t data){
    // Palette
    if(address >= 0x3F00){
        if(!(address - 0x3f00) % 4){
            palette[0] = data;
        }
        palette[address - 0x3f00] = data;
    }
    // Nametable
    else if(address >= 0x2000){
        auto nametable_num = (address - 0x2000)/1024;

        // Vertical mirroring
        if(cart.iNesHeader[6] & 1){
            nametable[nametable_num/2][address%1024] = data;
        }
        // Horizontal mirroring
        else
        {
            nametable[nametable_num%2][address%1024] = data;
        }
        
    }
    // Patterntable
    else{
        patterntable[address/0x1000][address%0x1000] = data;
    }
}

void PPU::cycle(){
    if (scanline < 240) {
        if(cycles < 256){
            auto pixel = getPixel(cycles,scanline);
            window.setPixel(pixel, cycles, scanline);
        }
        else if(cycles == 257){
            if(mask.renderSprites){
                // Evaluate sprites for next scanline
                spriteEvaluation();
            }
        }
        else{
            // TODO: verify what this cycles do 
            // nothing
        }
    } else if (scanline == 241) {
        if (cycles == 1) {
            window.showFrame();
            status.verticalBlank = 1;
            if(control.enableNmi){
                cpu.interruption();
            }
        }
    }

    cycles= (cycles+1)%341;
    if(cycles == 0){
        scanline = (scanline+1)%262;
    }
}    

uint8_t PPU::readCPU(uint16_t address) {
    address &= 7;
    uint8_t data = 0x00;

    //Nem todos Registradores d PPU devem ser lidos, 
    //portanto não há ações a serem feitas. São eles:
    // 0x0000 - Registrador de controle
    // 0x0001 - Registrador de Mask
    // 0x0003 - Registrador de endereço 
    // 0x0005 - Registrador de scroll 
    // 0x0006 - Registrador de endereço PPU

    //Seguem os demais com ações de leitura:
    switch (address) {

        // Registrador de Status
        case 0x0002:

            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.verticalBlank = 0;
            latch_address = 0;
            break;

        // Dados OAM
        case 0x0004:

            data = oam[oam_addr];
            break;

        // Dados PPU
        case 0x0007:

            //Buffer sempre contém o dado atrasado em um ciclo por conta da operação de leitura
            // Portanto o buffer deve ser atualizado para a próxima vez
            data = ppu_data_buffer;            
            ppu_data_buffer = readPPU(vram_addr.reg);            
            if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer; 
            vram_addr.reg += control.incrementMode ? 32 : 1;
    }

    return data;
}

void PPU::writeCPU(uint16_t address, uint8_t data) {
    address &= 7;
    switch (address) {

        // Registrador de Status 0x0002 não tem operação de escrita

        //Registrador do Control
        case 0x0000: 

            control.reg = data;
            tram_addr.nametable_x = control.nametableX;
            tram_addr.nametable_y = control.nametableY;
            break;

        //Registrador Mask
        case 0x0001: 

            mask.reg = data;
            break;
        
        //Registrador Endereço OAM
        case 0x0003:

            oam_addr = data;
            break;

        //Registrador Dados OAM
        case 0x0004:

            oam[oam_addr++] = data;
            break;   

        //Registrador Scroll
        case 0x0005:

            if (latch_address)
                scroll_position.y = data;
            else
                scroll_position.x = data;
            latch_address = !latch_address;
            break;

        //Registrador Endereço PPU
        case 0x0006:

            if (!latch_address)			
			    tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
		    else {
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr = tram_addr;
		    }            
            latch_address = !latch_address;
		break;

        //Registrador Dados PPU
        case 0x0007:

            writePPU(vram_addr.reg, data);
            vram_addr.reg += control.incrementMode ? 32 : 1;
    }
}

void PPU::copyOAM(uint8_t* oam_start) {
    copy_n(oam_start, sizeof(oam), oam);
}


uint32_t PPU::getPixel(uint8_t x, uint8_t y){
    auto pixel_nes_colour = readPPU(0x3F00);

    if(mask.renderBackground){
        // TODO: account for scrolling offset 
        auto nametable_x = x / 8;
        auto nametable_y = y / 8;

        //  Access nametable
        // TODO: Do no blindly use 0x2000. Take the nametable and the offset from
        //       the configuration register
        auto tile = readPPU(0x2000 + nametable_y*32 + nametable_x); 

        // Access patterntable
        auto tile_x = x % 8;
        auto tile_y = y % 8;
        auto tgt_tile_col = &patterntable[control.patternBackground][tile * 16 + tile_y];
        auto pixel_mask =     ((tgt_tile_col[0] >> (7 - tile_x)) & 0x01) +
                        2 * ((tgt_tile_col[8] >> (7 - tile_x)) & 0x01);

        // Access attributetable
        auto attributetable_index =  (nametable_y/4)*8 + (nametable_x/4);
        // TODO: Do no blindly use 0x2000. Take the nametable and the offset from
        //       the configuration register
        auto attribute_byte_pixel = readPPU(0x2000 + 0x3c0 + attributetable_index);

        union{
            struct{
                uint8_t top_left : 2;
                uint8_t top_right : 2;
                uint8_t bottom_left : 2;
                uint8_t bottom_right : 2;
            };
            uint8_t attribute_byte;
        } attribute_byte;

        uint8_t palette = 0;
        attribute_byte.attribute_byte = attribute_byte_pixel;
        if(nametable_y % 4 < 2){
            if(nametable_x % 4 < 2){
                palette = attribute_byte.top_left;
            }
            else{
                palette = attribute_byte.top_right;
            }
        }
        else{
            if(nametable_x % 4 < 2){
                palette = attribute_byte.bottom_left;
            }
            else{
                palette = attribute_byte.bottom_right;
            }

        }
        
        pixel_nes_colour = readPPU(0x3f00 + pixel_mask + 4 * palette);
    }    
    if (mask.renderSprites)    {
        bool set_sprite_pixel = false;
        auto i = 0;
        // Check list of sprites until end or until pixel is set to 
        // non-transparent colour.
        while( i<8 && spritesToDraw[i] > 0 && !set_sprite_pixel){
            // Get number of sprite in OAM
            auto oam_sprite = spritesToDraw[i];
            auto sprite_x = oam[oam_sprite*4 + 3];
            
            if(sprite_x >= x-7 && sprite_x <= x){
                // Get tile number
                auto sprite_tile = oam[oam_sprite*4 + 1];

                // Get tile attributes
                auto sprite_att = oam[oam_sprite*4 + 2];

                // Get pattern table of sprite with flips
                auto sprite_patterntable = control.patternSprite + 2* (sprite_att & 0x40)/64 + 4*(sprite_att & 0x80)/ 128;

                // Get pixel mask
                auto sprite_y = oam[oam_sprite*4];
                auto tile_x = x - sprite_x;
                auto tile_y = y - sprite_y;
                auto tgt_tile_col = &patterntable[sprite_patterntable][sprite_tile* 16 + tile_y];
                auto pixel_mask =     ((tgt_tile_col[0] >> (7 - tile_x)) & 0x01) +
                                2 * ((tgt_tile_col[8] >> (7 - tile_x)) & 0x01);

                // Background disabled -> render sprite
                // Background is transparent -> render sprite
                // Background is not transparent but sprite is not transp and has priority -> render sprite
                if(!mask.renderBackground
                    || (pixel_nes_colour == readPPU(0x3F00)
                    || pixel_mask != 0 && !(sprite_att & 0x20))){         
                    pixel_nes_colour = readPPU(0x3f10 + pixel_mask + 4 * (sprite_att & 0x03));
                }

                // If sprite was not transparent, we dont need to look at next sprite
                set_sprite_pixel = pixel_mask;
            }
            i++;
        }
    }
    return nesToRGB[pixel_nes_colour];
}

void PPU::spriteEvaluation() {
    int spritesCount = 0;
    bool spritesOverflow = false;
    int i;

    for(i = 0; i < 8; i++) {
        spritesToDraw[i] = -1;
    }

    for(i = 0; i < 64; i++) {
        // Reading one OAM entry (a sprite)
        uint8_t y = oam[i * 4];
        uint8_t tile_id = oam[i * 4 + 1];
        // Attributes contain: priority of the sprite, palette that it's using and the orientation
        uint8_t attributes = oam[i * 4 + 2];
        uint8_t x = oam[i * 4 + 3];

        // Checking if the result lies between 0 and the height of the sprite
        // If it does, then the sprite should be rendered
        // (Converting to signed numbers, because row must be a positive number so
        // we know the sprite is at the same height as the scanline)
        uint16_t row = (uint16_t)(scanline+1) - (uint16_t)y;
        if(row >= 0 && row < 8) {
            // Copying sprite to the next open slot in secondary OAM (unless 8 sprites have been found, in which case the write is ignored).
            // Salvando a posição do sprite
            spritesToDraw[spritesCount] = i;
            spritesCount++;
        }
    }

    if(spritesCount > 8) {
        spritesOverflow = true;
    }
}

}
