
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "graphics/Moblin.h"
#include "graphics/palettes.h"

#define MOBLIN_SPEED 4
#define LINK_PADDED_TILE_COUNT 12
extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;

extern const int16_t directions[9][2];

uint8_t moblinDirection = 0;
uint16_t moblinX, moblinY;
uint16_t moblinCounter = 200;

metasprite_t const *moblinMetasprite=0;

void SetupMoblin()
{
    // It's not clear in this tutorial, but:
    // We Take into consideration that some of links directions/sprites having different lengths
    // We'll "allocate" (not really in terms of memory, just in terms of organization) a set number of tiles for link
    // This way, we avoid links tiles overriding the moblins tiles
    set_sprite_data(LINK_PADDED_TILE_COUNT, Moblin_TILE_COUNT, Moblin_tiles);

    moblinX = 80 << 4;
    moblinY = 100 << 4;
    moblinDirection = J_UP;

    moblinMetasprite=Moblin_metasprites[0];
}

uint8_t UpdateMoblin(uint8_t lastSprite)
{

    uint8_t flip = FALSE;
    uint8_t moblinMoving = FALSE;

    // If we are about to reach zero
    if (moblinCounter <= 3)
    {

        // Reset our counter
        moblinCounter = 400;

        // Pick a new direction
        // use the DIV register to get a pseduo random value
        const uint8_t directionsOnly[] = {J_LEFT, J_RIGHT, J_DOWN, J_UP};
        moblinDirection = directionsOnly[DIV_REG % 4];
    }
    else
    {

        // Decrease our counter
        moblinCounter -= 3;
    }

    // The moblin is moving when the coutner is above 256
    if (moblinCounter > 256)
    {

        moblinMoving = TRUE;

        // Change his x and y positio based on the direction he's moving in
        moblinX += directions[moblinDirection][0] * MOBLIN_SPEED;
        moblinY += directions[moblinDirection][1] * MOBLIN_SPEED;


        // use frame 0 when the moblin isn't moving.
        uint8_t moblinFrame = moblinMoving ? twoFrameRealValue : 0;

        // use the proper metasprites for the moblin
        // Each direction has two metasprites
        // For the left direction, we'll just flip the right metasprite
        switch (moblinDirection)
        {
        case J_DOWN:
            moblinMetasprite = Moblin_metasprites[moblinFrame];
            break;
        case J_UP:
            moblinMetasprite = Moblin_metasprites[2 + moblinFrame];
            break;
        case J_RIGHT:
            moblinMetasprite = Moblin_metasprites[4 + moblinFrame];
            break;
        case J_LEFT:
            moblinMetasprite = Moblin_metasprites[6 + moblinFrame];
            break;
        }
    }

    // Our base tile isn't 0, that's where link's tiles start
    // We'll use the tiles after links as our base tiles
    // Draw the metasprite, and return how many sprites were used
      return move_metasprite(moblinMetasprite, LINK_PADDED_TILE_COUNT, lastSprite, moblinX >> 4, moblinY >> 4);
}