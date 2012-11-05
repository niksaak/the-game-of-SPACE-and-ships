#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <chipmunk/chipmunk.h>

#include "engine/state.h"
#include "main.h"

const int screenW = 512;
const int screenH = 512;
const int screenBPP = 32;
const Uint32 DefaultColor = 0xFFFFFFFF;

SDL_Surface* screen = NULL;
bool running = false;

//// System /////

void crash()
{
  fprintf(stderr, "\nCrashing mindlessly...\n");
  exit(1);
}

void init()
{
  printf("\nInitializing SDL: ");
  if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "\nError while initializing SDL: %s\n", SDL_GetError());
    crash();
  }
  printf("OK.\n");

  printf("Setting up screen: ");
  screen = SDL_SetVideoMode(screenW, screenH, screenBPP,
                            SDL_SWSURFACE | SDL_ANYFORMAT | SDL_HWPALETTE);
  if(screen == NULL) {
    fprintf(stderr, "Error while setting up screen: %s\n", SDL_GetError());
    crash();
  }
  SDL_SetClipRect(screen, NULL);
  printf("OK (%ix%i, %ibpp).\n",
         screen->w, screen->h, screen->format->BitsPerPixel);

  SDL_WM_SetCaption("gemu", "gemu");
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                      SDL_DEFAULT_REPEAT_INTERVAL);
  printf("Init OK\n");
}

void deinit()
{
  printf("Deinitializing SDL: ");
  SDL_Quit();
  printf("OK\n");
}

//// Helpers ////

static SDL_Point point(Uint16 x, Uint16 y)
{
  static SDL_Point point;
  point.x = x;
  point.y = y;
  return point;
}

static SDL_Point toSDLxy(cpVect cpv, const SDL_Surface* surface)
{
  static SDL_Point point;
  point.x = cpv.x + surface->w / 2;
  point.y = -cpv.y + surface->h / 2;

  if(point.x < 0 || point.x >= surface->w) {
    point.x = 0;
  }
  if(point.y < 0 || point.y >= surface->h) {
    point.y = 0;
  }

  return point;
}

cpFloat toDegrees(cpFloat radians)
{
  return radians * (180 / M_PI);
}

//// Ship ////

Ship* cpSpaceAddShip(cpFloat length, cpFloat width, cpFloat mass,
		     cpFloat accel, cpFloat turn, cpSpace* space)
{
  cpVect verts[3];
  verts[0] = cpv(length / 2, 0);
  verts[1] = cpv(-(length / 2), width / 2);
  verts[2] = cpv(-(length / 2), -(width / 2));

  cpBody* body = cpBodyNew(mass, cpMomentForPoly(mass, 3, verts, cpvzero));
  cpSpaceAddBody(space, body);

  cpShape* shape = cpPolyShapeNew(body, 3, verts, cpvzero);
  cpSpaceAddShape(space, shape);

  Ship* ship = (Ship*)malloc(sizeof(Ship));
  ship->body = body;
  ship->shape = shape;
  ship->accel = accel;
  ship->turn = turn;

  return ship;
}

//// Drawing ////

int drawShip(Ship* ship, SDL_Surface* surface)
{
  int i;
  SDL_Point pts[3];
  for(i = 0; i < 3; i++) {
    pts[i] = toSDLxy(cpBodyLocal2World(ship->body,
				       cpPolyShapeGetVert(ship->shape, i)),
		     surface);
  }

  return aatrigonColor(surface,
                       pts[0].x, pts[0].y,
                       pts[1].x, pts[1].y,
                       pts[2].x, pts[2].y,
                       DefaultColor);
}

//// Main /////

int main(int argc, char** argv)
{
  init();
  running = true;

  deinit();
  return 0;
}
