#include <SFML\Graphics.h>
#include "../../../Model/Cards/Cards.h"
#include <stdbool.h>

typedef struct CardUI
{
	sfSprite* background;
	sfSprite* illustration;
	sfText* name;
	sfSprite* resourcesIllu[4];
	sfText* resourcesTexts[4];
	sfText* turn;
	int number;
}CardUI;

void SetKnoledgeCardUI(int* knowledgeData);
void SetExpeditionCardUI(int* expeditionData);
void InitCardUI();
void UpdateCardUI(float deltaTime);
bool IsCardDisplay();
void hideCardUI();
void DisplayCardUI(sfRenderWindow* window);