#ifndef _BUILDMANAGER_H
#define _BUILDMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../Text/Text.h"
#include "../Button/Buttons.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"

typedef struct
{
	Button buildButton;
	sfText* buildText[3];
	bool isBuild;
}ButtonBuild;

typedef struct
{
	sfSprite *panelBackground;
	ButtonBuild buildsButt[3];
	bool isActive;
}BuildPanel;



void UpdateBuildButtonsUpgradeBuild(ButtonBuild* buttons);
void UpdateBuildButtons(ButtonBuild *buttons, int* data);
void DisplayBuildPanel(BuildPanel panels[], sfRenderWindow* mainWindow);
void ResetBuildPanel(BuildPanel* PanelToReset);
BuildPanel BuildPanelProto(int index);



#endif // !_BUILDMANAGER_H