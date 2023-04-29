#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "common.h"


// This is an easy way to determine a direction for a object with two frames per direction
const Vector8 directionsForTwoFrameObjects[7]={
    {0,1}, // Down
    {0,0}, 
    {0,-1}, // Up,
    {0,0},
    {1,0},// Right
    {0,0},
    {-1,0},// Left
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
