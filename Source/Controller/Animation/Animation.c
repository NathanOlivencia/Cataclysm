#include "Animation.h"

void UpdateAnimation(Animation *animToPlay, float timeElapsed)
{
	animToPlay->actualDuration += timeElapsed;
	float temp = animToPlay->actualDuration / animToPlay->totalDuration;

	if (temp >= 1.0f)
	{
		if (animToPlay->loop)
		{
			animToPlay->actualDuration -= animToPlay->totalDuration * (int)temp;
			temp = animToPlay->actualDuration / animToPlay->totalDuration;
		}
		else
		{
			animToPlay->actualFrameToShow = animToPlay->numberOfFrame - 1;
			return;
		}
	}

	animToPlay->actualFrameToShow = (int)(animToPlay->numberOfFrame * temp);

}

void UnLoadAnimation(Animation *toUnLoad)
{
	printf("ANIMATION UNLOAD\n");

	for (int i = 0; i < toUnLoad->numberOfFrame; i++)
	{
		sfTexture_destroy(sfSprite_getTexture(toUnLoad->spriteArray[i]));
		sfSprite_destroy(toUnLoad->spriteArray[i]);
	}

	toUnLoad->isLoaded = false;
}

void UnLoadFrameOfAnim(Animation *toUnLoad, int index)
{
	sfTexture_destroy(sfSprite_getTexture(toUnLoad->spriteArray[index]));
	sfSprite_destroy(toUnLoad->spriteArray[index]);
}

void ReloadAnimationOrigin(Animation *toReload, char* path, char* extension)
{
	printf("RELOAD OF : %s\n", path);
	toReload->actualDuration = 0.0f;
	toReload->actualFrameToShow = 0;
	char directoryOfimage[250];
	for (int i = 0; i < toReload->numberOfFrame; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "0000";
		}
		else if (i < 100)
		{
			toAdd = "000";
		}
		else if (i < 1000)
		{
			toAdd = "00";
		}
		else
		{
			toAdd = "0";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		toReload->spriteArray[i] = LoadSprite(directoryOfimage, 0);
	}

	toReload->isLoaded = true;
}

void ReloadAnimation(Animation *toReload, char* path, char* extension, sfVector2f pos)
{
	printf("RELOAD OF : %s\n", path);
	toReload->actualDuration = 0.0f;
	toReload->actualFrameToShow = 0;
	char directoryOfimage[250];
	for (int i = 0; i < toReload->numberOfFrame; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "0000";
		}
		else if (i < 100)
		{
			toAdd = "000";
		}
		else if (i < 1000)
		{
			toAdd = "00";
		}
		else
		{
			toAdd = "0";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		toReload->spriteArray[i] = LoadSprite(directoryOfimage, 0);
		sfSprite_setPosition(toReload->spriteArray[i], pos);
	}

	toReload->isLoaded = true;

}

Animation LoadAnimationSpecifiedIndexStart(int numberOfFrame, float totalDuration, char* path, char* extension, sfVector2f pos, bool isLoop, int index)
{
	Animation test;

	test.numberOfFrame = numberOfFrame;
	test.totalDuration = totalDuration;
	test.actualDuration = 0.0f;
	test.spriteArray = NULL;
	test.actualFrameToShow = 0;
	test.animPosition = pos;
	test.loop = isLoop;
	test.isLoaded = true;
	test.spriteArray = malloc(numberOfFrame * sizeof(sfSprite*));
	if (test.spriteArray == NULL)
	{
		printf("Error can't allocate dynamacily the array of sprite of the animation");
		test.actualDuration = -1.0f;
		return test;
	}

	printf("LOAD OF : %s\n", path);

	char directoryOfimage[250];
	for (int i = index; i < numberOfFrame + index; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "000";
		}
		else if (i < 100)
		{
			toAdd = "00";
		}
		else if (i < 1000)
		{
			toAdd = "0";
		}
		else
		{
			toAdd = "";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		test.spriteArray[i - index] = LoadSprite(directoryOfimage, 0);
		if (pos.x != 0 || pos.y != 0)
		{
			sfSprite_setPosition(test.spriteArray[i], pos);
		}
	}

	return test;

}



Animation LoadAnimation(int numberOfFrame, float totalDuration, char* path, char* extension, sfVector2f pos, bool isLoop)
{
	Animation test;

	test.numberOfFrame = numberOfFrame;
	test.totalDuration = totalDuration;
	test.actualDuration = 0.0f;
	test.spriteArray = NULL;
	test.actualFrameToShow = 0;
	test.animPosition = pos;
	test.loop = isLoop;
	test.isLoaded = true;
	test.spriteArray = malloc(numberOfFrame * sizeof(sfSprite*));
	if (test.spriteArray == NULL)
	{
		printf("Error can't allocate dynamacily the array of sprite of the animation");
		test.actualDuration = -1.0f;
		return test;
	}

	printf("LOAD OF : %s\n", path);

	char directoryOfimage[250];
	for (int i = 0; i < numberOfFrame; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "0000";
		}
		else if (i < 100)
		{
			toAdd = "000";
		}
		else if (i < 1000)
		{
			toAdd = "00";
		}
		else
		{
			toAdd = "0";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		test.spriteArray[i] = LoadSprite(directoryOfimage, 0);
		if (pos.x != 0 || pos.y != 0)
		{
			sfSprite_setPosition(test.spriteArray[i], pos);
		}
	}

	return test;

}