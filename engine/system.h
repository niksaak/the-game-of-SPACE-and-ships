#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

extern void crash();

extern void errorprint_SDL();

extern bool check_malloc(const void* ptr, const char* errstr,
                         bool crash_on_fail);

extern void* malloc_or_die(size_t size, const char* errstr);
