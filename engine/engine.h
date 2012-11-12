#pragma once

#include "system.h"
#include "state.h"

extern void init(struct StateMan* stateman,
                 const char* title, int width, int height, bool ogl);

extern void deinit();
