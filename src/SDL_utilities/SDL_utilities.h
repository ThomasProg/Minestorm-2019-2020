#ifndef _SDL_UTILITIES_H_
#define _SDL_UTILITIES_H_

#include "render.h"
#include "audio.h"
#include "font.h"

typedef struct s_assets
{
	t_render* render;
	t_audio*  audio;
	t_font*   font;
} t_assets;

t_assets* assets_create (
		const unsigned int nbTextures,
	   	const unsigned int nbSounds, 
		const unsigned int nbFonts);

void assets_destroy(t_assets* assets);

#endif
