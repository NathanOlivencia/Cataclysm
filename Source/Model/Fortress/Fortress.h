#ifndef _FORTRESS_H_
#define _FORTRESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../Utilities/Utilities.h"
#include "../Cards/Cards.h"

// BuildState of the differents knowledges
typedef struct BuildState
{
	KnowledgeCards knowledge;
	int builder;
	int materialsUsed;
	int tribeIndex;
	int turnProgress;
	bool buildDone;
	bool haveBeenManaged;
}BuildState;

// Fortress structure
typedef struct Fortress
{
	int fHabitants[2];// fHabitants[0] = tribu 1 |  fHabitants[1] = tribu 2
	int fMaterials;
	int fFood;
	int	fWater;
	BuildState fPartsBuildState[3];
}Fortress;

void PrintToScreenFortress(Fortress fortress);
bool FortressIsCompleted(BuildState bState[]);
void GetBackBuilder(Fortress *fortress, int *villagersInTribes, int answer, int i);
void AllowResourcesToBuildPart(BuildState *bState, Fortress *fort);
void AddResourcesToFortress(int tribe,int resources[], Fortress *fort);
bool AddKnowledgeToFort(int tribe,BuildState fPartsBuildState[], KnowledgeCards knowledgeToAdd);
int* FortressConsumptionCalculation(Fortress fortress, bool preview);
void FortressCalcul(Fortress *fortress);

#endif