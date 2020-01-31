#include "ExpeditionManager.h"

void UpdateExpeditionsPos(ButtonExpedition button[], int indexOfButton)
{
	sfVector2f posButt = sfSprite_getPosition(button[indexOfButton].expeditionButton.sprite);

	for (int i = 0; i < 3; i++)
	{	
		if (button[i].expeditionButton.isWorking)
		{
			sfVector2f posTemp = sfSprite_getPosition(button[i].expeditionButton.sprite);
			if (posTemp.y > posButt.y)
			{
				UpdateExpeditionsPos(button, i);

				posTemp.y = posButt.y;
				ChangeButtonPosition(&button[i].expeditionButton, posTemp);
			
				// Redisposition of all texts and illustrations
				for (int j = 0; j < 4; j++)
				{
					sfVector2f resourceIlluTempPos = sfSprite_getPosition(button[indexOfButton].resourcesIllu[j]);
					sfSprite_setPosition(button[i].resourcesIllu[j], resourceIlluTempPos);
				}
				for (int j = 0; j < 5; j++)
				{
					sfVector2f textTempPos = sfText_getPosition(button[indexOfButton].expeditionText[j]);
					sfText_setPosition(button[i].expeditionText[j], textTempPos);
				}

				return;
			}
		}
	}
}

void ChangeExpButtonsColor(ButtonExpedition* button, bool hasExpToHandle)
{
	for (int i = 0; i < 5; i++)
	{
		if (!button->expIsManaged && hasExpToHandle)
		{
			sfText_setFillColor(button->expeditionText[i], sfColor_fromRGB(0, 0, 0));
			sfSprite_setColor(button->expeditionButton.sprite, sfColor_fromRGB(255, 255, 255));
		}
		else 
		{
			sfText_setFillColor(button->expeditionText[i], sfColor_fromRGBA(0, 0, 0, 100));
			sfSprite_setColor(button->expeditionButton.sprite, sfColor_fromRGBA(255, 255, 255, 100));
		}
	}
}

void UpdateExpeditionsButton(ButtonExpedition button[], ButtonExpedition origin, int* data, int playerID)
{
	if (!button[data[0]].expeditionButton.isWorking)
	{
		sfVector2f higherButtonPos = { origin.expeditionButton.rect.left, origin.expeditionButton.rect.top };
		int workingButtonCpt = 0;
		

		for (int i = 0; i < 3; i++)
		{
			if (button[i].expeditionButton.isWorking)
			{
				workingButtonCpt++;
			}
		}

		button[data[0]].expeditionButton.isWorking = true;

		sfVector2f newButtonPos = { higherButtonPos.x, higherButtonPos.y + 115.0f * workingButtonCpt };

		ChangeButtonPosition(&button[data[0]].expeditionButton, newButtonPos);

		for (int j = 0; j < 4; j++)
		{
			sfVector2f resourceIlluPos = sfSprite_getPosition(origin.resourcesIllu[j]);
			resourceIlluPos.y += 115.f * workingButtonCpt;

			sfSprite_setPosition(button[data[0]].resourcesIllu[j], resourceIlluPos);
		}
		for (int j = 0; j < 5; j++)
		{
			sfVector2f textPos = sfText_getPosition(origin.expeditionText[j]);
			textPos.y += 115.f * workingButtonCpt;

			sfText_setPosition(button[data[0]].expeditionText[j], textPos);
		}
	}

	UpdateTextWithStringDisplay(button[data[0]].expeditionText[0], "Turn", data[1]);
	for (int i = 1; i < 5; i++)
	{
		UpdateTextDisplay(button[data[0]].expeditionText[i], data[i + 1]);
	}

	if (data[1] == 0)
	{
		button[data[0]].expeditionButton.isWorking = false;
		UpdateExpeditionsPos(button, data[0]);
	}
	
}

void DisplayExpeditionPanel(ExpeditionPanel panels[], sfRenderWindow* mainWindow)
{
	for (int id = 0; id < 2; id++)
	{
		if (panels[id].isActive)
		{
			sfRenderWindow_drawSprite(mainWindow, panels[id].panelBackground, NULL);
			for (int i = 0; i < 3; i++)
			{
				if (panels[id].expeditionsButt[i].expeditionButton.isWorking)
				{
					DisplayButtons(mainWindow, &panels[id].expeditionsButt[i].expeditionButton, 1);

					for (int j = 0; j < 5; j++)
					{
						sfRenderWindow_drawText(mainWindow, panels[id].expeditionsButt[i].expeditionText[j], NULL);
					}
					for (int j = 0; j < 4; j++)
					{
						sfRenderWindow_drawSprite(mainWindow, panels[id].expeditionsButt[i].resourcesIllu[j], NULL);
					}
				}
			}
		}
	}
}

void ResetExpPanel(ExpeditionPanel* PanelToReset)
{
	PanelToReset->isActive = false;
	for (int i = 0; i < 3; i++)
	{
		PanelToReset->expeditionsButt[i].expeditionButton.isWorking = false;
		PanelToReset->expeditionsButt[i].expIsManaged = false;
	}
}

ExpeditionPanel PanelProto(int index)
{
	ExpeditionPanel newPanel;

	sfFont* font = sfFont_createFromFile("./Resources/Font/Action of the Time II.ttf");

	sfVector2f size = { 324.0f, 100.0f };
	sfVector2f posOrigin = { 30.0f + index * (1920.0f - 339.0f - 60.0f), 600.0f };
	char* filesDirectories = "./Resources/HUD/mainSceneHUD/ExpeditionPanel/ExpeditionsStatsButton.png";

	for (int  i = 0; i < 3; i++)
	{
		//Pos Button with whiteBackground
		sfVector2f tempPos = posOrigin;
		tempPos.x += 6.0f;
		tempPos.y += i * 115.f;
		printf("%f\n", tempPos.y);
		newPanel.expeditionsButt[i].expeditionButton = InitButton(size, tempPos, i, filesDirectories, false);

		//Disposition illustrations and texts in the button
		sfVector2f illusTempPos = tempPos;
		sfVector2f textTempPos = illusTempPos;

		// Turn text
		textTempPos.x += size.x /2 - 20.0f;
		textTempPos.y += 2.0f;
		newPanel.expeditionsButt[i].expeditionText[0] = InitText("Turn", textTempPos, sfBlack, 18);

		// Villagers illu and text
		illusTempPos.x = tempPos.x + 10.0f;
		illusTempPos.y += 15.0f;
		newPanel.expeditionsButt[i].resourcesIllu[0] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/people.png", 0);
		sfSprite_setPosition(newPanel.expeditionsButt[i].resourcesIllu[0], illusTempPos);

		textTempPos.x = illusTempPos.x + 15.0f + 15.0f;
		textTempPos.y = illusTempPos.y + 4.0f;
		newPanel.expeditionsButt[i].expeditionText[1] = InitText("", textTempPos, sfBlack, 18);
		
		// Food illu and text
		illusTempPos.y += 50.0f;
		newPanel.expeditionsButt[i].resourcesIllu[1] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/food.png", 0);
		sfSprite_setPosition(newPanel.expeditionsButt[i].resourcesIllu[1], illusTempPos);

		textTempPos.y = illusTempPos.y + 4.0f;
		newPanel.expeditionsButt[i].expeditionText[2] = InitText("", textTempPos, sfBlack, 18);

		// Water illu and text
		illusTempPos.x = tempPos.x + size.x - 25.0f - 15.0f;
		newPanel.expeditionsButt[i].resourcesIllu[2] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/water.png", 0);
		sfSprite_setPosition(newPanel.expeditionsButt[i].resourcesIllu[2], illusTempPos);

		textTempPos.x = illusTempPos.x - 25.0f;
		newPanel.expeditionsButt[i].expeditionText[3] = InitText("", textTempPos, sfBlack, 18);

		// Materials illu and text
		illusTempPos.y -= 50.0f;
		newPanel.expeditionsButt[i].resourcesIllu[3] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/wood.png", 0);
		sfSprite_setPosition(newPanel.expeditionsButt[i].resourcesIllu[3], illusTempPos);

		textTempPos.y = illusTempPos.y + 4.0f;
		newPanel.expeditionsButt[i].expeditionText[4] = InitText("", textTempPos, sfBlack, 18);

		// Rescale illustrations
		sfVector2f scaleIllu;
		scaleIllu.x = 15.0f / 25.0f;
		scaleIllu.y = scaleIllu.x;
		
		for (int j = 0; j < 4; j++)
		{
			sfSprite_setScale(newPanel.expeditionsButt[i].resourcesIllu[j], scaleIllu);
		}
		
		newPanel.expeditionsButt[i].expeditionButton.isWorking = false;
		newPanel.expeditionsButt[i].expIsManaged = false;
	}

	newPanel.higherButtonInPanel = newPanel.expeditionsButt[0];

	newPanel.panelBackground = LoadSprite("./Resources/HUD/mainSceneHUD/ExpeditionPanel/expeditionPanelBackground.png", 0);

	posOrigin.y -= 60.0f;
	sfSprite_setPosition(newPanel.panelBackground, posOrigin);
	newPanel.isActive = false;

	return newPanel;
}