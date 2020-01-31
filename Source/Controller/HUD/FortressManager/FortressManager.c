#include "FortressManager.h"
#include "../../../Model/Utilities/Utilities.h"

char* EnumToStringFortress(int index)
{
	switch (index)
	{
		case 0:
			return("Nothing");
		case 1:
			return("Tribe");
		case 2:
			return("Builder:");
		case 3:
			return("MaxBuilder:");
		case 4:
			return("MaterialsUsed");
		case 5:
			return("MaterialNeeded");
		case 6:
			return("TurnProgress:");
		case 7:
			return("TotalTurnNeeded");
		default:
			return("NULL");
	}
}

void UpdateFortressDisplayButton(DisplayButton *buttons, int* data)
{
	sfTexture* partIlluTexture;
	sfTexture* playerIconTexture;
	if (!buttons->fakeButton.isWorking)
	{
		buttons->fakeButton.isWorking = true;
	}

	switch (data[0])
	{
	case 0 :
		partIlluTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/mur.png", NULL);
		break;
	case 1:
		partIlluTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/bouclier.png", NULL);
		break;
	case 2:
		partIlluTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/porte.png", NULL);
		break;
	default:
		break;
	}

	if (data[2] == 2)
	{
		playerIconTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/peopleGreen.png", NULL);
	}
	else
	{
		playerIconTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/peopleYellow.png", NULL);
	}

	sfTexture_setSmooth(partIlluTexture, sfTrue);
	sfSprite_setTexture(buttons->fortressPartIllu, partIlluTexture, 1);
	sfTexture_setSmooth(playerIconTexture, sfTrue);
	sfSprite_setTexture(buttons->resourceIllu[0], playerIconTexture, 1);

	sfText_setString(buttons->fortressText[0], enumToString(data[0]));
	UpdateTextWithStringDisplay(buttons->fortressText[1], EnumToStringFortress(1), data[2]);

	UpdateTextWithSeparator(buttons->fortressText[2], data[3], "/", data[4]);
	UpdateTextWithSeparator(buttons->fortressText[3], data[5], "/", data[6]);
	sfText_setString(buttons->fortressText[4], EnumToStringFortress(6));
	UpdateTextWithSeparator(buttons->fortressText[5], data[7], "/", data[8]);
}

void DisplayFortressPanel(FortressPanel panelToDisplay, sfRenderWindow* mainWindow)
{
	if (panelToDisplay.isActive)
	{
		sfRenderWindow_drawSprite(mainWindow, panelToDisplay.fortressBackGround, NULL);
		for (int i = 0; i < 3; i++)
		{
			if (panelToDisplay.fortressDisplayButton[i].fakeButton.isWorking)
			{
				DisplayButtons(mainWindow, &panelToDisplay.fortressDisplayButton[i].fakeButton, 1);
				sfRenderWindow_drawSprite(mainWindow, panelToDisplay.fortressDisplayButton[i].fortressPartIllu, NULL);
				for (int j = 0; j < 6; j++)
				{
					sfRenderWindow_drawText(mainWindow, panelToDisplay.fortressDisplayButton[i].fortressText[j], NULL);
				}
				for (int j = 0; j < 2; j++)
				{
					sfRenderWindow_drawSprite(mainWindow, panelToDisplay.fortressDisplayButton[i].resourceIllu[j], NULL);
				}
			}
		}
		DisplayButtons(mainWindow, panelToDisplay.buttonsQuitandSendRes, 2);
	}
}

void ResetFortressPanel(FortressPanel *PanelToReset)
{
	for (int i = 0; i < 3; i++)
	{
		PanelToReset->fortressDisplayButton[i].fakeButton.isWorking = false;
	}
	PanelToReset->isActive = false;
}

FortressPanel FortressPanelProto()
{
	FortressPanel newPanel;

	sfVector2f size = { 324.0f, 100.0f };
	sfVector2f posOrigin = { 1920.0f / 2 - 555 / 2, 160.0f };
	sfVector2f tempPos;
	tempPos.x = posOrigin.x + 25.0f;
	tempPos.y = posOrigin.y + 55.0f;

	for (int i = 0; i < 3; i++)
	{
		//Pos Button with whiteBackground
		if (i > 0)
		{
			tempPos.y += 170.0f + 18.0f;
		}
	
		newPanel.fortressDisplayButton[i].fakeButton = InitButton(size, tempPos, -1, NULL, false);

		sfVector2f textTempPos = tempPos;
		sfVector2f illuTempPos = tempPos;
		sfVector2f partIlluTempPos = { tempPos.x + 504 / 2 - 70 / 2, tempPos.y + 30.0f };

		textTempPos.x += 505.0f / 2 - 80.0f;
		textTempPos.y += 10.0f;
		newPanel.fortressDisplayButton[i].fortressText[0] = InitText("test", textTempPos, sfBlack, 20);

		newPanel.fortressDisplayButton[i].fortressPartIllu = sfSprite_create();
		sfSprite_setPosition(newPanel.fortressDisplayButton[i].fortressPartIllu, partIlluTempPos);
		
		textTempPos.x = tempPos.x + 10.0f;
		textTempPos.y += 22.0f;
		newPanel.fortressDisplayButton[i].fortressText[1] = InitText("test", textTempPos, sfBlack, 20);
		

		illuTempPos.x = tempPos.x + 10.0f;
		illuTempPos.y = textTempPos.y + 22.0f;
		newPanel.fortressDisplayButton[i].resourceIllu[0] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/people.png", 0);
		sfSprite_setPosition(newPanel.fortressDisplayButton[i].resourceIllu[0], illuTempPos);

		textTempPos.x = illuTempPos.x + 35.0f + 15.0f;
		textTempPos.y = illuTempPos.y + 10.0f;
		newPanel.fortressDisplayButton[i].fortressText[2] = InitText("test", textTempPos, sfBlack, 20);

		illuTempPos.x = tempPos.x + 505.0f - 45.0f - 15.0f;
		newPanel.fortressDisplayButton[i].resourceIllu[1] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/wood.png", 0);
		sfSprite_setPosition(newPanel.fortressDisplayButton[i].resourceIllu[1], illuTempPos);

		textTempPos.x = illuTempPos.x - 30.0f - 48.0f;
		newPanel.fortressDisplayButton[i].fortressText[3] = InitText("test", textTempPos, sfBlack, 20);

		textTempPos.x = tempPos.x + 505 / 2 - 80.0f;
		textTempPos.y = tempPos.y + 170.0f - 50.0f;
		newPanel.fortressDisplayButton[i].fortressText[4] = InitText("test", textTempPos, sfBlack, 20);

		textTempPos.x += 55.0f;
		textTempPos.y += 24.0f;
		newPanel.fortressDisplayButton[i].fortressText[5] = InitText("test", textTempPos, sfBlack, 20);

		newPanel.fortressDisplayButton[i].fakeButton.isWorking = false;
	}

	newPanel.fortressBackGround = LoadSprite("./Resources/HUD/mainSceneHUD/FortressPanel/fortress-window.png", 0);
	sfSprite_setPosition(newPanel.fortressBackGround, posOrigin);
	newPanel.isActive = false;
	
	sfVector2f littleButsize = { 25.0f, 25.0f };
	sfFloatRect rect = sfSprite_getGlobalBounds(newPanel.fortressBackGround);

	sfVector2f tempPosition;
	tempPosition.x = rect.left + rect.width - 32.5f;
	tempPosition.y = rect.top;
	newPanel.buttonsQuitandSendRes[0] = InitButton(littleButsize, tempPosition, 0, "./Resources/HUD/mainSceneHUD/FortressPanel/close.png", 0);
	
	tempPosition.x = rect.left + rect.width / 2.0f;

	tempPosition.y = rect.top + rect.height + 50.0f;
	newPanel.buttonsQuitandSendRes[1] = InitButton(size, tempPosition, 1, "./Resources/HUD/mainSceneHUD/FortressPanel/send-button.png", 1);
	
	return (newPanel);
}