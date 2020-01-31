#ifndef _GRAPHICSUTILITIES_H
#define _GRAPHICSUTILITIES_H

sfSprite* LoadSprite(char* sNom, int isCentered);
void BlitSprite(sfRenderWindow* window, sfSprite* sprite, float fPosX, float fPosY);


#endif // !_GRAPHICSUTILITIES_H
