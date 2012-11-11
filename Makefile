NAME := game
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
BITMAPS := $(wildcard *.bmp)
BITMAP_INCLUDES := $(BITMAPS:.bmp=bmp.c)
BITMAP_OBJECTS := $(BITMAPS:.bmp=bmp.o)
ENGINE_SOURCES := $(wildcard engine/*.c)
ENGINE_OBJECTS := $(ENGINE_SOURCES:.c=.o)

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS_STATIC := $(shell sdl2-config --static-libs)
CFLAGS += $(SDL_CFLAGS) -Wall -O0 -std=c11
LDFLAGS += $(SDL_LDFLAGS_STATIC)

.PHONY: all engine clean
.SUFFIXES: .bmp

all: $(NAME)

$(NAME): $(OBJECTS) $(ENGINE_OBJECTS) $(BITMAP_OBJECTS)
	$(CC) -o $@ $? $(LDFLAGS)

$(ENGINE_OBJECTS):
	$(MAKE) -C engine/

%bmp.c: %.bmp
	xxd -i $< $@

clean:
	@- mv -v $(NAME) $(NAME).old ; \
	   $(RM) -v $(OBJECTS) ; \
	   $(RM) -v $(BITMAP_OBJECTS) $(BITMAP_INCLUDES) ; \
	   $(MAKE) -C engine/ clean

