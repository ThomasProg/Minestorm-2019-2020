#ifndef _FONT_H_
#define _FONT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "queue.h"

typedef struct s_font
{
	t_queue* font;
} t_font;

t_font* font_create(const unsigned int nbFonts);
void    font_add(t_font* font, const char* filename, const unsigned int size);
void    font_destroy(t_font* font);

#endif
