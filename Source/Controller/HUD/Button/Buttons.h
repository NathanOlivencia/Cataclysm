#ifndef _BUTTONS_H
#define _BUTTONS_H

typedef struct BUTTON
{
	sfFloatRect rect; // the button's hitbox
	int numberToReturn; // number returned by the button
	sfSprite* sprite; // explicit enough
	bool isWorking; // false : disables the button, true : activates it
	bool isDisplay;
}Button;

// changes the button's state
// if the button is active, disables it, else enables it
void SwitchButtonState(Button* button, bool isActive);

// Display or not the button
// If the button is desable, the button is automaticaly desactivated ( bool isWorking = false ).
void SwitchButtonDisplay(Button* button, bool isDisplay);

// detects which button is pressed and returns its value
// if none is, returns -1
int GetButtonValue(sfVector2i mousePosition, Button testedButton);

// detects which button is pressed and returns its value
// if none is, returns -1
int GetButtonsValue(sfVector2i mousePosition, Button testedButtons[], int numberButtons);

// creates a button
Button InitButton(sfVector2f size, sfVector2f buttonPos, int numberToReturn, char* pathOfSpriteToLoad, bool isCentered);

// display a button if he is enabled
void DisplayButtonHUD(sfRenderWindow* window, Button button);

// display buttons if they are enabled
void DisplayButtons(sfRenderWindow* window, Button buttons[], int numberSprites);

// changes the button's position and its hitbox
void ChangeButtonPosition(Button* button, sfVector2f newPosition);

#endif