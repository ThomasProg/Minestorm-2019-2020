BINARY=exe

CFLAGS=-Wall -Werror -O0 -g -MMD -Isrc -I/usr/include
LDLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRCS= $(wildcard src/dynamicArray/*.c) $(wildcard src/widgets/*.c) $(wildcard src/vector2D/*.c) $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/SDL_utilities/*.c) $(wildcard src/entities/*.c) $(wildcard src/entities/mines_subtype/*.c)
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
