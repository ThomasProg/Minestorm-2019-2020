#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "queue.h"

typedef struct s_audio
{
	t_queue*   sounds;
	Mix_Music* music; //TODO : create and destroy music
} t_audio;

t_audio* audio_create(const unsigned int nbSounds);
void     audio_add(t_audio* audio, const char* filename);
void     audio_destroy(t_audio* audio);

#endif
