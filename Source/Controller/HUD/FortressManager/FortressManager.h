#ifndef _FORTRESSMANAGER_H
#define _FORTRESSMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../Text/Text.h"
#include "../Button/Buttons.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"


//Button that are not clickable / only for display 
typedef struct
{
	Button fakeButton;
	sfSprite* fortressPartIllu;
	sfText* fortressText[6];
	sfSprite* resourceIllu[2];
}DisplayButton;

typedef struct
{
	sfSprite* fortressBackGround;
	DisplayButton fortressDisplayButton[3];
	Button buttonsQuitandSendRes[2];
	bool isActive;
}FortressPanel;


void UpdateFortressDisplayButton(DisplayButton *buttons, int* data);
void DisplayFortressPanel(FortressPanel panelToDisplay, sfRenderWindow* mainWindow);
void ResetFortressPanel(FortressPanel *PanelToReset);
FortressPanel FortressPanelProto();

#endif // !_FORTRESSMANAGER_H
