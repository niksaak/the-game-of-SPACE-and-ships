#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include "state.h"
#include "system.h"

State* state(initf init, deinitf deinit, redrawf redraw,
             idlef idle, keydownf keydown, keyupf keyup)
{
  State* state = (State*)malloc(sizeof(State));
  check_malloc(state, "ERROR: state allocation failed.", true);
  
  state->data = NULL;
  state->invocables = NULL;
  state->init = init;
  state->deinit = deinit;
  state->redraw = redraw;
  state->idle = idle;
  state->keydown = keydown;
  state->keyup = keyup;
  state->devoke = false;

  return state;
}

void destate(State* state)
{
  assert(state != NULL);
  assert(state->data == NULL);

  free(state);
}

StateCons* statecons(State* state, StateCons* list)
{
  assert(state != NULL);

  StateCons* cons = (StateCons*)malloc(sizeof(StateCons));
  check_malloc(cons, "ERROR: statecons allocation failed", true);

  cons->this = state;
  cons->parent = list;

  return cons;
}

void statepush(State* state, StateCons** list)
{
  *list = statecons(state, *list);
}

StateCons* destatecons(StateCons* list)
{
  assert(list != NULL);

  StateCons* parent = list->parent;
    
  free(list);

  return parent;
}

void statepop(StateCons** list)
{
  *list = destatecons(*list);
}

DataCons* datacons(void* data, DataCons* list)
{
  assert(data != NULL);

  DataCons* cons = (DataCons*)malloc(sizeof(DataCons));
  check_malloc(cons, "ERROR: datacons allocation failed.", true);

  cons->first = data;
  cons->rest = list;

  return cons;
}

void datapush(void* data, DataCons** list)
{
  *list = datacons(data, *list);
}

DataCons* dedatacons(DataCons* list)
{
  assert(list != NULL);

  DataCons* rest = list->rest;

  free(list);

  return rest;
}

void datapop(DataCons** list)
{
  *list = dedatacons(*list);
}

StateCons* clear_statecons(StateCons* list)
{
  if(list != NULL) {
    if(list->parent != NULL) {
      return clear_statecons(destatecons(list));
    } else {
      return destatecons(list);
    }
  }
  return list;
}

StateCons* add_invocable_state(State* invocable, State* state)
{
  assert(state != NULL);
  assert(invocable != NULL);

  statepush(invocable, &state->invocables);

  return state->invocables;
}

void clear_invocables(State* state)
{
  state->invocables = clear_statecons(state->invocables);
}

void do_state(StateMan* stateman)
{
  assert(stateman != NULL);
  assert(stateman->states != NULL);
  assert(stateman->states->this != NULL);

  static SDL_Event event;
  State* stt = stateman->states->this;

  if(stt->init != NULL) {
    stt->init(stt);
  }

  while(!stt->devoke) {
    if(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYDOWN:
        if(stt->keydown != NULL) {
          stt->keydown(stt, &event);
        }
        break;
      case SDL_KEYUP:
        if(stt->keyup != NULL) {
          stt->keyup(stt, &event);
        }
        break;
      default:
        break;
      }
    } else {
      if(stt->idle != NULL) {
        stt->idle(stt);
      }
    }
    if(stt->redraw != NULL) {
      stt->redraw(stt);
    } else {
      fprintf(stderr, "\nERROR: state have no drawing function.\n");
      crash();
    }

    SDL_Delay(16); // TODO normal framerate thingy
  }

  if(stt->deinit != NULL) {
    stt->deinit(stt);
  }

  stt->devoke = false;
}

void invoke_state(State* state, StateMan* stateman)
{
  assert(state != NULL);
  assert(stateman != NULL);

  statepush(state, &stateman->states);
  do_state(stateman);
  statepop(&stateman->states);
}

void devoke_state(State* state)
{
  state->devoke = true;
}

StateMan stateman(SDL_Surface* screen)
{
  StateMan stm;

  stm.running = false;
  stm.window = NULL;
  stm.renderer.type = RENDERER_TYPE_NIL;
  stm.renderer.sdl_renderer = NULL;
  stm.renderer.glcontext = NULL;
  stm.states = NULL;

  return stm;
}
