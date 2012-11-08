#include <stdlib.h>
#include <stdio.h>
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

bool check_malloc(const void* ptr, const char* errorstring, bool crash_on_fail)
{
  if(ptr == NULL) {
    fprintf(stderr, errorstring);
    if(crash_on_fail) {
      crash();
    } else {
      return false;
    }
  } else {
    return true;
  }
  
  return false;
}

void deinit() {
  printf("Deinitializing SDL: ");
  SDL_Quit();
  printf("OK.\n");
}
