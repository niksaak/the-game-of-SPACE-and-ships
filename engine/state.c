#include <stdlib.h>
#include <assert.h>

#include "state.h"

State* state(initf init, deinitf deinit,
             redrawf redraw, keydownf keydown, keyupf keyup)
{
  State* state = (State*)malloc(sizeof(State));

  state->data = NULL;
  state->init = init;
  state->deinit = deinit;
  state->redraw = redraw;
  state->keydown = keydown;
  state->keyup = keyup;
  state->invocables = NULL;

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

  cons->this = state;
  cons->parent = list;

  return cons;
}

void statepush(State* state, StateCons* list)
{
  assert(state != NULL);

  list = statecons(state, list);
}

StateCons* destatecons(StateCons* list)
{
  assert(list != NULL);

  StateCons* parent = list->parent;
    
  free(list);

  return parent;
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

StateCons* add_invocable(State* invocable, State* state)
{
  assert(state != NULL);
  assert(invocable != NULL);

  statepush(invocable, state->invocables);

  return state->invocables;
}

void clear_invocables(State* state)
{
  state->invocables = clear_statecons(state->invocables);
}

StateCons* invoke_state(State* state, StateMan* stateman)
{
  assert(state != NULL);
  stateman->states = statecons(state, stateman->states);
  if(stateman->states->this->init != NULL) {
    stateman->states->this->init(state);
  }
  
  return stateman->states;
}

StateCons* devoke_current_state(StateMan* stateman)
{
  assert(stateman != NULL);
  assert(stateman->states != NULL);

  if(stateman->states->this->deinit) {
    stateman->states->this->deinit(stateman->states->this);
  }
  stateman->states = destatecons(stateman->states);

  return stateman->states;
}

StateMan stateman(SDL_Surface* screen)
{
  StateMan stm;

  stm.scrw = &screen->w;
  stm.scrh = &screen->h;
  stm.running = true;
  stm.states = NULL;

  return stm;
}
