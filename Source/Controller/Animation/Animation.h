#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../GraphicsUtilities/GraphicsUtilities.h"

typedef struct
{
	int numberOfFrame;
	int actualFrameToShow;
	float actualDuration;
	float totalDuration;
	sfVector2f animPosition;
	sfSprite** spriteArray;
	bool loop;
	bool isLoaded;
} Animation;


void UpdateAnimation(Animation *animToPlay, float timeElapsed);
void UnLoadAnimation(Animation *toUnLoad);
void UnLoadFrameOfAnim(Animation *toUnLoad, int index);
void ReloadAnimationOrigin(Animation *toReload, char* path, char* extension);
void ReloadAnimation(Animation *toReload, char* path, char* extension, sfVector2f pos);
Animation LoadAnimationSpecifiedIndexStart(int numberOfFrame, float totalDuration, char* path, char* extension, sfVector2f pos, bool isLoop, int index);
Animation LoadAnimation(int numberOfFrame, float totalDuration, char* path, char* extension, sfVector2f pos, bool isLoop);

#endif // !_ANIMATION_H


// 1355 660