#include "Tribes.h"

//Version to proto version
Tribes InitTribes()
{
	Tribes playerTribes;

	for (int i = 0; i < 3; i++)
	{
		playerTribes.intel[i] = GetProto(6);
		playerTribes.resourcesGenerationPerTurn[i] = 0;
		ResetExpeditions(&playerTribes.onGoingExpedition[i]);
		playerTribes.onGoingExpedition[i].isDead = false;
	}

	return(playerTribes);
}

//f(x) represent the loss of habitant in case the tribe doesn't have enough food
//f(x) =>  E = (e - e*0.6h) < 0 || F = (f*0.5h) < 0 => h-|E*0.6| || h-|F*0.5|
int ApplyCalculations(int toSubstract, int toSubstractWith, int *habitantsLoss, float percentageOfLoss)
{
	int tmp = toSubstract - toSubstractWith;

	if (tmp <= 0)
	{
		if (percentageOfLoss != 0.0)
		{
			*habitantsLoss += (int)round(-(tmp * percentageOfLoss));
		}
		return 0;
	}
	else
	{
		return tmp;
	}
}

int* ConsumptionCalculation(Tribes tribe, int turn, bool isPreview)
{
	int* toReturn = NULL;
	float summerNegatives = turn <= 10 ? 0.2f : 0.0f;
	//Coefficient that applies to the winter ressource and also the materials ressource
	float winterNegatives = turn > 10 ? 0.1f : 0.0f;

	toReturn = calloc(3, sizeof(int));
	if (toReturn == NULL)
	{
		printf("error lign 103 Tribes.c\n");
	}
	
	toReturn[0] = (int)round(tribe.habitants * (0.5f + winterNegatives));
	toReturn[1] = (int)round(tribe.habitants * (0.5f + summerNegatives));
	toReturn[2] = (int)round(tribe.habitants * winterNegatives);

	if (isPreview)
	{
		toReturn[0] -= tribe.resourcesGenerationPerTurn[0];
		toReturn[1] -= tribe.resourcesGenerationPerTurn[2];
		toReturn[2] -= tribe.resourcesGenerationPerTurn[1];
	}

	return(toReturn);
}

//Simulate the consomations of habitants for each tribe
void ApplyConsumptionCalculation(Tribes *tmp, int turn, bool checkIfIsDead)
{
	if (checkIfIsDead)
	{
		return;
	}

	int habitantsLoss = 0;

	int* comsuption = ConsumptionCalculation(*tmp, turn, false);

	tmp->food = ApplyCalculations(tmp->food, comsuption[0], &habitantsLoss, (0.5f + (turn <= 10 ? 0.2f : 0.0f)));
	tmp->water = ApplyCalculations(tmp->water, comsuption[1], &habitantsLoss, (0.5f + (turn <= 10 ? 0.2f : 0.0f)));
	tmp->materials = ApplyCalculations(tmp->materials, comsuption[2], &habitantsLoss, 0.0f);
	tmp->habitants = ApplyCalculations(tmp->habitants, habitantsLoss, &habitantsLoss, 0.0f);
}

//Store the new gain effect of a newly build upgrade to the array referencing if the tribe gain ressources each turn or not
void UpgradeGainCalculation(Tribes *playerTribes)
{
	playerTribes->food += playerTribes->resourcesGenerationPerTurn[0];
	playerTribes->materials += playerTribes->resourcesGenerationPerTurn[1];
	playerTribes->water += playerTribes->resourcesGenerationPerTurn[2];
}

//Apply Modification to Tribes stats
void ApplyChangeToTribesCalculs(int *turnEffects, Tribes *t1, bool checkIfIsDead)
{
	if (checkIfIsDead)
	{
		return;
	}

	t1->habitants += turnEffects[0];
	ClampBetweenZAndCap(&t1->habitants, -1);
	t1->food += turnEffects[1];
	ClampBetweenZAndCap(&t1->food, -1);
	t1->water += turnEffects[2];
	ClampBetweenZAndCap(&t1->water, -1);
	t1->materials += turnEffects[3];
	ClampBetweenZAndCap(&t1->materials, -1);

}

//Check if intel is not already in the array else add it to the array referencing the intel the tribe has
bool AddKnowledgeToTribe(Tribes *t, int index)
{
	for (int i = 0; i < 3; i++)
	{
		if (t->intel[i].name == index || t->resourcesGenerationPerTurn[index-3] > 0)
		{
			printf("Erreur savoir deja present dans la tribu\n");
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (t->intel[i].name < 3 || t->intel[i].name > 5)
		{
			t->intel[i] = GetProto(index);
			return true;
		}
	}
	return false;
}

//Check if Tribe has enough ressources comparing at the value the player want to withdraw else clamp to max it has
void CheckIfHasEnoughRessources(Tribes tribe, int valueToCheck[])
{
	if (valueToCheck[1] > tribe.food)
	{
		valueToCheck[1] = tribe.food;
	}
	if (valueToCheck[2] > tribe.water)
	{
		valueToCheck[2] = tribe.water;
	}
	if (valueToCheck[3] > tribe.materials)
	{
		valueToCheck[3] = tribe.materials;
	}
}

//Check if has intel and the adequat ressource to build the upgrade
bool HasEnoughToUpgradeTribe(Tribes t1)
{
	for (int i = 0; i < 3; i++)
	{
		if (t1.intel[i].name >= 3 && t1.intel[i].name <= 5 && t1.intel[i].materialsToBuild <= t1.materials)
		{
			return true;
		}
	}
	return false;
}

//Add the upgrade to the Tribe
void ApplyKnowledgeToTribe(Tribes *t1, int index)
{
	//map the corresponding value to add between knowledge for tribes who are 3 to 5 and array of updrages wich are 0 to 2
	int map = t1->intel[index].name - 3;
	int resourcesToGive = map == 1 ? 15 : 30;
	
	t1->resourcesGenerationPerTurn[map] += resourcesToGive;
	t1->materials -= t1->intel[index].materialsToBuild;
}

//Return two array representating the value to give and to withdraw respecting the following order : [0] = habitants; [1] = food; [2] = water; [3] = materials;
//playerExchange[0] = the sender ; playerExchange[1] = the reciever
void GiveRessourceToEntities(int toGive[], int toWithdraw[], Tribes playerExchange, bool toTribes)
{
	//AskEffects(toGive, toTribes ? 3 : 4);
	CheckIfHasEnoughRessources(playerExchange, toGive);

	//Reversing value to withdraw from this tribe to give to the other tribe
	for (int i = 0; i < 4; i++)
	{
		if (toGive[i] > 0)
		{
			toWithdraw[i] = -toGive[i];
		}
		else
		{
			toGive[i] = 0;
			toWithdraw[i] = 0;
		}
	}
}

//Reset expedition array in tribes struct
void ResetExpeditions(OnGoingExpedition *expeditiontoReset)
{
	expeditiontoReset->foodGained = 0;
	expeditiontoReset->habitantLeft = 0;
	expeditiontoReset->materialsGained = 0;
	expeditiontoReset->actualTurn = t0;
	expeditiontoReset->waterGained = 0;
	expeditiontoReset->isManaged = false;

}

//Check if has tribe has expedition on going
bool HasExpeditionsOnGoing(OnGoingExpedition expeditionsToCheck[])
{
	for (int i = 0; i < 3; i++)
	{
		if(expeditionsToCheck[i].actualTurn != t0 && !expeditionsToCheck[i].isManaged)
		{
			return true;
		}
	}
	return false;
}

//Print Tribes Stats
void PrintToScreenTribes(Tribes t1, int indexTribes)
{
	printf("Tribu %d \n", indexTribes);
	printf("Habitants %d\n", t1.habitants);
	printf("Food %d\n", t1.food);
	printf("Water %d\n", t1.water); 
	printf("Materials %d\n", t1.materials);
	printf("Upgrades : ");
	for (int i = 0; i < 3; i++)
	{
		if (t1.resourcesGenerationPerTurn[i] != 0)
		{
			printf("\n - %s", enumToString(i + 3));
		}
	}
	printf("\n");
	printf("Knowledge : ");
	for (int i = 0; i < 3; i++)
	{
		if (t1.intel[i].name >= 0 && t1.intel[i].name <= 5)
		{
			printf("\n - %s", enumToString(t1.intel[i].name));
		}

	}
	printf("\n");
	printf("***************************************************\n");
}