#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "macros.h"
#include "textbox.h"
#include "string/string_utilities.h"

t_textbox* textbox_create(const char* str, SDL_Rect location, TTF_Font* font)
{
    t_textbox* textbox = malloc(sizeof(t_textbox));
    textbox->location = location;
    textbox->font = font;
    //textbox->str = str; //to duplicate;
    textbox->str = string_duplicate(str);

    return textbox;
}

void textbox_destroy(t_textbox* textbox)
{
    free(textbox->str);
}

void textbox_setText(t_textbox* textbox, const char* newStr)
{
    if (textbox->str != NULL)
	    free(textbox->str);

    textbox->str = string_duplicate(newStr);
}

void textbox_render(SDL_Renderer* renderer, t_textbox* textbox)
{
    //TTF_Font* font = TTF_OpenFont("media/font.ttf", 20);
    // if (font == NULL)
    // {
    //     return;
    // }

    SDL_Color white = {255, 255, 255, 0};

    SDL_Surface* surfaceMess = TTF_RenderText_Solid(textbox->font, textbox->str, white);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMess);

    SDL_FreeSurface(surfaceMess);

    SDL_Rect rect = {WINDOW_SIZE_X/2, 0, 100, 100};

    TTF_SizeText(textbox->font, textbox->str, &rect.w, &rect.h);

    rect.w *= 2;
    rect.h *= 2;
    rect.x -= rect.w / 2;

    SDL_RenderCopy(renderer, message, NULL, &rect);

    SDL_DestroyTexture(message);
}