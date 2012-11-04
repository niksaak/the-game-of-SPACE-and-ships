#pragma once

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

extern const int screenW;
extern const int screenH;
extern const int screenBPP;
extern const Uint32 defaultColor;

extern SDL_Surface* screen;
extern bool running;

typedef struct {
  Sint16 x;
  Sint16 y;
} SDL_Point;

typedef struct {
  cpBody* body;
  cpShape* shape;
  cpFloat accel;
  cpFloat turn;
} Ship;
