#include "SDL_utilities.h"

t_assets* assets_create
	(const unsigned int nbTextures, const unsigned int nbSounds, const unsigned int nbFonts)
{
	t_assets* assets = malloc(sizeof(t_assets));

	assets->render   = render_create(nbTextures);
	assets->audio    = audio_create(nbSounds);
	assets->font     = font_create(nbFonts);
	
	return assets;
}

void assets_destroy(t_assets* assets)
{
	if (assets == NULL)
		return;

	render_destroy(assets->render);
	audio_destroy(assets->audio);
	font_destroy(assets->font);

	free(assets);
}
