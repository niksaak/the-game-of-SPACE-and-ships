#include <stdlib.h>
#include <assert.h>

#include "state.h"

State* state(int (*init)(State*),
             int (*deinit)(State*),
             int (*redraw)(State*),
             int (*keydown)(State*, SDL_Event*),
             int (*keyup)(State*, SDL_Event*))
{
  State* state = (State*)malloc(sizeof(State));

  state.data = NULL;
  state.init = init;
  state.deinit = deinit;
  state.redraw = redraw;
  state.keydown = keydown;
  state.keyup = keyup;
  state.invocables = NULL;

  return state;
}

void destate(State* state)
{
  assert(state != NULL);
  assert(state->data == NULL);

  free(state);
}

StateCons* statecons(State* state, const StateCons* list)
{
  assert(state != NULL);

  StateCons* cons = (StateCons*)malloc(sizeof(StateCons));

  cons->this = state;
  cons->parent = list;

  return cons;
}

StateCons* destatecons(StateCons* list)
{
  assert(list != NULL);

  StateCons* parent = list->parent;
  free(list);

  return parent;
}

StateCons* add_invocable(State* invocable, State* state)
{
  assert(state != NULL);
  assert(invocable != NULL);

  state->invocables = statecons(invocable, state->invocables);

  return state->invocables;
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

static StateMan stateman( SDL_Surface* screen)
{
  static StateMan stm;

  stm.scrw = &screen->w;
  stm.scrh = &screen->h;
  stm.running = true;
  stm.states = NULL;

  return stm;
}
