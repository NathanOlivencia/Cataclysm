#ifndef _MENU_H
#define _MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../Button/Buttons.h"

typedef struct 
{
	sfSprite* background;
	sfSprite* title;
	sfSprite** images;
	Button* button;
	int imagesNumber;
	int buttonNumber;
	sfVector2f Position;
	bool isActive;
}Menu;

Menu CreateGameMenu(sfVector2f menuPosition, Button* buttons, int buttonNumber, int imagesNumber, bool isCentered);
void AddBackgroundToMenu(Menu* menu, char* fileDirectory, sfVector2f position, bool isCentered);
void AddTitleToMenu(Menu* menu, char* fileDirectory, sfVector2f position, bool isCentered);
void AddImageToMenu(Menu* menu, char* fileDirectory, int arrayIndex, sfVector2f position, bool isCentered);
void AddImagesToMenu(Menu* menu, char* fileDirectories[], sfVector2f positions[], bool isCentered);
void AlignMenuButtons(Menu* menu, int firstButtonIndex, sfVector2f initPosition, float offset, int offsetDir);
void UpdateMenuPosition(Menu* menu, sfVector2f Position);
void DisplayMenu(Menu menu, sfRenderWindow *window);
Menu CreateMainMenuProto(sfVideoMode mode);
Menu CreateMainSceneHUD(sfVideoMode mode);
Menu CreateExpMenuManagement(sfVideoMode mode);
Menu CreateBuildMenuManagement(sfVideoMode mode);
Menu CreateBuildersMenuManagement(sfVideoMode mode);
Menu CreatePauseMenu(sfVideoMode mode);
Menu CreateGameOverMenu(sfVideoMode mode);
Menu CreateGameWonMenu(sfVideoMode mode);
#endif// !HEADER_MENU
