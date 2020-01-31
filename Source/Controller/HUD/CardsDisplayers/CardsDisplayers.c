#include "CardsDisplayers.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"
#include "../Text/Text.h"

float displayTime = 4.0f;

CardUI currentCard;
float currentDisplayTime;
bool isDisplay = false;

char* knowledgeFile[7];
char* expeditionFiles[4];

// Create a new knowledgeCard UI according to the knowledgeData;
// knowledgeData size 4 : nameIndex, habitantToFullBuild, MaterialToBuild, numberOfTurnNeeded
void SetKnoledgeCardUI(int* knowledgeData)
{
	int index = knowledgeData[0] + 1;
	sfTexture* tmpCardIlluTexture = sfTexture_createFromFile(knowledgeFile[index], NULL);
	sfTexture_setSmooth(tmpCardIlluTexture, sfTrue);
	sfSprite_setTexture(currentCard.illustration, tmpCardIlluTexture, 1);
	sfText_setString(currentCard.name, GetKnowledgeCardName(knowledgeData[0]));

	currentCard.number = 3;

	UpdateTextDisplay(currentCard.resourcesTexts[0], knowledgeData[1]);
	UpdateTextDisplay(currentCard.resourcesTexts[3], knowledgeData[2]);

	UpdateTextDisplay(currentCard.turn, knowledgeData[3]);

	currentDisplayTime = 0.0f;
	isDisplay = true;
}

// Create a new ExpeditionCard UI according to the expeditionData;
// expeditionData size 5 : TypeIndex, habitantFluctuation, foodGained, waterGained, materialGained
void SetExpeditionCardUI(int* expeditionData)
{
	int index;
	
	if (expeditionData[0] < 11) 
	{
		index = 0;
	}
	else if (expeditionData[0] == 13)
	{
		index = 3;
	}
	else
	{
		index = expeditionData[0] - 10;
	}



	sfTexture* tmpCardIlluTexture = sfTexture_createFromFile(expeditionFiles[index], NULL);
	sfTexture_setSmooth(tmpCardIlluTexture, sfTrue);
	sfSprite_setTexture(currentCard.illustration, tmpCardIlluTexture, 1);
	sfText_setString(currentCard.name, GetExpeditionCardName(expeditionData[0]));

	currentCard.number = 4;

	UpdateTextDisplay(currentCard.resourcesTexts[0], expeditionData[1]);

	UpdateTextDisplay(currentCard.resourcesTexts[1], expeditionData[2]);

	UpdateTextDisplay(currentCard.resourcesTexts[2], expeditionData[3]);

	UpdateTextDisplay(currentCard.resourcesTexts[3], expeditionData[4]);

	currentDisplayTime = 0.0f;
	isDisplay = true;
}

void InitCardUI()
{
	sfVector2f cardUIPos = { 1920 / 2 - (555 / 2), 1080 / 2 - (655.0f / 2)};
	sfVector2f illustrationPos = { cardUIPos.x + (55 / 2) - 1.5f,  cardUIPos.y + 58};
	sfVector2f namePos = { cardUIPos.x + 50, illustrationPos.y + 360 };
	sfVector2f resourcesIlluPos = { cardUIPos.x + 40, namePos.y + 50 };
	sfVector2f textsPos = { resourcesIlluPos.x + 45 + 40, resourcesIlluPos.y + 10 };

	knowledgeFile[0] = "./Resources/HUD/mainSceneHUD/illustrations/BadBackKnow.png";
	knowledgeFile[1] = "./Resources/HUD/mainSceneHUD/illustrations/Forteresse_Mur.png";
	knowledgeFile[2] = "./Resources/HUD/mainSceneHUD/illustrations/Forteresse_Bouclier.png";
	knowledgeFile[3] = "./Resources/HUD/mainSceneHUD/illustrations/Forteresse_Porte.png";
	knowledgeFile[4] = "./Resources/HUD/mainSceneHUD/illustrations/Champs.png";
	knowledgeFile[5] = "./Resources/HUD/mainSceneHUD/illustrations/ScierieKnow.png";
	knowledgeFile[6] = "./Resources/HUD/mainSceneHUD/illustrations/Puits.png";

	expeditionFiles[0] = "./Resources/HUD/mainSceneHUD/Form/Asset_Illu/StartExplo.png";
	expeditionFiles[1] = "./Resources/HUD/mainSceneHUD/illustrations/GoodBackExplo.png";
	expeditionFiles[2] = "./Resources/HUD/mainSceneHUD/illustrations/BadBackExplo.png";
	expeditionFiles[3] = "./Resources/HUD/mainSceneHUD/illustrations/IlluRetourPerdant.png";

	currentCard.background = LoadSprite("./Resources/HUD/mainSceneHUD/globalSceneHUD/info-window.png", 0);
	sfSprite_setPosition(currentCard.background, cardUIPos);

	currentCard.illustration = LoadSprite(expeditionFiles[0], 0);
	sfSprite_setPosition(currentCard.illustration, illustrationPos);

	currentCard.resourcesIllu[0] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/people.png", 0);
	currentCard.resourcesIllu[1] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/food.png", 0);
	currentCard.resourcesIllu[2] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/water.png", 0);
	currentCard.resourcesIllu[3] = LoadSprite("./Resources/HUD/mainSceneHUD/Asset_Icon/wood.png", 0);

	currentCard.name = InitText("", namePos, sfBlack, 20);

	sfSprite_setPosition(currentCard.resourcesIllu[0], resourcesIlluPos);
	currentCard.resourcesTexts[0] = InitText("", textsPos, sfBlack, 25);

	resourcesIlluPos.y += 100;
	textsPos.y += 100;
	sfSprite_setPosition(currentCard.resourcesIllu[1], resourcesIlluPos);
	currentCard.resourcesTexts[1] = InitText("", textsPos, sfBlack, 25);

	resourcesIlluPos.x = cardUIPos.x + 555 - 40 - 45;
	textsPos.x = resourcesIlluPos.x - 40 - 10;
	sfSprite_setPosition(currentCard.resourcesIllu[2], resourcesIlluPos);
	currentCard.resourcesTexts[2] = InitText("", textsPos, sfBlack, 25);

	resourcesIlluPos.y -= 100;
	textsPos.y -= 100;
	sfSprite_setPosition(currentCard.resourcesIllu[3], resourcesIlluPos);
	currentCard.resourcesTexts[3] = InitText("", textsPos, sfBlack, 25);

	textsPos.x = cardUIPos.x + 555 / 2;
	textsPos.y += 100;
	currentCard.turn = InitText("", textsPos, sfBlack, 25);
}

void UpdateCardUI(float deltaTime)
{
	if (currentDisplayTime < displayTime && isDisplay)
	{
		currentDisplayTime += deltaTime;
	}
	else
	{
		if (isDisplay)
		{
			isDisplay = false;
			currentDisplayTime = 0.0f;
		}
	}
}

bool IsCardDisplay()
{
	return isDisplay;
}

void hideCardUI()
{
	isDisplay = false;
}

void DisplayCardUI(sfRenderWindow* window)
{
	if (isDisplay)
	{
		sfRenderWindow_drawSprite(window, currentCard.illustration, NULL);
		sfRenderWindow_drawSprite(window, currentCard.background, NULL);
		sfRenderWindow_drawText(window, currentCard.name, NULL);

		if (currentCard.number == 3)
		{
			sfRenderWindow_drawSprite(window, currentCard.resourcesIllu[0],NULL);
			sfRenderWindow_drawSprite(window, currentCard.resourcesIllu[3], NULL);

			sfRenderWindow_drawText(window, currentCard.resourcesTexts[0], NULL);
			sfRenderWindow_drawText(window, currentCard.resourcesTexts[3], NULL);

			sfRenderWindow_drawText(window, currentCard.turn, NULL);
		}
		else if(currentCard.number == 4)
		{
			for (int i = 0; i < currentCard.number; i++)
			{
				sfRenderWindow_drawSprite(window, currentCard.resourcesIllu[i], NULL);
				sfRenderWindow_drawText(window, currentCard.resourcesTexts[i], NULL);
			}
		}
	}
}