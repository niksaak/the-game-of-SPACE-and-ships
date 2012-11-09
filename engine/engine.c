#include <assert.h>

#include "engine.h"
#include "system.h"
#include "state.h"

#define INIT_FLAGS SDL_INIT_VIDEO

static void create_glcontext_or_renderer(StateMan* stateman)
{
  assert(stateman->renderer.type == RENDERER_TYPE_NIL);

  Uint32 winflags = SDL_GetWindowFlags(stateman->window);

  if(winflags & SDL_WINDOW_OPENGL) {
    stateman->renderer.type = RENDERER_TYPE_OGL;
    stateman->renderer.glcontext = NULL;
    printf("TODO: OpenGL support. \\(^ж^)/\n");
  } else {
    stateman->renderer.type = RENDERER_TYPE_SDL;
    stateman->renderer.sdl_renderer =
      SDL_CreateRenderer(stateman->window, -1,
                         SDL_RENDERER_ACCELERATED |
                         SDL_RENDERER_TARGETTEXTURE);
  }
}

void init(StateMan* stateman,
          const char* title, int width, int height, bool ogl)
{
  assert(stateman != NULL);
  
  Uint32 ogl_flag;

  if(stateman->running) {
    fprintf(stderr, "ERROR: Init called when `running' flag set.\n");
    crash();
  }

  printf("Initializing SDL: ");
  if(SDL_Init(INIT_FLAGS) != 0) {
    errorprint_SDL();
    crash();
  }
  printf("OK\n");

  printf("Creating window: ");
  if(ogl) {
    ogl_flag = SDL_WINDOW_OPENGL;
  } else {
    ogl_flag = 0;
  }
  stateman->window = SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      width, height,
                                      SDL_WINDOW_SHOWN | ogl_flag);
  if(stateman->window == NULL) {
    errorprint_SDL();
    crash();
  }
  printf("OK.\n");

  printf("Creating renderer: ");
  create_glcontext_or_renderer(stateman);
  printf("OK.\n");
}

void deinit() {
  printf("Deinitializing SDL: ");
  SDL_Quit();
  printf("OK.\n");
}
