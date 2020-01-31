#include "Sounds.h"

// loads a sound
sfSound* LoadSound()
{
	sfSound* sound = sfSound_create();
	return sound;
}

sfSoundBuffer** bufferSound(char* bufferFile[], int lenght)
{
	sfSoundBuffer** buffer;
	buffer = malloc(lenght*sizeof(sfSoundBuffer*));

	for (int i = 0; i < lenght; i++)
	{
		sfSoundBuffer* buffer = sfSoundBuffer_createFromFile(bufferFile[lenght]);
		sfSound_setBuffer(LoadSound(), buffer);
	}
}

// loads a music
sfMusic* LoadMusic(char* musicFile)
{
	sfMusic* music = sfMusic_createFromFile(musicFile);
	return music;
}

// changes a sound's volume, pitch, and make it loop or not
void SetSoundData(sfSound* sound, float volume, float pitch, bool isLooping)
{
	sfSound_setVolume(sound, volume);
	sfSound_setPitch(sound, pitch);
	sfSound_setLoop(sound, isLooping);
}

// changes a music's volume, pitch, and make it loop or not
void SetMusicData(sfMusic* music, float volume, float pitch, bool isLooping)
{
	sfMusic_setVolume(music, volume);
	sfMusic_setPitch(music, pitch);
	sfMusic_setLoop(music, isLooping);
}

// plays a sound
void SoundPlay(sfSound* sound)
{
	if (sfSound_getStatus(sound) != sfPlaying)
	{
		sfSound_play(sound);
	}
}

// plays a music
void MusicPlay(sfMusic* music)
{
	if (sfMusic_getStatus(music) != sfPlaying)
	{
		sfMusic_play(music);
	}
}