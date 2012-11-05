NAME := game
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
ENGINE_SOURCES := $(wildcard engine/*.c)
ENGINE_OBJECTS := $(ENGINE_SOURCES:.c=.o)

CFLAGS += -Wall -O0 -std=c11
LDFLAGS += -lSDL -lSDL_gfx -lm -lchipmunk

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
