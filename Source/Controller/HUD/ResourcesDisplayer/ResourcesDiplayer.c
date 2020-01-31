#include "ResourcesDiplayer.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"
#include "../Text/Text.h"

void RemoveChangeText(ResourcesBar* resourcesBar, int startIndex)
{
	for (int i = 0; i < resourcesBar->elementsNumber; i++)
	{
		if (resourcesBar->modifDisplayers[startIndex].values[i] != 0)
		{
			resourcesBar->resourcesElements[i].dataValue += resourcesBar->modifDisplayers[startIndex].values[i];
			UpdateTextDisplay(resourcesBar->resourcesElements[i].text, resourcesBar->resourcesElements[i].dataValue);
		}
	}
	
	for (int i = 0; i < resourcesBar->modifDisplayerActive; i++)
	{
		resourcesBar->modifDisplayers[i].activeTime = 0.0f;
		resourcesBar->modifDisplayers[i].isActive = false;
		for (int j = 0; j < resourcesBar->elementsNumber; j++)
		{
			resourcesBar->modifDisplayers[i].values[j] = 0;
		}
		
		if (resourcesBar->modifDisplayers[i + 1].isActive)
		{
			resourcesBar->modifDisplayers[i].activeTime = resourcesBar->modifDisplayers[i + 1].activeTime;
			resourcesBar->modifDisplayers[i].isActive = true;

			for (int j = 0; j < resourcesBar->elementsNumber; j++)
			{
				resourcesBar->modifDisplayers[i].values[j] = resourcesBar->modifDisplayers[i + 1].values[j];
				sfText_setFillColor(resourcesBar->modifDisplayers[i].texts[j], sfText_getFillColor(resourcesBar->modifDisplayers[i + 1].texts[j]));
				UpdateTextDisplay(resourcesBar->modifDisplayers[i].texts[j], resourcesBar->modifDisplayers[i].values[j]);
			}
		}
	}

	resourcesBar->modifDisplayerActive--;
}

ResourcesElement* createResourcesElements(char* imageFileDirectory[], int data[], int elementNumber, sfVector2f startOffset, float textOffset)
{
	ResourcesElement *tmpResourcesElements;
	sfFont* font;
	sfColor previewColor = {186, 105, 0, 255};
	tmpResourcesElements = malloc(sizeof(ResourcesElement)* elementNumber);
	font = sfFont_createFromFile("./Resources/Font/Action of the Time II.ttf");

	for (int i = 0; i < elementNumber; i++)
	{
		tmpResourcesElements[i].image = LoadSprite(imageFileDirectory[i], 0);
		tmpResourcesElements[i].dataValue = data[i];
		tmpResourcesElements[i].text = InitText("", startOffset, sfBlack, 28);
		tmpResourcesElements[i].previewText = InitText("", startOffset, previewColor, 30);
		UpdateTextDisplay(tmpResourcesElements[i].text, data[i]);
		tmpResourcesElements[i].startOffset = startOffset;
		tmpResourcesElements[i].textOffset = textOffset;
	}
	return tmpResourcesElements;
}

ResourcesBar createResourcesBar(ResourcesElement resourcesElement[], int elementNumber, sfVector2f firstElementPos, sfVector2f resourceElementSize, int direction)
{
	ResourcesBar tmpResourcesBar;
	sfVector2f currentElementPos;
	sfVector2f currentTextPos;

	sfFont* font = sfFont_createFromFile("./Resources/Font/Action of the Time II.ttf");

	tmpResourcesBar.elementsNumber = elementNumber;
	tmpResourcesBar.modifDisplayerNumber = 6;
	tmpResourcesBar.modifDisplayerActive = 0;
	tmpResourcesBar.displayTime = 3.0f;
	tmpResourcesBar.resourcesElements = malloc(sizeof(ResourcesElement)*elementNumber);

	for (int i = 0; i < tmpResourcesBar.modifDisplayerNumber; i++)
	{
		tmpResourcesBar.modifDisplayers[i].texts = malloc(sizeof(sfText*)* elementNumber);
		tmpResourcesBar.modifDisplayers[i].values = malloc(sizeof(int)* elementNumber);

		tmpResourcesBar.modifDisplayers[i].valuesNumber = elementNumber;
		tmpResourcesBar.modifDisplayers[i].activeTime = 0.0f;
		tmpResourcesBar.modifDisplayers[i].isActive = false;
	}
	
	if (direction == 0)
	{
		for (int i = 0; i < elementNumber; i++)
		{
			currentElementPos.x = resourcesElement[i].startOffset.x + (resourceElementSize.x * i) + firstElementPos.x;
			currentElementPos.y = resourcesElement[i].startOffset.y;
			
			currentTextPos.x = currentElementPos.x + 30.0f ;
			currentTextPos.y = currentElementPos.y + 12.0f;

			sfSprite_setPosition(resourcesElement[i].image, currentElementPos);
			sfText_setPosition(resourcesElement[i].text, currentTextPos);
			sfText_setPosition(resourcesElement[i].previewText, currentTextPos);

			for (int j = 0; j < tmpResourcesBar.modifDisplayerNumber; j++)
			{
				currentTextPos.y += 40.0f;
				tmpResourcesBar.modifDisplayers[j].texts[i] = InitText("00", currentTextPos, sfRed, 30);
				tmpResourcesBar.modifDisplayers[j].values[i] = 0;
				sfText_setOutlineThickness(tmpResourcesBar.modifDisplayers[j].texts[i], 5.0f);
			}

			tmpResourcesBar.resourcesElements[i] = resourcesElement[i];
		}
		tmpResourcesBar.hitBox.top = currentElementPos.y = resourcesElement[0].startOffset.y;
		tmpResourcesBar.hitBox.left = firstElementPos.x;
		tmpResourcesBar.hitBox.height = resourceElementSize.y;
		tmpResourcesBar.hitBox.width = resourceElementSize.x * elementNumber;

		tmpResourcesBar.hasFinishShowDisplay = true;
		tmpResourcesBar.haveToShowPreview = false;
	}
	return tmpResourcesBar;
}

void UpdateResourcesBar(ResourcesBar* resBar, float deltaTime)
{
	for (int i = 0; i < resBar->modifDisplayerActive; i++)
	{
		if (resBar->modifDisplayers[i].isActive)
		{
			if (resBar->modifDisplayers[i].activeTime < resBar->displayTime)
			{
				resBar->modifDisplayers[i].activeTime += deltaTime;

				//Fade effect
				
				int ratioAlpha;
				
				if (resBar->modifDisplayers[i].activeTime <= 1.0f)
				{
					ratioAlpha = (int)((resBar->modifDisplayers[i].activeTime / 1.0f) * 255);
				}
				else if (resBar->modifDisplayers[i].activeTime >= resBar->displayTime - 1.0f)
				{
					float temp = (float)((resBar->displayTime - resBar->modifDisplayers[i].activeTime) / 1.0f);
					ratioAlpha = (int)(temp * 255);
				}
				else 
				{
					ratioAlpha = 255;
				}

				
				for (int j = 0; j < resBar->elementsNumber; j++)
				{
					sfColor tempColor = sfText_getFillColor(resBar->modifDisplayers[i].texts[j]);
					tempColor.a = ratioAlpha;
					sfColor tempColor2 = { 0, 0, 0, ratioAlpha};
					sfText_setFillColor(resBar->modifDisplayers[i].texts[j], tempColor);
					sfText_setOutlineColor(resBar->modifDisplayers[i].texts[j], tempColor2);
				}
			}
			else
			{
				RemoveChangeText(resBar, i);
				return;
			}
		}
	}
}

void DisplayResourcesBar(sfRenderWindow* window, ResourcesBar* resourcesBar)
{
	
	for (int i = 0; i < resourcesBar->elementsNumber; i++)
	{
		sfRenderWindow_drawSprite(window, resourcesBar->resourcesElements[i].image, NULL);
		if (!resourcesBar->haveToShowPreview)
		{
			sfRenderWindow_drawText(window, resourcesBar->resourcesElements[i].text, NULL);
		}
		else
		{
			int villagerIndex = resourcesBar->elementsNumber > 4 ? 2 : 1;
			if (i < villagerIndex)
			{
				sfRenderWindow_drawText(window, resourcesBar->resourcesElements[i].text, NULL);
			}
			sfRenderWindow_drawText(window, resourcesBar->resourcesElements[i].previewText, NULL);
		}
	}

	for (int i = 0; i < resourcesBar->modifDisplayerActive; i++)
	{
		if (resourcesBar->modifDisplayers[i].isActive)
		{
			for (int j = 0; j < resourcesBar->modifDisplayers[i].valuesNumber; j++)
			{
				if (resourcesBar->modifDisplayers[i].values[j] != 0)
				{
					sfRenderWindow_drawText(window, resourcesBar->modifDisplayers[i].texts[j], NULL);	
				}
			}
		}
	}
}

void ConsumptionPreview(ResourcesBar* resourcesBar, sfVector2i mousePos, int barID)
{
	if (sfFloatRect_contains(&resourcesBar->hitBox, (float)mousePos.x, (float)mousePos.y) && !resourcesBar->haveToShowPreview)
	{
		sfColor tmpColor = { 0,0,0, 155 };
		sfText_setFillColor(resourcesBar->resourcesElements[0].text, tmpColor);
		
		if (barID == 2)
		{
			sfText_setFillColor(resourcesBar->resourcesElements[1].text, tmpColor);
		}
		resourcesBar->haveToShowPreview = true;
	}
	else if (!sfFloatRect_contains(&resourcesBar->hitBox, (float)mousePos.x, (float)mousePos.y) && resourcesBar->haveToShowPreview)
	{
		sfText_setFillColor(resourcesBar->resourcesElements[0].text, sfBlack);
		if (barID == 2)
		{
			sfText_setFillColor(resourcesBar->resourcesElements[1].text, sfBlack);
		}
		resourcesBar->haveToShowPreview = false;
	}
}

void UpdateResourcesBarPreview(ResourcesBar* resourcesBar, int* newData)
{
	sfColor supColor = { 95, 198, 36, 255 };
	sfColor equalColor = { 13, 195, 219, 255 };
	sfColor infColor = { 237, 45, 45, 255 };
	
	int offSet = resourcesBar->elementsNumber == 4 ? 1 : 2;

	for (int i = 0; i < 3; i++)
	{
		int temp = 0;
		for (int k = 0; k < resourcesBar->modifDisplayerActive; k++)
		{
			temp += resourcesBar->modifDisplayers[k].values[i + offSet];
		}

		int value = resourcesBar->resourcesElements[i + offSet].dataValue - newData[i] + temp;
		if (value >= 0)
		{
			sfText_setFillColor(resourcesBar->resourcesElements[i + offSet].previewText, supColor);
		}
		else if (value == 0)
		{
			sfText_setFillColor(resourcesBar->resourcesElements[i + offSet].previewText, equalColor);
		}
		else
		{
			sfText_setFillColor(resourcesBar->resourcesElements[i + offSet].previewText, infColor);
			value = 0;
		}

		UpdateTextDisplay(resourcesBar->resourcesElements[i + offSet].previewText, value);
	}
}

void UpdateResourcesBarInfos(ResourcesBar* resourcesBar, int* newData)
{
	sfColor addColor = {95, 198, 36, 255};
	sfColor removeColor = {237, 45, 45, 255};

	if (resourcesBar->modifDisplayerActive >= resourcesBar->modifDisplayerNumber)
	{
		printf("Too much changes at the same time!\n");
		return;
	}

	resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].isActive = true;
	int cptUpdateIsZero = 0;
	int cptNbUpdate = 0;
	for (int i = 0; i < resourcesBar->elementsNumber; i++)
	{
		if (newData[i] != 0 || (newData[i] == 0 && resourcesBar->resourcesElements[i].dataValue != 0))
		{
			int temp = 0;
			for (int k = 0; k < resourcesBar->modifDisplayerActive; k++)
			{
				temp += resourcesBar->modifDisplayers[k].values[i];
			}

			int value = newData[i] - resourcesBar->resourcesElements[i].dataValue - temp;
			resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].values[i] = value;
			if (value > 0)
			{
				sfText_setColor(resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].texts[i], addColor);
			}
			else if( value < 0)
			{
				sfText_setColor(resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].texts[i], removeColor);
			}
			else
			{
				cptUpdateIsZero++;
			}

			cptNbUpdate++;

			UpdateTextDisplay(resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].texts[i], value);
		}
	}

	if (cptUpdateIsZero == cptNbUpdate)
	{
		resourcesBar->modifDisplayers[resourcesBar->modifDisplayerActive].isActive = false;
	}
	else
	{
		resourcesBar->modifDisplayerActive++;
	}
}

bool HasFinishToDisplayEndTurnChange(ResourcesBar resourcesBar[], int resourcesBarNumber)
{
	int endDisplayCount = 0;
	for (int i = 0; i < resourcesBarNumber; i++)
	{
		if (resourcesBar[i].hasFinishShowDisplay)
		{
			endDisplayCount++;
		}
	}
	return endDisplayCount == resourcesBarNumber;
}

void InitResourceBarWithData(ResourcesBar* rBar, int *data)
{
	for (int i = 0; i < rBar->elementsNumber; i++)
	{
		rBar->resourcesElements[i].dataValue = data[i];
		UpdateTextDisplay(rBar->resourcesElements[i].text, rBar->resourcesElements[i].dataValue);
	}
}

ResourcesBar InfoBarProto(int ID, int* dataInfos)
{
	char* directories[5];
	sfVector2f firstElementPos;
	sfVector2f resourceElementSize;
	sfVector2f startOffset;
	int elementNumber;
	ResourcesElement* resourceElements;
	ResourcesBar resourceBar;

	startOffset.x = 0.0f;
	startOffset.y = 15.0f;
	resourceElementSize.x = 116.0f;
	resourceElementSize.y = 56.0f;

	if (dataInfos == NULL)
	{
		dataInfos = malloc(5 * sizeof(char*));
		for (int i = 0; i < 5; i++)
		{
			dataInfos[i] = 0;
		}
	}

	switch (ID)
	{
	case 0:
		elementNumber = 4;
		firstElementPos.x = 30;
		directories[0] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0004_peopleYellow.png";
		directories[1] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0002_food.png";
		directories[2] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0000_water.png";
		directories[3] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0001_ressources.png";
		break;
	case 1:
		elementNumber = 4;
		firstElementPos.x = 1920 - ((resourceElementSize.x * elementNumber) + 30);
		directories[0] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0005_peopleGreen.png";
		directories[1] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0002_food.png";
		directories[2] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0000_water.png";
		directories[3] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0001_ressources.png";
		break;
	case 2:
		elementNumber = 5;
		firstElementPos.x = (1920 / 2) - (((resourceElementSize.x * elementNumber) + 15) / 2);
		directories[0] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0004_peopleYellow.png";
		directories[1] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0005_peopleGreen.png";
		directories[2] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0002_food.png";
		directories[3] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0000_water.png";
		directories[4] = "./Resources/HUD/mainSceneHUD/Asset_Icon/_0001_ressources.png";
		break;
	default:
		printf("| ERROR VALUE | ResourcesDisplayer.c L.96");
		break;
	}
	firstElementPos.y = 0.0f;

	resourceElements = malloc(sizeof(resourceElements)* elementNumber);
	resourceElements = createResourcesElements(directories, dataInfos, elementNumber, startOffset, 35.0f);
	resourceBar = createResourcesBar(resourceElements, elementNumber, firstElementPos, resourceElementSize, 0);

	return resourceBar;
}