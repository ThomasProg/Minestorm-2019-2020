#include "font.h"

t_font* font_create(const unsigned int nbFonts)
{
	t_font* font = malloc(sizeof(*font));
	font->font = queue_create(nbFonts, sizeof(TTF_Font*));

	return font;
}

void font_add(t_font* font, const char* filename, const unsigned int size)
{
	queue_add(font->font);
	TTF_Font** elems = font->font->data;
	elems[font->font->nextIndex] = TTF_OpenFont(filename, size);
	font->font->nextIndex++;
}

void font_destroy(t_font* font)
{
	TTF_Font** font_style;

	for (unsigned int i = 0; i < font->font->nextIndex; i++)
	{
		font_style = font->font->data;
		if (font_style[i] != NULL)
			TTF_CloseFont(font_style[i]);
	}
	
	queue_destroy(font->font);
	free(font);
}

TTF_Font* font_get(t_font* font, unsigned int i)
{
	return ((TTF_Font**)(font->font->data))[i];
	//return *((TTF_Font**) dynamicArray_GetItem(&font->font, i));
}
