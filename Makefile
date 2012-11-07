NAME := game
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
ENGINE_SOURCES := $(wildcard engine/*.c)
ENGINE_OBJECTS := $(ENGINE_SOURCES:.c=.o)

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS_STATIC := -static $(shell sdl2-config --static-libs)
CFLAGS += $(SDL_CFLAGS) -Wall -O0 -std=c11
LDFLAGS += $(SDL_LDFLAGS_STATIC) -lSDL_gfx -lm -lchipmunk

.PHONY: all engine clean

all: $(NAME)

$(NAME): $(OBJECTS) $(ENGINE_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $<

$(ENGINE_OBJECTS):
	$(MAKE) -C engine/

clean:
	@- $(RM) -v $(NAME) ; \
	   $(RM) -v $(OBJECTS) ; \
	   $(MAKE) -C engine/ clean
