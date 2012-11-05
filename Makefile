CC      = gcc
CFLAGS  = -Wall -O0 -std=c11
LDFLAGS = -lSDL -lSDL_gfx -lm -lchipmunk
EXE     = game

$(EXE): main.o state.o
	$(CC) $(LDFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -v $(EXE) *.o
