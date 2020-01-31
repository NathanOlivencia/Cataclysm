#include <SFML\Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GraphicsUtilities.h"


sfSprite* LoadSprite(char* sNom, int isCentered)
{
	sfTexture* tempTex;
	sfSprite* tempSprite;
	tempTex = sfTexture_createFromFile(sNom, NULL);

	if (tempTex == NULL)
	{
		printf("Could not load a texture\n");
	}

	tempSprite = sfSprite_create();
	sfTexture_setSmooth(tempTex, true);
	sfSprite_setTexture(tempSprite, tempTex, 0);

	if (isCentered)
	{
		sfVector2u tailleImage = sfTexture_getSize(tempTex);
		sfVector2f centre = { tailleImage.x / 2.0f, tailleImage.y / 2.0f };
		sfSprite_setOrigin(tempSprite, centre);
	}
	return tempSprite;
}

void BlitSprite(sfRenderWindow* window, sfSprite* sprite, float fPosX, float fPosY)
{
	sfVector2f tempPos = { fPosX, fPosY };
	sfSprite_setPosition(sprite, tempPos);
	sfRenderWindow_drawSprite(window, sprite, NULL);
}