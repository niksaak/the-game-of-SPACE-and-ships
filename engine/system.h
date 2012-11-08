#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

extern void crash();

extern void errorprint_SDL();

extern void check_malloc(const void* ptr, const char* errorstring, bool crash);

extern void deinit();
