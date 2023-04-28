
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "graphics/Moblin.h"
#include "graphics/palettes.h"

#define MOBLIN_SPEED 4
extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;

extern const int16_t directions[9][2];

uint8_t moblinDirection = 0;
uint16_t moblinX, moblinY;
uint16_t moblinCounter = 200;

void SetupMoblin()
{

    set_sprite_data(8, Moblin_TILE_COUNT, Moblin_tiles);

    moblinX = 80 << 4;
    moblinY = 100 << 4;
    moblinDirection = J_UP;
}

uint8_t UpdateMoblin(uint8_t lastSprite)
{

    metasprite_t const *moblinMetasprite;

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
    }

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
    case J_RIGHT:
        moblinMetasprite = Moblin_metasprites[4 + moblinFrame];
        break;
    case J_LEFT:
        moblinMetasprite = Moblin_metasprites[4 + moblinFrame];
        flip = TRUE;
        break;
    case J_UP:
        moblinMetasprite = Moblin_metasprites[2 + moblinFrame];
        break;
    }

    // Draw the metasprite, and return how many sprites were used
    // use the vflip (flips along the vertical axis) if we are drawing the left sprite
    if (flip)
        return move_metasprite_vflip(moblinMetasprite, 8, lastSprite, moblinX >> 4, moblinY >> 4);
    else
        return move_metasprite(moblinMetasprite, 8, lastSprite, moblinX >> 4, moblinY >> 4);
}