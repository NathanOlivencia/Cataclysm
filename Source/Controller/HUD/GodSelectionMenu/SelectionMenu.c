#include "SelectionMenu.h"
#include <math.h>

typedef struct GodCardButton
{
	Button button;
	sfVector2f basePos;
}GodCardButton;

typedef struct GodCardSelected
{
	bool isSelected;
	sfSprite* sprite;
	sfText* dataTexts[4];
}GodCardSelected;

typedef struct
{
	int playersSelectedNumber;
	int starters[6][4];
	GodCardButton cardButtons[6];
	Button mainMenuButton;
	GodCardSelected cards[6];
	sfSprite* background;
	sfText* actualPlayerDisplay;
	sfText* title;
	sfVector2f pos;
	GodCards godsSelected[2];
	bool localReload;
}SelectionScene;

float timePass;
const float timeBeforeChangeScene = 1.0f;
float currentTime;

SelectionScene selectionScene;

// ----------------------------------------------------------------------------------------------
void InitSelectionScene(sfVideoMode mode, bool isReload)
{
	selectionScene.playersSelectedNumber = 0;

	float baseCardSizeX = 762;
	float baseCardSizeY = 1170;
	sfVector2f cardPos;
	sfVector2f cardSize;
	sfVector2f selectedCardSize;
	sfVector2f cardScale;
	sfVector2f cardSelectedPos;
	float offsetCardPosX;

	timePass = 0.0f;
	currentTime = 0.0f;

	cardScale.x = 300 / baseCardSizeX;
	cardScale.y = cardScale.x;

	cardSize.x = 250;
	cardSize.y = 360;

	selectedCardSize.x = baseCardSizeX * cardScale.x;
	selectedCardSize.y = baseCardSizeY * cardScale.y;

	offsetCardPosX = (mode.width - (cardSize.x * 6)) / 7;
	cardPos.x = offsetCardPosX;
	cardPos.y = (mode.height / 2) - (cardSize.y / 2) + 100.0f;

	if (isReload || selectionScene.localReload)
	{
		sfText_setString(selectionScene.actualPlayerDisplay, "Player 1");
		
		for (int i = 0; i < 6; i++)
		{
			selectionScene.cards[i].isSelected = false;
			selectionScene.cardButtons[i].button.isWorking = true;
			selectionScene.mainMenuButton.isWorking = true;
			selectionScene.cardButtons[i].button.isDisplay = true;

			cardSelectedPos.x = cardPos.x - 25;
			cardSelectedPos.y = cardPos.y + 100;

			sfVector2f textPos = { cardSelectedPos.x + 60 , cardSelectedPos.y + (selectedCardSize.y / 2) + 106 };
			sfText_setPosition(selectionScene.cards[i].dataTexts[0], textPos);
			textPos.y += 82.0f;
			sfText_setPosition(selectionScene.cards[i].dataTexts[1], textPos);
			textPos.x = cardSelectedPos.x + selectedCardSize.x - 80.0f;
			sfText_setPosition(selectionScene.cards[i].dataTexts[2], textPos);
			textPos.y -= 82.0f;
			sfText_setPosition(selectionScene.cards[i].dataTexts[3], textPos);
			cardPos.x += cardSize.x + offsetCardPosX;
		}

		return;
	}

	sfVector2f titlePos;
	sfVector2f playerDisplayPos;
	int fontSize = 15;
	currentTime = 0.0f;

	titlePos.x = mode.width / 2 - 450.0f;
	titlePos.y = 200.0f;

	playerDisplayPos.x = mode.width / 2 - 150.0f;
	playerDisplayPos.y = 100.0f;

	selectionScene.background = LoadSprite("Resources/Map/BackGround/Fond2.png", 0);

	selectionScene.mainMenuButton = InitButton((sfVector2f) { 300, 120 }, (sfVector2f) { 20, 20 }, 10, "Resources/HUD/mainSceneHUD/PauseMenu/MainMenu.png", false);

	selectionScene.title = InitText("Choose your Divinity", titlePos, sfWhite, 80);
	sfText_setOutlineColor(selectionScene.title, (sfColor) { 42, 155, 204, 255 });
	sfText_setOutlineThickness(selectionScene.title, 12.0f);

	selectionScene.actualPlayerDisplay = InitText("Player 1", playerDisplayPos, sfWhite, 60);
	sfText_setOutlineColor(selectionScene.actualPlayerDisplay, (sfColor) { 42, 155, 204, 255 });
	sfText_setOutlineThickness(selectionScene.actualPlayerDisplay, 10.0f);

	int starters[6][4] =
	{
		{ 50, 120, 150, 60 },
		{ 40, 100, 100, 20 },
		{ 45, 110, 120, 75 },
		{ 60, 90, 110, 10 },
		{ 70, 115, 150, 15 },
		{ 35, 70, 80, 50 }
	};

	char* godFileDirectory[6] = 
	{ 
		"Resources/Gods/Amateratsu v2.png", "Resources/Gods/Aphrodite v2.png", "Resources/Gods/Kukulkan v2.png",
		"Resources/Gods/Odin v2.png" ,"Resources/Gods/Osiris v2.png" ,"Resources/Gods/Shiva v2.png" 
	};

	char* choosenGodFileDirectory[6] = 
	{ 
		"Resources/Gods/Amateratsu-Start v2.png", "Resources/Gods/Aphrodite-Start v2.png", "Resources/Gods/Kukulkan-Start v2.png",
		"Resources/Gods/Odin-Start v2.png", "Resources/Gods/Osiris-Start v2.png", "Resources/Gods/Shiva-Start v2.png", 
	};


	for (int i = 0; i < 6; i++)
	{
		selectionScene.cardButtons[i].basePos = cardPos;
		selectionScene.cardButtons[i].button = InitButton(cardSize, cardPos, i, godFileDirectory[i], 0);
		selectionScene.cards[i].sprite = LoadSprite(choosenGodFileDirectory[i], 0);

		sfSprite_setScale(selectionScene.cards[i].sprite, cardScale);

		cardSelectedPos.x = cardPos.x - 25;
		cardSelectedPos.y = cardPos.y + 100;
	
		sfSprite_setPosition(selectionScene.cards[i].sprite, cardSelectedPos);

		selectionScene.cards[i].isSelected = false;

		sfVector2f textPos = { cardSelectedPos.x + 60 , cardSelectedPos.y + (selectedCardSize.y / 2) + 106};
		selectionScene.cards[i].dataTexts[0] = InitText("", textPos, sfWhite, fontSize);

		textPos.y += 82.0f;
		selectionScene.cards[i].dataTexts[1] = InitText("", textPos, sfWhite, fontSize);

		textPos.x = cardSelectedPos.x + selectedCardSize.x - 80.0f;
		selectionScene.cards[i].dataTexts[2] = InitText("", textPos, sfWhite, fontSize);

		textPos.y -= 82.0f;
		selectionScene.cards[i].dataTexts[3] = InitText("", textPos, sfWhite, fontSize);

		cardPos.x += cardSize.x + offsetCardPosX;

		for (int j = 0; j < 4; j++)
		{
			selectionScene.starters[i][j] = starters[i][j];

			sfText_setOutlineColor(selectionScene.cards[i].dataTexts[j], (sfColor) { 0, 0, 0, 10 });
			sfText_setOutlineThickness(selectionScene.cards[i].dataTexts[j], 2);
		}
	}

	selectionScene.localReload = true;
}

// ----------------------------------------------------------------------------------------------
int GetGodSelectionButtonsIndex(sfVector2i mousePos)
{
	int value = -1; ;
	for (int i = 0; i < 6; i++)
	{
		int temp = GetButtonValue(mousePos, selectionScene.cardButtons[i].button);
		if (temp != -1)
		{
			value = temp;
		}
	}
	if (value == -1)
	{
		value = GetButtonValue(mousePos, selectionScene.mainMenuButton);
	}
	return value;
}

bool CheckSelectionIsFinished()
{
	return(selectionScene.playersSelectedNumber == 2);
}

// ----------------------------------------------------------------------------------------------
void GetGodPlayer(int godIndex)
{
	if (selectionScene.playersSelectedNumber == 2)
	{
		return;
	}
	
	selectionScene.playersSelectedNumber++;
	if (selectionScene.playersSelectedNumber < 2)
	{
		UpdateTextWithStringDisplay(selectionScene.actualPlayerDisplay, "Player", selectionScene.playersSelectedNumber + 1);
	}
	else
	{
		selectionScene.mainMenuButton.isWorking = false;
	}

	int index = rand() % 6;

	selectionScene.cards[godIndex].isSelected = true;
	SwitchButtonDisplay(&selectionScene.cardButtons[godIndex], false);

	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].enumToString = "";

	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].godID = godIndex;
	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].habitantStart = selectionScene.starters[index][0];
	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].foodStart = selectionScene.starters[index][1];
	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].waterStart = selectionScene.starters[index][2];
	selectionScene.godsSelected[selectionScene.playersSelectedNumber - 1].materialsStart = selectionScene.starters[index][3];
	
	UpdateTextsDisplay(selectionScene.cards[godIndex].dataTexts, selectionScene.starters[index], 4);

	for (int i = 0; i < 4; i++)
	{
		if (selectionScene.starters[index][i] >= 100)
		{
			sfVector2f currentTextPos = sfText_getPosition(selectionScene.cards[godIndex].dataTexts[i]);
			currentTextPos.x -= 5.0f;
			sfText_setPosition(selectionScene.cards[godIndex].dataTexts[i], currentTextPos);
		}
	}
}

// ----------------------------------------------------------------------------------------------
int* GetGodDatas(int playerID)
{
	int* datas;

	datas = malloc(5 * sizeof(int));

	datas[0] = playerID;
	datas[1] = selectionScene.godsSelected[playerID].habitantStart;
	datas[2] = selectionScene.godsSelected[playerID].foodStart;
	datas[3] = selectionScene.godsSelected[playerID].waterStart;
	datas[4] = selectionScene.godsSelected[playerID].materialsStart;

	return datas;
}

// ----------------------------------------------------------------------------------------------
int getGodID(int playerID)
{
	return selectionScene.godsSelected[playerID].godID;
}

// ----------------------------------------------------------------------------------------------
void GodHoverAnimation(sfVector2i mousePosition, float deltaTime)
{
	if (selectionScene.playersSelectedNumber < 2)
	{
		for (int i = 0; i < 6; i++)
		{
			int tmp = GetButtonValue(mousePosition, selectionScene.cardButtons[i].button);
			sfVector2f currentButtonPos = sfSprite_getPosition(selectionScene.cardButtons[i].button.sprite);
			if (tmp != -1)
			{
				timePass += deltaTime;
				currentButtonPos.y = sinf(timePass * 4) * 15.0f + selectionScene.cardButtons[i].basePos.y + 15.0f;
				sfSprite_setPosition(selectionScene.cardButtons[i].button.sprite, currentButtonPos);
			}
			else
			{
				if (currentButtonPos.y != selectionScene.cardButtons[i].basePos.y)
				{
					timePass = 0.0f;
					ChangeButtonPosition(&selectionScene.cardButtons[i].button, selectionScene.cardButtons[i].basePos);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------
void UpdateGodSelectionMenu(void(*cbData)(int*), void(*cbSetGodHUD)(int, int), float deltatime)
{
	if (selectionScene.playersSelectedNumber == 2)
	{
		if (currentTime < timeBeforeChangeScene)
		{
			currentTime += deltatime;
		}
		else
		{
			cbData(GetGodDatas(0));
			cbData(GetGodDatas(1));
			cbSetGodHUD(0, selectionScene.godsSelected[0].godID);
			cbSetGodHUD(1, selectionScene.godsSelected[1].godID);
		}
	}
}

// ----------------------------------------------------------------------------------------------
void DisplaySelectionScene(sfRenderWindow* window)
{
	sfRenderWindow_drawSprite(window, selectionScene.background, NULL);
	sfRenderWindow_drawText(window, selectionScene.actualPlayerDisplay, NULL);
	sfRenderWindow_drawText(window, selectionScene.title, NULL);

	DisplayButtonHUD(window, selectionScene.mainMenuButton);

	for (int i = 0; i < 6; i++)
	{
		DisplayButtonHUD(window, selectionScene.cardButtons[i].button);
		if (selectionScene.cards[i].isSelected)
		{
			sfRenderWindow_drawSprite(window, selectionScene.cards[i].sprite, NULL);

			for (int j = 0; j < 4; j++)
			{
				sfRenderWindow_drawText(window, selectionScene.cards[i].dataTexts[j], NULL);
			}
		}
	}
}