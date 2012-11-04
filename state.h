#pragma once

#include <stdbool.h>

typedef struct StateMan {
  int* scrw;
  int* scrh;
  bool running;
  struct StateCons* states;
} StateMan;

typedef struct StateCons {
  struct State* this;
  struct StateCons* parent;
} StateCons;

typedef struct DataCons {
  void* first;
  struct DataCons* rest;
}

typedef struct State {
  DataCons* data;
  int (*init)(State*);
  int (*deinit)(State*);
  int (*redraw)(struct State*);
  int (*keydown)(struct State*, SDL_Event*);
  int (*keyup)(struct State*, SDL_Event*);
  struct StateCons* invocables;
} State;
