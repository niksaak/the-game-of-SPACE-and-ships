#pragma once

#include <stdbool.h>

#include <SDL/SDL.h>

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
} DataCons;

typedef int (*initf)(struct State* state);
typedef int (*deinitf)(struct State* state);
typedef int (*redrawf)(struct State* state);
typedef int (*keydownf)(struct State* state, SDL_Event* event);
typedef int (*keyupf)(struct State* state, SDL_Event* event);

typedef struct State {
  struct DataCons* data;
  initf init;
  deinitf deinit;
  redrawf redraw;
  keydownf keydown;
  keyupf keyup;
  struct StateCons* invocables;
} State;

extern State* state(initf init, deinitf deinit,
                    redrawf redraw, keydownf keydown, keyupf keyup);

extern void destate(State* state);

extern StateCons* statecons(State* state, StateCons* list);

extern void statepush(State* state, StateCons* list);

extern StateCons* destatecons(StateCons* list);

extern StateCons* clear_statecons(StateCons* list);

extern StateCons* add_invocable(State* invocable, State* state);

extern void clear_invocables(State* state);

extern StateCons* invoke_state(State* state, StateMan* stateman);

extern StateCons* devoke_current_state(StateMan* stateman);

extern StateMan stateman(SDL_Surface* screen);
