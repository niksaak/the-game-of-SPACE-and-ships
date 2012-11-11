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

bool check_malloc(const void* ptr, const char* errstr, bool crash_on_fail)
{
  if(ptr == NULL) {
    fprintf(stderr, errstr);
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

void* malloc_or_die(size_t size, const char* errstr)
{
  void* ptr = malloc(size);

  check_malloc(ptr, errstr, true);

  return ptr;
}
