#include <SFML\Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Buttons.h"

void SwitchButtonState(Button* button, bool isActive)
{
	sfColor enableColor = { 255, 255, 255, 255 };
	sfColor desableColor = { 130, 130, 130, 220 };

	button->isWorking = isActive;
	
	if (button->sprite != NULL)
	{
		if (button->isWorking)
		{	
			sfSprite_setColor(button->sprite, enableColor);
		}
		else
		{
			sfSprite_setColor(button->sprite, desableColor);
		}
	}
}

void SwitchButtonDisplay(Button* button, bool isDisplay)
{
	button->isDisplay = isDisplay;
	if (!isDisplay)
	{
		button->isWorking = false;
	}
}

int GetButtonValue(sfVector2i mousePosition, Button testedButton)
{
	int valueToReturn = -1; // the function will return -1 if the buttons is not pressed

	if (testedButton.isWorking && sfFloatRect_contains(&testedButton.rect, (float)mousePosition.x, (float)mousePosition.y)) // if the button is activated
	{
		valueToReturn = testedButton.numberToReturn; // gets the value of the pressed button
	}

	return valueToReturn;
}

int GetButtonsValue(sfVector2i mousePosition, Button testedButtons[], int numberButtons)
{
	int valueToReturn = -1; // the function will return -1 if the buttons is not pressed
	for (int i = 0; i < numberButtons; i++) // chacks all buttons of the array
	{
		valueToReturn = GetButtonValue(mousePosition, testedButtons[i]);
		if (valueToReturn != -1)
		{
			return valueToReturn;
		}
	}
	return valueToReturn;
}

Button InitButton(sfVector2f size, sfVector2f buttonPos, int numberToReturn, char* pathOfSpriteToLoad, bool isCentered) // creates a button
{
	Button button; // the created button
	sfVector2f scale; // the button's scale
	sfTexture* texture; // the button's texture

	if (pathOfSpriteToLoad != NULL)
	{
		texture = sfTexture_createFromFile(pathOfSpriteToLoad, NULL); // load the button's texture
		sfTexture_setSmooth(texture, true);
	}
	else
	{
		button.isWorking = true;
		button.isDisplay = false;

		button.sprite = NULL;

		button.rect.top = buttonPos.y;
		button.rect.left = buttonPos.x;
		button.rect.height = size.y;
		button.rect.width = size.x;

		button.numberToReturn = numberToReturn;

		return button;
	}

	button.sprite = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(button.sprite, texture, 0);
	// the scale's ratio (to rescale the texture according to the button's size)
	scale.x = size.x / (float)sfTexture_getSize(texture).x;
	scale.y = size.y / (float)sfTexture_getSize(texture).y;

	sfSprite_setScale(button.sprite, scale);

	if (isCentered)
	{
		sfVector2f center = { (float)sfTexture_getSize(texture).x / 2, (float)sfTexture_getSize(texture).y / 2 }; // get the texture's center
		sfSprite_setOrigin(button.sprite, center);
	}

	sfSprite_setPosition(button.sprite, buttonPos);

	button.rect = sfSprite_getGlobalBounds(button.sprite); // sets the button's hitbox

	button.isWorking = true; // activates the button
	button.isDisplay = true; // the button can be display
	button.numberToReturn = numberToReturn; // sets the button's value

	return button;
}

void DisplayButtonHUD(sfRenderWindow* window, Button button)
{
	if (button.isDisplay && button.sprite != NULL)
	{
		sfRenderWindow_drawSprite(window, button.sprite, NULL);
	}
}

void DisplayButtons(sfRenderWindow* window, Button buttons[], int numberSprites)
{
	for (int i = 0; i < numberSprites; i++)
	{
		DisplayButtonHUD(window, buttons[i]);
	}
}

void ChangeButtonPosition(Button* button, sfVector2f newPosition)
{
	sfSprite_setPosition(button->sprite, newPosition); // reset the button's position
	button->rect = sfSprite_getGlobalBounds(button->sprite); // reset the button's hitbox
}