#ifndef _SELECTION_MENU_H
#define _SELECTION_MENU_H

#include "SFML\Graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../../Model/Cards/Cards.h"
#include "../Button/Buttons.h"
#include "../Text/Text.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"

void InitSelectionScene(sfVideoMode mode, bool isReload);
bool CheckSelectionIsFinished();
void GetGodPlayer(int godIndex);
int* GetGodDatas(int playerID);
int getGodID(int playerID);
int GetGodSelectionButtonsIndex(sfVector2i mousePos);
void GodHoverAnimation(sfVector2i mousePosition, float deltaTime);
void UpdateGodSelectionMenu(void(*cbData)(int*), void(*cbSetGodHUD)(int, int), float deltatime);
void DisplaySelectionScene(sfRenderWindow* window);

#endif // !_SELECTION_MENU_H