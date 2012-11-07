#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define RENDERER_TYPE_NIL 0
#define RENDERER_TYPE_SDL 1
#define RENDERER_TYPE_OGL 2

typedef struct StateMan {
  bool running;
  SDL_Window* window;
  struct {
    Uint8 type;
    union {
      SDL_Renderer* sdl_renderer;
      SDL_GLContext* glcontext;
    };
  } renderer;
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

typedef void (*initf)(struct State*);
typedef void (*deinitf)(struct State*);
typedef void (*redrawf)(struct State*);
typedef void (*idlef)(struct State*);
typedef void (*keydownf)(struct State*, SDL_Event*);
typedef void (*keyupf)(struct State*, SDL_Event*);

typedef struct State {
  struct DataCons* data;
  struct StateCons* invocables;
  initf init;
  deinitf deinit;
  redrawf redraw;
  idlef idle;
  keydownf keydown;
  keyupf keyup;
  bool devoke;
} State;

// State basic constructor
extern State* state(initf init, deinitf deinit,
                    redrawf redraw, keydownf keydown, keyupf keyup);

extern void destate(State* state);

// STATE CONS operations
extern StateCons* statecons(State* state, StateCons* list);

extern void statepush(State* state, StateCons** list);

extern StateCons* destatecons(StateCons* list);

extern void statepop(StateCons** list);

extern StateCons* clear_statecons(StateCons* list);

// DATA CONS operations
extern DataCons* datacons(void* data, DataCons* list);

extern void datapush(void* data, DataCons** list);

extern DataCons* dedatacons(DataCons* list);

extern void datapop(DataCons** list);

// State invokes
extern StateCons* add_invocable(State* invocable, State* state);

extern void clear_invocables(State* state);

extern StateCons* invoke_state(State* state, StateMan* stateman);

extern StateCons* devoke_state(State* state);

// State manager basic constructor
extern StateMan stateman(SDL_Surface* screen);
