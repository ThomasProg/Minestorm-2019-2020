#include "audio.h"

t_audio* audio_create(const unsigned int nbSounds)
{
	if (nbSounds > 0)
	{
		t_audio* audio = malloc(sizeof(*audio));
		audio->sounds = queue_create(nbSounds, sizeof(Mix_Chunk*));
		audio->music = NULL;

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		return audio;
	}
	else
		return NULL;
}

void audio_add(t_audio* audio, const char* filename)
{
	queue_add(audio->sounds);
	Mix_Chunk** elems = audio->sounds->data;
	elems[audio->sounds->nextIndex] = Mix_LoadWAV(filename);
	audio->sounds->nextIndex++;
}

void audio_destroy(t_audio* audio)
{
	if (audio != NULL)
	{
		Mix_Chunk** sound;

		for (unsigned int i = 0; i < audio->sounds->nextIndex; i++)
		{
			sound = audio->sounds->data;
			if (sound[i] != NULL)
				Mix_FreeChunk(sound[i]);
		}
		
		if (audio->music != NULL)
			free(audio->music);

		Mix_CloseAudio();
		
		queue_destroy(audio->sounds);
		free(audio);
	}
}
