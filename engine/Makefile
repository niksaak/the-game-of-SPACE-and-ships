SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS_STATIC := $(shell sdl2-config --static-libs)
CFLAGS += $(SDL_CFLAGS) -Wall -O0 -std=c11
LDFLAGS += $(SDL_LDFLAGS_STATIC)

.PHONY: all clean

all: $(OBJECTS)

clean:
	@- $(RM) -v $(OBJECTS)
