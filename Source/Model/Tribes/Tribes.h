#ifndef _TRIBES_H_
#define _TRIBES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../Cards/Cards.h"
#include "../Utilities/Utilities.h"

typedef struct
{
	int habitantLeft;
	int waterGained;
	int foodGained;
	int materialsGained;
	TurnOfExpedition actualTurn;
	bool isManaged;
	bool isDead;
} OnGoingExpedition;

typedef struct Tribe
{
	int habitants;
	int food;
	int water;
	int materials;
	int resourcesGenerationPerTurn[3];
	bool hasExpComingBack;
	KnowledgeCards intel[3];
	OnGoingExpedition onGoingExpedition[3];
} Tribes;

Tribes InitTribes();
int* ConsumptionCalculation(Tribes tribe, int turn, bool isPreview);
void ApplyConsumptionCalculation(Tribes *tmp, int turn, bool checkIfIsDead);
void UpgradeGainCalculation(Tribes *t1);
void ApplyChangeToTribesCalculs(int *turnEffects, Tribes *t1, bool checkIfIsDead);
bool AddKnowledgeToTribe(Tribes *t, int index);
void CheckIfHasEnoughRessources(Tribes tribe, int valueToCheck[]);
bool HasEnoughToUpgradeTribe(Tribes t1);
void ApplyKnowledgeToTribe(Tribes *t1, int index);
void GiveRessourceToEntities(int toGive[], int toWithdraw[], Tribes playerExchange, bool toTribes);
void ResetExpeditions(OnGoingExpedition *expeditiontoReset);
bool HasExpeditionsOnGoing(OnGoingExpedition expeditionsToCheck[]);
void PrintToScreenTribes(Tribes t1, int indexTribes);

#endif