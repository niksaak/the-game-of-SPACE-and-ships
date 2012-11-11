#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "engine/engine.h"
#include "images.h"

//// Helpers ////

SDL_Surface* get_image(void* img, int size)
{
  SDL_Surface* pic;

  pic = SDL_LoadBMP_RW(SDL_RWFromConstMem(img, size), 1);
  
  return pic;
}

typedef struct Image {
  SDL_Texture* tex;
  SDL_Rect rect;
} Image;

static Image
texify(SDL_Surface* surface, SDL_Renderer* renderer, SDL_Window* win)
{
  static Image img;
  SDL_DisplayMode mode;
  SDL_Texture* texture;
  
  printf("creating texture from image...\n");
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  printf("done!\n");

  SDL_GetWindowDisplayMode(win, &mode);
  img.tex = texture;
  img.rect.x = mode.w / 2 - surface->w / 2;
  img.rect.y = mode.h / 2 - surface->h / 2;
  img.rect.w = surface->w;
  img.rect.h = surface->h;

  return img;
}

//// States ////

////-- state1

int st1_init(State* state)
{
  Image* img = NULL;
  SDL_Surface* surface;

  img = (Image*)malloc_or_die(sizeof(Image),
                              "Unable to alloc struct for image\n");
  printf("trying to load image...\n");
  surface = get_image(testpic_bmp, testpic_bmp_len);
  if(img == NULL) {
    printf("nyoo, can't load this cuute image /(0o0)\\\n");
    crash();
  }
  printf("loaded image \\(^o^)/\n");

  *img = texify(surface, state->stateman->renderer.sdl_renderer,
                state->stateman->window);

  datapush(img, &state->data);

  free(surface);
  return 0;
}

int st1_deinit(State* state)
{
  free(state->data->first);
  datapop(&state->data);

  return 0;
}

int st1_redraw(State* state) {
  static Image* img;
  static SDL_Renderer* renderer;

  img = (Image*)state->data->first;
  renderer = state->stateman->renderer.sdl_renderer;
  
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, img->tex, NULL, &img->rect);
  SDL_RenderPresent(renderer);

  return 0;
}

int st1_keydown(State* state, SDL_Event* event)
{
  if(event->key.keysym.sym == SDLK_c) {
    printf("\nQuitting beautifully,");
    crash();
  } else if(event->key.keysym.sym == SDLK_q) {
    printf("Quitting state.\n");
    devoke_state(state);
  } else if(event->key.keysym.sym == SDLK_RETURN) {
    printf("Invoking state2 hopefully...\n");
    invoke_state(state->invocables->this, state->stateman);
  }

  return 0;
}

////-- state2:

int st2_init(State* state)
{
  Image* img = NULL;
  SDL_Surface* surface;

  img = (Image*)malloc_or_die(sizeof(Image),
                              "Unable to alloc struct for image\n");
  printf("trying to load image...\n");
  surface = get_image(testpic_2_bmp, testpic_2_bmp_len);
  if(img == NULL) {
    printf("waaaaah, somethingwrong with secooond image /(*0*)\\\n");
    crash();
  }
  printf("loaded image \\(^0^)/\n");

  *img = texify(surface, state->stateman->renderer.sdl_renderer,
                state->stateman->window);

  datapush(img, &state->data);

  free(surface);
  return 0;
}

int st2_deinit(State* state)
{
  free(state->data->first);
  datapop(&state->data);

  return 0;
}

int st2_redraw(State* state)
{
  static Image* img;
  static SDL_Renderer* renderer;

  img = (Image*)state->data->first;
  renderer = state->stateman->renderer.sdl_renderer;

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, img->tex, NULL, &img->rect);
  SDL_RenderPresent(renderer);

  return 0;
}

int st2_keydown(State* state, SDL_Event* event)
{
  devoke_state(state);
  return 0;
}

//// Main ////

int main(int argc, char** argv)
{
  StateMan sttmanager = stateman();
  State* state1 = NULL;
  State* state2 = NULL;

  state1 = state(&st1_init, &st1_deinit, &st1_redraw,
                 NULL, &st1_keydown, NULL);
  state2 = state(&st2_init, &st2_deinit, &st2_redraw,
                 NULL, &st2_keydown, NULL);
  add_invocable(state2, state1);

  printf("\nHAI SEKAI! SHYAMEIMARU!\n");
  init(&sttmanager, "TEST", 512, 512, false);

  invoke_state(state1, &sttmanager);

  deinit();

  return 0;
}
