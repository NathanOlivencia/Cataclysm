#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <SFML\Graphics.h>
#include "GameStateManager/GameStateManager.h"
#include "./HUD/HUDController.h"
#include "GraphicsUtilities\GraphicsUtilities.h"
#include "MainSceneController/MainSceneController.h"
#include <Windows.h>
#if _DEBUG
#include "../CheatCode/Cheat.h"
#endif


void InputManager(sfRenderWindow* mainWindow)
{
	static sfEvent event;
	
	while (sfRenderWindow_pollEvent(mainWindow, &event))
	{
		if (event.type == sfEvtKeyPressed)
		{
			if(event.key.code == sfKeyEscape)
			{
				ManageStateInputKeyboard();
				return;
			}
		}
		
		if (event.type == sfEvtMouseButtonPressed)
		{
			if (event.mouseButton.button == sfMouseLeft)
			{
				ManageState(sfMouse_getPositionRenderWindow(mainWindow));
			}
		}

		if (event.type == sfEvtClosed)
		{
			exit(0);
		}
	}
}

void UpdateManager(sfRenderWindow* mainWindow, float deltaTime)
{
#if _DEBUG
	CheatMain();
#endif
	UpdateState(mainWindow, deltaTime);
}

void DisplayManager(sfRenderWindow* mainWindow)
{
	sfRenderWindow_clear(mainWindow, sfWhite);

	DisplayMainScene(mainWindow, GetActualState());
	DisplayHUD(mainWindow, GetActualScene());

	sfRenderWindow_display(mainWindow);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, INT nCmdShow)
{
	sfRenderWindow* mainWindow = NULL;
	sfClock* timerClock;
	float deltaTime;
	timerClock = sfClock_create();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	printf("CataClysm Prototype V1.0\nComputer Side Simulation \n-----------------------------------\n");

	//Init of models values

	int turn = 1;

	float countTo1sec = 0.0f;
	int frameThisSec = 0;

	InitGameState();
	InitMainMenu(sfVideoMode_getDesktopMode());

	mainWindow = sfRenderWindow_create(sfVideoMode_getDesktopMode(), "CataClysm", /*sfFullscreen*/ sfDefaultStyle, NULL);

	//Set OnRun Icon
	sfImage* icon = sfImage_createFromFile("Resources/Icon/card_back_Events.png");
	sfRenderWindow_setIcon(mainWindow, 700, 700, sfImage_getPixelsPtr(icon));

	while (sfRenderWindow_isOpen(mainWindow))
	{
		deltaTime = sfTime_asSeconds(sfClock_restart(timerClock));

		InputManager(mainWindow);

		UpdateManager(mainWindow, deltaTime);
		
		DisplayManager(mainWindow);
	}

	return 0;
}

#if _DEBUG
void main(void)
{
	WinMain(NULL, NULL, NULL, 0);
}
#endif