#include "DiceSimulator.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"
#include "../Text/Text.h"

sfSprite* diceFace[6];
int currentDiceFace;
float displayTime;
float currentTime;
bool isDisplayingFinalDice;
int finalDiceFace;

// -------------------------------------------------------------------------------------------------------------------
void initDice(sfVideoMode mode)
{
	sfVector2f position = { mode.width / 2.0f, mode.height / 2.0f };
	
	isDisplayingFinalDice = false;

	for (int i = 0; i < 6; i++)
	{
		char fileDirectory[100];
		sprintf(fileDirectory, "%s%d%s", "./Resources/HUD/mainSceneHUD/Dice/", i + 1, ".png");
		diceFace[i] = LoadSprite(fileDirectory, 1);
		sfSprite_setPosition(diceFace[i], position);
	}
	finalDiceFace = -1;
	currentDiceFace = -1;
}

// -------------------------------------------------------------------------------------------------------------------
void LaunchDice()
{
	currentDiceFace = rand() % 6;
	displayTime = 0.2f;
	currentTime = 0.0f;
	isDisplayingFinalDice = false;
}

// -------------------------------------------------------------------------------------------------------------------
bool IsFinalFace()
{
	return isDisplayingFinalDice && currentTime >= displayTime;
}

void setDiceValue(int value)
{
	finalDiceFace = value;
}

// -------------------------------------------------------------------------------------------------------------------
void UpdateDice(float deltaTime)
{
	if (currentTime <= displayTime)
	{
		currentTime += deltaTime;
	}
	else
	{
		if (isDisplayingFinalDice)
		{
			sfSprite_setScale(diceFace[currentDiceFace], (sfVector2f) { 1.0f, 1.0f });
			
			return;
		}

		int tmpValue = rand() % 6;
		if (tmpValue == currentDiceFace)
		{
			if (tmpValue > 0 && tmpValue < 5)
			{
				currentDiceFace = tmpValue + 1;
			}
			else if (tmpValue == 0)
			{
				currentDiceFace = 1 + rand() % 5;
			}
			else
			{
				currentDiceFace = rand() % 5;
			}
		}
		else
		{
			currentDiceFace = tmpValue;
		}

		displayTime += 0.4f;
		currentTime = 0.0f;

		if (displayTime >= 2.0f)
		{
			isDisplayingFinalDice = true;
			currentDiceFace = finalDiceFace - 1;
			finalDiceFace = -1;
			sfSprite_setScale(diceFace[currentDiceFace], (sfVector2f) { 1.2f, 1.2f });
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------
void DisplayDice(sfRenderWindow* window)
{
	sfRenderWindow_drawSprite(window, diceFace[currentDiceFace], NULL);
}