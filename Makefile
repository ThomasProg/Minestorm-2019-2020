BINARY=exe

CFLAGS=-Wall -Werror -O0 -g -MMD -Isrc -Iinclude
LDLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRCS= $(wildcard src/vector2D/*.c) $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/SDL_utilities/*.c) $(wildcard src/entities/*.c)
OBJS=$(SRCS:.c=.o)

ifeq ($(TYPE),DEBUG)
	CFLAGS+=-D_DEBUG_
endif

.PHONY: all clean run

all: $(BINARY)

-include $(OBJS:.o=.d)

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

$(BINARY): $(OBJS)
	gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

run: $(BINARY)
	./$(BINARY)


clean:
	rm -rf $(OBJS) $(OBJS:.o=.d) $(BINARY)
