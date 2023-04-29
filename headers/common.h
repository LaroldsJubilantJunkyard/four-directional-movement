#ifndef COMMON_HEADER

#define COMMON_HEADER

typedef struct Vector8{
    int8_t x;
    int8_t y;
} Vector8;

extern const Vector8 directionsForTwoFrameObjects[7];

extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;
#endif