#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "graphics/palettes.h"
#include "moblin.h"
#include "link.h"

// This is an easy way to determine a direction using one of our joypad constants
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

uint8_t joypadCurrent=0;

uint16_t twoFrameCounter= 0;
uint8_t twoFrameRealValue=0;


/**
 * @brief Our moblin and link has two frame walk animation. They'll share a common variable that determines which frame to show
 * when they are walking. 
 */
void UpdateTwoFrameCounter(){
    twoFrameCounter+=3;
    twoFrameRealValue = twoFrameCounter>>4;

    // Stop & reset if the vlaue is over 2
    if(twoFrameRealValue>=2){
        twoFrameRealValue=0;
        twoFrameCounter=0;
    }
}

void main(void)
{
    // Turn on sprites
    // Enable the 8x16 (tall) sprites mode
    SHOW_SPRITES;
    SPRITES_8x16;
    DISPLAY_ON;

   // Set our color palettes into vram
   set_sprite_palette(0,palettes_PALETTE_COUNT,palettes_palettes);

    // Setup link & the moblin
   SetupLink();
   SetupMoblin();

    // Loop forever
    while(1) {

        // Save the current state of the joypad
        // it's important NOT to call the joypad function more than once
        joypadCurrent=joypad();

        UpdateTwoFrameCounter();

        uint8_t lastSprite=0;
        
        lastSprite += UpdateLink();
        lastSprite += UpdateMoblin(lastSprite);

        // Hide any extra sprites
        // This might not be as useful in this demo
        // But this will be helpful for actual games
        // Without this, extra "leftover" sprites may weidly linger around.
        hide_sprites_range(lastSprite,40);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}
