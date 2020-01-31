#ifndef _EXPEDITIONMANAGER_H
#define _EXPEDITIONMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../Text/Text.h"
#include "../Button/Buttons.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"

typedef struct
{
	Button expeditionButton;
	sfSprite* resourcesIllu[4];
	sfText* expeditionText[5];
	bool expIsManaged;
} ButtonExpedition;

typedef struct
{
	sfSprite *panelBackground;
	ButtonExpedition higherButtonInPanel;
	ButtonExpedition expeditionsButt[3];
	bool isActive;
} ExpeditionPanel;

void ChangeExpButtonsColor(ButtonExpedition* button, bool hasExpToHandle);
void UpdateExpeditionsButton(ButtonExpedition button[], ButtonExpedition origin, int* data, int playerID);
void DisplayExpeditionPanel(ExpeditionPanel panels[], sfRenderWindow* mainWindow);
void ResetExpPanel(ExpeditionPanel* PanelToReset);
//Player ID for placement purpose
ExpeditionPanel PanelProto(int index);

#endif // !_EXPEDITIONMANAGER_H