#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "Text.h"


sfText* InitText(char* message, sfVector2f position, sfColor color, int size)
{
	sfFont* font = sfFont_createFromFile("./Resources/Font/ELEPHNT.TTF");
	sfText *textToInit = NULL;
	textToInit = sfText_create();
	sfText_setFont(textToInit, font);
	sfText_setString(textToInit, message);
	sfText_setPosition(textToInit, position);
	sfText_setFillColor(textToInit, color);
	sfText_setCharacterSize(textToInit, size);
	sfText_setStyle(textToInit, sfTextRegular);
	return(textToInit);
}

void UpdateTextDisplay(sfText* text, int dataTribe)
{
	char msgToDisplay[12];
	sprintf(msgToDisplay, "%d", dataTribe);
	sfText_setString(text, msgToDisplay);
}

void UpdateTextsDisplay(sfText** text, int dataTribe[], int index)
{
	for (int i = 0; i < index; i++)
	{
		char msgToDisplay[12];
		sprintf(msgToDisplay, "%d", dataTribe[i]);
		sfText_setString(text[i], msgToDisplay);
	}
}

void UpdateTextWithStringDisplay(sfText* text, char* msg, int data)
{
	char msgToDisplay[50];
	sprintf(msgToDisplay, "%s %d", msg, data);
	sfText_setString(text, msgToDisplay);
}

void UpdateTextWithSeparator(sfText* text, int data1, char* separator, int data2)
{
	char msgToDisplay[50];
	sprintf(msgToDisplay, "%d %s %d", data1, separator, data2);
	sfText_setString(text, msgToDisplay);
}