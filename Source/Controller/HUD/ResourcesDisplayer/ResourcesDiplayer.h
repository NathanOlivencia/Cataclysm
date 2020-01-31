#ifndef _ResourcesDisplayer_H
#define _ResourcesDisplayer_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>

typedef struct ModifDisplayer
{
	sfText** texts;
	//--
	int* values;
	//--
	int valuesNumber;
	float activeTime;
	bool isActive;
}ModifDisplayer;

typedef struct ResourcesElement
{
	sfSprite* image;
	sfText* text;
	sfText* previewText;
	sfVector2f startOffset;
	//--
	int dataValue;
	float textOffset;
	
}ResourcesElement;

typedef struct ResourcesBar
{
	ResourcesElement* resourcesElements;
	ModifDisplayer modifDisplayers[6];
	//--
	int elementsNumber;
	int modifDisplayerNumber;
	int modifDisplayerActive;
	//--
	float displayTime;
	//--
	sfFloatRect hitBox;
	bool hasFinishShowDisplay;
	bool haveToShowPreview;
}ResourcesBar;

ResourcesElement* createResourcesElements(char* imageFileDirectory[], int data[], int elementNumber, sfVector2f startOffset, float textOffset);
ResourcesBar createResourcesBar(ResourcesElement resourcesElement[], int elementNumber, sfVector2f firstElementPos, sfVector2f resourceElementSize, int direction);
void UpdateResourcesBar(ResourcesBar* resBar, float deltaTime);
void DisplayResourcesBar(sfRenderWindow* window, ResourcesBar* resourcesBar);
void ConsumptionPreview(ResourcesBar* resourcesBar, sfVector2i mousePos, int barID);
void UpdateResourcesBarPreview(ResourcesBar* resourcesBar, int* newData);
void UpdateResourcesBarInfos(ResourcesBar* resourcesBar, int* newInfos);
bool HasFinishToDisplayEndTurnChange(ResourcesBar resourcesBar[], int resourcesBarNumber);
void InitResourceBarWithData(ResourcesBar* rBar, int *data);
ResourcesBar InfoBarProto(int ID, int* dataInfos);
#endif // !_ResourcesDisplayer_H
