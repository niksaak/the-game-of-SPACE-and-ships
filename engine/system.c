#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include "system.h"
#include "state.h"

void crash()
{
  fprintf(stderr, "\nCrashing mindlessly...\n");
  exit(1);
}

void errorprint_SDL()
{
  fprintf(stderr, "ERROR: %s\n", SDL_GetError());
}

void deinit() {
  printf("Deinitializing SDL: ");
  SDL_Quit();
  printf("OK.\n");
}
