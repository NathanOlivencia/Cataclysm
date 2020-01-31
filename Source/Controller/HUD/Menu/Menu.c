#include "Menu.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"

Menu CreateGameMenu(sfVector2f menuPosition, Button* buttons, int buttonNumber, int imagesNumber, bool isCentered)
{
	Menu menu;

	menu.isActive = true;
	menu.imagesNumber = imagesNumber;
	menu.buttonNumber = buttonNumber;
	menu.background = NULL;
	menu.title = NULL;

	menu.images = malloc(sizeof(sfSprite*) * imagesNumber);

	menu.button = malloc(sizeof(Button) * buttonNumber);

	for (int i = 0; i < buttonNumber; i++)
	{
		menu.button[i] = buttons[i];
	}
	 
	menu.Position = menuPosition;

	return menu;
}

void AddBackgroundToMenu(Menu* menu, char* fileDirectory, sfVector2f position, bool isCentered)
{
	sfTexture* texture = sfTexture_createFromFile(fileDirectory, NULL);
	menu->background = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(menu->background, texture, true);

	if (isCentered)
	{
		sfVector2u imageSize = sfTexture_getSize(texture);
		sfVector2f center = { imageSize.x / 2.0f, imageSize.y / 2.0f };
		sfSprite_setOrigin(menu->background, center);
	}

	sfSprite_setPosition(menu->background, position);
}

void AddTitleToMenu(Menu* menu, char* fileDirectory, sfVector2f position, bool isCentered)
{
	sfTexture* texture = sfTexture_createFromFile(fileDirectory, NULL);
	menu->title = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(menu->title, texture, true);

	if (isCentered)
	{
		sfVector2u imageSize = sfTexture_getSize(texture);
		sfVector2f center = { imageSize.x / 2.0f, imageSize.y / 2.0f };
		sfSprite_setOrigin(menu->title, center);
	}

	sfSprite_setPosition(menu->title, position);
}

void AddImageToMenu(Menu* menu, char* fileDirectory, int arrayIndex, sfVector2f position, bool isCentered)
{
	menu->images[arrayIndex] = LoadSprite(fileDirectory, isCentered);
	sfSprite_setPosition(menu->images[arrayIndex], position);
}

void AddImagesToMenu(Menu* menu, char* fileDirectories[], sfVector2f positions[], bool isCentered)
{
	for (int i = 0; i < menu->imagesNumber; i++)
	{
		menu->images[i] = LoadSprite(fileDirectories[i], isCentered);
		sfSprite_setPosition(menu->images[i], positions[i]);
	}
}

void AlignMenuButtons(Menu* menu, int firstButtonIndex, sfVector2f initPosition, float offset, int offsetDir)
{
	sfVector2f newposition;
	ChangeButtonPosition(&menu->button[firstButtonIndex], initPosition);
	for (int i = firstButtonIndex + 1; i < menu->buttonNumber; i++)
	{
		if (offsetDir == 1)
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width /2  + menu->button[i].rect.width / 2 + offset;
			newposition.y = initPosition.y;
		}
		else if (offsetDir == 2)
		{
			newposition.x = initPosition.x;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2 + menu->button[i].rect.height / 2 + offset;
		}
		else
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width / 2 + menu->button[i].rect.width / 2 + offset;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2 + menu->button[i].rect.height / 2 + offset;
		}

		ChangeButtonPosition(&menu->button[i], newposition);
	}
}

void UpdateMenuPosition(Menu* menu, sfVector2f Position)
{
	for (int i = 0; i < menu->buttonNumber; i++)
	{
		ChangeButtonPosition(&menu->button[i], Position);
	}
	
	sfSprite_setPosition(menu->background, Position);
}

void DisplayMenu(Menu menu, sfRenderWindow *window)
{
	if (menu.background != NULL)
	{
		sfRenderWindow_drawSprite(window, menu.background, NULL);
	}
	if (menu.title != NULL)
	{
		sfRenderWindow_drawSprite(window, menu.title, NULL);
	}
	if (menu.imagesNumber > 0)
	{
		for (int i = 0; i < menu.imagesNumber; i++)
		{
			sfRenderWindow_drawSprite(window, menu.images[i], NULL);
		}
	}

	DisplayButtons(window, menu.button, menu.buttonNumber);
}

Menu CreateMainMenuProto(sfVideoMode mode)
{
	Button MainMenuButtons[4];
	sfVector2f MainMenuButtonsPos = { 400, 400 };
	sfVector2f MainMenuButtonsSize = { 620, 120 };
	char* filesDirectories[5] = 
	{ 
		"./Resources/HUD/HomeMenu/buttonPlay.png",
		"./Resources/HUD/HomeMenu/buttonRules.png",
		"./Resources/HUD/HomeMenu/buttonCredits.png",
		"./Resources/HUD/HomeMenu/buttonQuit.png",
		"./Resources/HUD/HomeMenu/title.png" 
	};

	for (int i = 0; i < 4; i++)
	{
		if (i > 1)
		{
			MainMenuButtonsPos.x = 300;
			MainMenuButtonsPos.y = 600;
			MainMenuButtonsSize.x = 300;
			MainMenuButtonsSize.y = 120;
		}
		MainMenuButtons[i] = InitButton(MainMenuButtonsSize, MainMenuButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f origin = { 0.0f, 0.0f };
	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f };
	sfVector2f posSecond = { 800.0f, 880.0f };
	sfVector2f titlePos = { mode.width / 2.0f, 200.0f };

	Menu MainMenu = CreateGameMenu( origin, MainMenuButtons, 4, 0, false);
	AddBackgroundToMenu(&MainMenu, "./Resources/HUD/HomeMenu/Forto.png", (sfVector2f) { mode.width / 2.0f, mode.height / 2.0f }, true);
	AlignMenuButtons(&MainMenu, 0, posFirst, 30.f, 2);
	AlignMenuButtons(&MainMenu, 2, posSecond, 20.f, 1);
	AddTitleToMenu(&MainMenu, filesDirectories[4], titlePos, true);
	return (MainMenu);
}

Menu CreateMainSceneHUD(sfVideoMode mode)
{
	
	Button MainMenuButtons[11];
	char* containerFile = "./Resources/HUD/mainSceneHUD/globalSceneHUD/barButtons.png";
	char* frameFile = "./Resources/HUD/mainSceneHUD/globalSceneHUD/frame.png";
	sfVector2f menuOrigin = { 0.0f, 0.0f };
	sfVector2f containersPos[2] = 
	{
		{10.0f, mode.height - 115.f},
		{ mode.width - (355.0f + 8.0f),  mode.height - 115.f }
	};
	
	// RESOURCES EXPEDITION
	MainMenuButtons[0] = InitButton((sfVector2f) { 355.0f, 105.0f }, (sfVector2f) { (mode.width / 2) - 455.0f, 950.0f }, 0, "./Resources/HUD/mainSceneHUD/globalSceneHUD/exploreButton.png", 0);
	// KNOLEDGES EXPEDITION
	MainMenuButtons[1] = InitButton((sfVector2f) { 355.0f, 105.0f }, (sfVector2f) { (mode.width / 2) + 100.0f, 950.0f }, 1, "./Resources/HUD/mainSceneHUD/globalSceneHUD/learnButton.png", 0);
	// DONATION
	MainMenuButtons[2] = InitButton((sfVector2f) { 250.0f, 200.0f }, (sfVector2f) { 180.0f, 580.0f } , 2, NULL, 0);
	MainMenuButtons[3] = InitButton((sfVector2f) { 250.0f, 200.0f }, (sfVector2f) { mode.width - 250.0f - 180.0f, 580.0f }, 2, NULL, 0);
	// FORTRESS
	MainMenuButtons[4] = InitButton((sfVector2f) { 480.0f, 480.0f }, (sfVector2f) { (mode.width / 2) - 250.0f, 80.0f }, 3, NULL, 0);
	// BUILD
	MainMenuButtons[5] = InitButton((sfVector2f) { 85.0f, 85.0f }, (sfVector2f) { 130.0f + 85.0f, mode.height - 130.0f}, 4, "./Resources/HUD/mainSceneHUD/BuildPanel/build.png", 0);
	MainMenuButtons[6] = InitButton((sfVector2f) { 85.0f, 85.0f }, (sfVector2f) { mode.width - 130.0f - (85.0f *2), mode.height - 130.0f }, 4, "./Resources/HUD/mainSceneHUD/BuildPanel/build.png", 0);
	// DIVINE FAVOR
	MainMenuButtons[7] = InitButton((sfVector2f) { 300.0f, 350.0f }, (sfVector2f) { 150.0f, 150.0f }, 5, NULL, 0);
	MainMenuButtons[8] = InitButton((sfVector2f) { 300.0f, 350.0f }, (sfVector2f) { mode.width - 150.0f - 300.f, 150.0f }, 5, NULL, 0);
	// MANAGE RESOURCES EXPEDITIONS
	MainMenuButtons[9] = InitButton((sfVector2f) { 85.0f, 85.0f }, (sfVector2f) { 100.0f, mode.height - 130.0f }, 6, "./Resources/HUD/mainSceneHUD/ExpeditionPanel/expedition.png", 0);
	MainMenuButtons[10] = InitButton((sfVector2f) { 85.0f, 85.0f }, (sfVector2f) {mode.width - (100.0f + 85.0f), mode.height - 130.0f }, 6, "./Resources/HUD/mainSceneHUD/ExpeditionPanel/expedition.png", 0);

	MainMenuButtons[7].isWorking = false;
	MainMenuButtons[8].isWorking = false;
	
	Menu MainMenu = CreateGameMenu(menuOrigin, MainMenuButtons, 11, 0, false);
	AddBackgroundToMenu(&MainMenu, frameFile, menuOrigin, 0);

	return(MainMenu);
}

Menu CreateExpMenuManagement(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button MainMenuButtons[2];

	char* filesDirectories[2] =
	{
		"./Resources/HUD/mainSceneHUD/ExpeditionPanel/buttonContinue.png",
		"./Resources/HUD/mainSceneHUD/ExpeditionPanel/buttonComeBack.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 240.0f, 100.0f };
	for (int i = 0; i < 2; i++)
	{
		sfVector2f posButtons = { mode.width / 2.0f , mode.height / 2.0f + i * 150.0f };

		MainMenuButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 1);
	}
	
	toReturnMenu = CreateGameMenu(origin, MainMenuButtons, 2, 0, false);

	toReturnMenu.isActive = false;

	return (toReturnMenu);
}

Menu CreateBuildMenuManagement(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button MainMenuButtons[2];

	char* filesDirectories[2] =
	{
		"./Resources/HUD/mainSceneHUD/BuildPanel/buttonBuild.png",
		"./Resources/HUD/mainSceneHUD/BuildPanel/buttonCancel.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 240.0f, 100.0f };
	for (int i = 0; i < 2; i++)
	{
		sfVector2f posButtons = { mode.width / 2.0f , mode.height / 2.0f + i * 150.0f };

		MainMenuButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 1);
	}

	toReturnMenu = CreateGameMenu(origin, MainMenuButtons, 2, 0, false);

	toReturnMenu.isActive = false;

	return (toReturnMenu);
}

Menu CreateBuildersMenuManagement(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button MainMenuButtons[2];

	char* filesDirectories[2] =
	{
		"./Resources/HUD/mainSceneHUD/FortressPanel/buttonComeBack.png",
		"./Resources/HUD/mainSceneHUD/FortressPanel/buttonContinue.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 240.0f, 100.0f };
	for (int i = 0; i < 2; i++)
	{
		sfVector2f posButtons = { mode.width / 2.0f , mode.height / 2.0f + i * 150.0f };

		MainMenuButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 1);
	}

	toReturnMenu = CreateGameMenu(origin, MainMenuButtons, 2, 0, false);

	toReturnMenu.isActive = false;

	return (toReturnMenu);
}

Menu CreatePauseMenu(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button pauseButtons[3];

	char* filesDirectories[3] =
	{
		"./Resources/HUD/mainSceneHUD/PauseMenu/buttonContinue.png",
		"./Resources/HUD/mainSceneHUD/PauseMenu/MainMenu.png",
		"./Resources/HUD/mainSceneHUD/PauseMenu/buttonQuit.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 300.0f, 120.0f };
	for (int i = 0; i < 3; i++)
	{
		sfVector2f posButtons = { mode.width / 2.0f , mode.height / 2.0f - 150.0f + i * 150.0f };

		pauseButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 1);
	}

	sfVector2f zero = { 0.0f, 0.0f };
	toReturnMenu = CreateGameMenu(origin, pauseButtons, 3, 0, false);
	AddBackgroundToMenu(&toReturnMenu, "./Resources/HUD/mainSceneHUD/PauseMenu/pause.png", zero, 0);
	
	toReturnMenu.isActive = false;

	return (toReturnMenu);
}

Menu CreateGameOverMenu(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button gameOverButtons[2];

	char* filesDirectories[2] =
	{
		"./Resources/HUD/mainSceneHUD/GameOverMenu/MainMenu.png",
		"./Resources/HUD/mainSceneHUD/GameOverMenu/buttonQuit.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 300.0f, 120.0f };
	for (int i = 0; i < 2; i++)
	{
		sfVector2f posButtons = { origin.x + i* 92.0f + (1-i) * -(ButtonSize.x +92.0f), 690.0f };

		gameOverButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 0);
	}

	sfVector2f zero = { 0.0f, 0.0f };
	toReturnMenu = CreateGameMenu(origin, gameOverButtons, 2, 0, false);
	AddBackgroundToMenu(&toReturnMenu, "./Resources/HUD/mainSceneHUD/GameOverMenu/gameOverScreen.png", zero, 0);

	toReturnMenu.isActive = false;

	return (toReturnMenu);
}

Menu CreateGameWonMenu(sfVideoMode mode)
{
	Menu toReturnMenu;
	Button gameOverButtons[2];

	char* filesDirectories[2] =
	{
		"./Resources/HUD/mainSceneHUD/GameWonMenu/MainMenu.png",
		"./Resources/HUD/mainSceneHUD/GameWonMenu/buttonQuit.png"
	};

	sfVector2f origin = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f ButtonSize = { 300.0f, 120.0f };
	for (int i = 0; i < 2; i++)
	{
		sfVector2f posButtons = { origin.x + i* 92.0f + (1 - i) * -(ButtonSize.x + 92.0f), 690.0f };

		gameOverButtons[i] = InitButton(ButtonSize, posButtons, i, filesDirectories[i], 0);
	}

	sfVector2f zero = { 0.0f, 0.0f };
	toReturnMenu = CreateGameMenu(origin, gameOverButtons, 2, 0, false);
	AddBackgroundToMenu(&toReturnMenu, "./Resources/HUD/mainSceneHUD/GameWonMenu/gameWon.png", zero, 0);

	toReturnMenu.isActive = false;

	return (toReturnMenu);
}