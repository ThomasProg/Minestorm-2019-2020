#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "SDL2/SDL.h"

typedef struct 
{
    TTF_Font* font;
    SDL_Rect location;
    char* str;
} t_textbox;

t_textbox* textbox_create(const char* str, SDL_Rect location, TTF_Font* font);
void       textbox_destroy(t_textbox* textbox);
void       textbox_render(SDL_Renderer* renderer, t_textbox* textbox);
void       textbox_setText(t_textbox* textbox, const char* newStr);

#endif