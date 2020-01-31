#include <SFML\Audio.h>
#include <stdbool.h>

#ifndef SOUNDS_H
#define SOUNDS_H

// loads a sound
sfSound* LoadSound();

sfSoundBuffer** bufferSound(char* bufferFile[], int lenght);

// loads a music
sfMusic* LoadMusic(char* musicFile);

// changes a sound's volume, pitch, and make it loop or not
void SetSoundData(sfSound* sound, float volume, float pitch, bool isLooping);

// changes a music's volume, pitch, and make it loop or not
void SetMusicData(sfMusic* music, float volume, float pitch, bool isLooping);

// plays a sound
void SoundPlay(sfSound* sound);

// plays a music
void MusicPlay(sfMusic* music);

#endif // SOUNDS_H