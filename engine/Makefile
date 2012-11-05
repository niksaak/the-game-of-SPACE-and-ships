SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)

CFLAGS += -Wall -O0 -std=c11
LDFLAGS += -lSDL

default: $(OBJECTS)

clean:
	@- $(RM) -v $(OBJECTS)
