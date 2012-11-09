#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

extern void crash();

extern void errorprint_SDL();

extern bool check_malloc(const void* ptr, const char* errorstring,
                         bool crash_on_fail);
