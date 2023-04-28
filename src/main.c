#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "graphics/palettes.h"

const int16_t directions[9][2] ={
    {0,0},
    
    {1,0},// Right
    {-1,0},// Left
    {0,0},
    {0,-1},// Up
    {0,0},
    {0,0},
    {0,0},
    {0,1},//Down

};

uint16_t scrollValue=0;

uint8_t joypadCurrent=0,joypadPrevious=0;

uint16_t twoFrameCounter= 0;
uint8_t twoFrameRealValue=0;


uint8_t UpdateLink();
void SetupLink();
uint8_t UpdateMoblin(uint8_t lastSprite);
void SetupMoblin();

void main(void)
{
    SHOW_SPRITES;
    SPRITES_8x16;
    DISPLAY_ON;

   set_sprite_palette(0,palettes_PALETTE_COUNT,palettes_palettes);

   SetupLink();
   SetupMoblin();

    // Loop forever
    while(1) {

        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();

        twoFrameCounter+=3;

        twoFrameRealValue = twoFrameCounter>>4;
        if(twoFrameRealValue>=2){
            twoFrameRealValue=0;
            twoFrameCounter=0;
        }

        uint8_t lastSprite = UpdateLink();
        lastSprite+=UpdateMoblin(lastSprite);

        hide_sprites_range(lastSprite,40);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}
