#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "graphics/LinkDown.h"
#include "graphics/LinkUp.h"
#include "graphics/LinkRight.h"
#include "graphics/palettes.h"

#define LINK_SPEED 5

extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;

extern const int16_t directions[9][2];

uint8_t linkDirection = 0;
uint16_t linkX, linkY;

uint8_t flipLink = FALSE;

// Keep track of which metasprite to use for link
metasprite_t const *linkMetasprite;

void SetupLink()
{
    // set the down tiles in
    set_sprite_data(0, LinkDown_TILE_COUNT, LinkDown_tiles);

    // Position near the top middle
    // Scale the position, since we are using scaled integers
    linkX = 80 << 4;
    linkY = 40 << 4;

    // Start by facing down
    linkDirection = J_DOWN;

    // Start with the down metasprite
    linkMetasprite = LinkDown_metasprites[0];
}

uint8_t UpdateLink()
{

    // Save our last direction
    // So we can keep track of directional changes
    uint8_t linkLastDirection = linkDirection;
    uint8_t linkMoving = FALSE;
    linkDirection = linkDirection;

    // check if the right joypad button is pressed
    if (joypadCurrent & J_RIGHT)
    {
        linkX += LINK_SPEED;
        linkDirection = J_RIGHT;
        linkMoving = TRUE;
    }

    // check if the left joypad button is pressed
    if (joypadCurrent & J_LEFT)
    {
        linkX -= LINK_SPEED;
        linkDirection = J_LEFT;
        linkMoving = TRUE;
    }

    // check if the down joypad button is pressed
    if (joypadCurrent & J_DOWN)
    {
        linkY += LINK_SPEED;
        linkDirection = J_DOWN;
        linkMoving = TRUE;
    }

    // check if the up joypad button is pressed
    if (joypadCurrent & J_UP)
    {
        linkY -= LINK_SPEED;
        linkDirection = J_UP;
        linkMoving = TRUE;
    }

    // If link is moving
    if (linkMoving)
    {

        // If we changed direction
        if (linkDirection != linkLastDirection)
        {

            // Set the sprite data for the new side we are facing
            // We do not have 'left' tiles, we'll use the 'right' tiles and flip them
            switch (linkDirection)
            {
            case J_DOWN:
                set_sprite_data(0, LinkDown_TILE_COUNT, LinkDown_tiles);
                break;
            case J_RIGHT:
                set_sprite_data(0, LinkRight_TILE_COUNT, LinkRight_tiles);
                break;
            case J_LEFT:
                set_sprite_data(0, LinkRight_TILE_COUNT, LinkRight_tiles);
                break;
            case J_UP:
                set_sprite_data(0, LinkUp_TILE_COUNT, LinkUp_tiles);
                break;
            }
        }

        // Change the metasprite link uses
        // We don't have 'left' metasprites. So we'll use the right metasprites and flip
        switch (linkDirection)
        {
        case J_DOWN:
            linkMetasprite = LinkDown_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;
        case J_RIGHT:
            linkMetasprite = LinkRight_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;
        case J_LEFT:
            linkMetasprite = LinkRight_metasprites[twoFrameRealValue];
            flipLink = TRUE;
            break;
        case J_UP:
            linkMetasprite = LinkUp_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;
        }
    }

    // flip along the vertical access if neccessary
    // Draw link at the non-scaled position
    // Return how many sprites were used
    if (flipLink)
        return move_metasprite_vflip(linkMetasprite, 0, 0, linkX >> 4, linkY >> 4);
    else
        return move_metasprite(linkMetasprite, 0, 0, linkX >> 4, linkY >> 4);
}