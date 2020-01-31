#include "Fortress.h"



// Display informations to the console
// Parameters : ( Fortress fortress )
void PrintToScreenFortress(Fortress fortress)
{
	printf("FORT RESOURCES : \n ***  ***\n");
	printf("fort habitants tribu 1 = %d\n", fortress.fHabitants[0]);// NEW
	printf("fort habitants tribu 2 = %d\n", fortress.fHabitants[1]);// NEW
	printf("fort food  = %d\n", fortress.fFood);
	printf("fort water  = %d\n", fortress.fWater);
	printf("fort materials  = %d\n", fortress.fMaterials);

	printf("\nKNOWLEDGES BUILDING STATS\n ** **\n");

	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name != Empty)
		{
			printf(" Knowledge : %s\n", enumToString(fortress.fPartsBuildState[i].knowledge.name));
			if (fortress.fPartsBuildState[i].buildDone)
			{
				printf("   ! Build Completed !\n");
			}
			else
			{
				printf(" Tribe that can build this part : Tribe %d\n", fortress.fPartsBuildState[i].tribeIndex + 1);// NEW
				printf(" Builder allowed : %d / %d\n", fortress.fPartsBuildState[i].builder, fortress.fPartsBuildState[i].knowledge.habitantsToFullBuildPotential);
				printf(" Materials used : %d / %d\n", fortress.fPartsBuildState[i].materialsUsed, fortress.fPartsBuildState[i].knowledge.materialsToBuild);
				printf(" Turn(s) to finish building part : %d / %d\n", fortress.fPartsBuildState[i].turnProgress, fortress.fPartsBuildState[i].knowledge.numberOfTurnToBuild);
			}
			printf("*\n");
		}
	}

	printf("***************************************************\n");
}

// Check if the fortresse is completed
// Return true if all the BuildState of the array have been complete (turnProgress == numberOfTurnToBuild)
// Parameters : ( BuildState bState[] )
bool FortressIsCompleted(BuildState bState[])
{
	for (int i = 0; i < 3; i++)
	{
		if(bState[i].turnProgress != bState[i].knowledge.numberOfTurnToBuild)
		{
			printf("Fortress not complete\n");
			return false;
		}
	}
	return true;
}

// Get back the builders of the tribe allowed to build fortress's parts
// Parameters : ( Fortress *fortress | int *villagersInTribes | int tribeIndex )
void GetBackBuilder(Fortress *fortress, int *villagersInTribes, int answer, int i)
{
		switch (answer)
		{
		case 0:
			*villagersInTribes += fortress->fPartsBuildState[i].builder;
			break;
		case 1:
			fortress->fHabitants[fortress->fPartsBuildState[i].tribeIndex] += fortress->fPartsBuildState[i].builder;
			break;
		default:
			break;
		}
		fortress->fPartsBuildState[i].builder = 0;
}

// Allow resources (materials and builder) for building fortress's parts if it's possible
// Parameters : ( BuildState *bState | Fortress *fort )
void AllowResourcesToBuildPart(BuildState *bState, Fortress *fort)
{
	
	int divisor = bState->knowledge.numberOfTurnToBuild;
	int builderNeeded = bState->knowledge.habitantsToFullBuildPotential;
	int materialsNeeded = bState->knowledge.materialsToBuild / divisor;

	printf("|*%s*|\n", enumToString(bState->knowledge.name));

	if (bState->builder >= bState->knowledge.habitantsToFullBuildPotential)
	{
		printf("Builders number is good to work\n");	
	}
	else
	{
		if (builderNeeded > fort->fHabitants[bState->tribeIndex])
		{
			printf("*/!/* Tribe %d, you don't have enough villagers in the fortress to build. **\n", bState->tribeIndex+1);
		}
		else
		{
			fort->fHabitants[bState->tribeIndex] -= builderNeeded;
			bState->builder += builderNeeded;
			printf("Adding builders to build\n");
		}	
	}
	
	if (materialsNeeded > fort->fMaterials)
	{
		printf("*/!/* There is not enough materials in the fortress to build. **\n");
	}
	else if (bState->builder >= bState->knowledge.habitantsToFullBuildPotential)
	{
		// Use ressources to build
		printf("Use Materials\n");
		
		fort->fMaterials -= materialsNeeded;
		bState->materialsUsed += materialsNeeded;
		bState->turnProgress++;
		printf("Build fort. Turn validated.\n");
	}
	printf("\n");
}

// Add resources to the fortress (using an array)
// Parameters : ( int tribe | int resources[] | Fortress *fort )
void AddResourcesToFortress(int tribe, int resources[], Fortress *fort)
{
	fort->fHabitants[tribe] += resources[0];
	fort->fFood += resources[1];
	fort->fWater += resources[2];
	fort->fMaterials += resources[3];
}

// Add knowledge to the fortress if it's not already learned
// Parameters : ( int tribe | BuildState fPartsBuildState[] | KnowledgeCards knowledgeToAdd )
bool AddKnowledgeToFort(int tribe, BuildState fPartsBuildState[], KnowledgeCards knowledgeToAdd)
{
	for(int i = 0; i < 3 ; i++)
	{
		if(fPartsBuildState[i].knowledge.name != Empty)
		{
			if(fPartsBuildState[i].knowledge.name == knowledgeToAdd.name)
			{
				printf("SAVOIR DEJA APPRIT !\n");
				return false;
			}
		}
		else
		{
			fPartsBuildState[i].knowledge = knowledgeToAdd;
			fPartsBuildState[i].tribeIndex = tribe;
			fPartsBuildState[i].builder = 0;
			fPartsBuildState[i].materialsUsed = 0;
			fPartsBuildState[i].turnProgress = 0;
			fPartsBuildState[i].buildDone = false;
			return true;
		}
	}
	return false;
}

//  Represent the loss of habitant in case the fortress doesn't have enough resources in
// Parameters : ( int toSubstract | int toSubstractWith | int *habitantsLoss | float percentageOfLoss)
int ApplyConsumption(int toSubstract, int toSubstractWith, int *habitantsLoss, float percentageOfLoss)
{
	int result = toSubstract - toSubstractWith;

	if (result <= 0)
	{
		if (percentageOfLoss != 0.0)
		{
			*habitantsLoss += (int)round(-(result * percentageOfLoss));
		}
		return 0;
	}
	else
	{
		/**habitantsLoss -= result;
		if (habitantsLoss < 0)
		{
			*habitantsLoss = 0;
		}*/
		return result;
	}
}

int* FortressConsumptionCalculation(Fortress fortress, bool preview)
{
	int* toReturn = NULL;
	toReturn = calloc(3, sizeof(int));
	
	int totalHabitants = fortress.fHabitants[0] + fortress.fHabitants[1];
	int consoMaterials = 0;

	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name != Empty && fortress.fPartsBuildState[i].builder != 0)
		{
			totalHabitants += fortress.fPartsBuildState[i].builder;
			if (preview && !fortress.fPartsBuildState[i].buildDone)
			{
				consoMaterials += fortress.fPartsBuildState[i].knowledge.materialsToBuild / fortress.fPartsBuildState[i].knowledge.numberOfTurnToBuild; /// bState->knowledge.materialsToBuild / divisor;
			}
		}
	}

	int conso = (int)round(totalHabitants * 0.5f);

	for (int i = 0; i < 2; i++)
	{
		toReturn[i] = conso;
	}
	toReturn[2] = consoMaterials;
	return(toReturn);
}

// Simulate the consumption of villagers in the fortress
// Parameters : ( Fortress *fortress )
void FortressConsumption(Fortress *fortress)
{
	int habitantsLoss = 0;
	int t1Habitants = 0;
	int t2Habitants = 0;

	for (int knowledgeIndex = 0; knowledgeIndex < 3; knowledgeIndex++)
	{
		if (fortress->fPartsBuildState[knowledgeIndex].builder > 0)
		{
			if (fortress->fPartsBuildState[knowledgeIndex].tribeIndex == 0)
			{
				t1Habitants += fortress->fPartsBuildState[knowledgeIndex].builder;
			}
			else
			{
				t2Habitants += fortress->fPartsBuildState[knowledgeIndex].builder;
			}
		}
	}

	for (int tribeIndex = 0; tribeIndex < 2; tribeIndex++)
	{
		if (fortress->fHabitants[tribeIndex] > 0)
		{
			if (tribeIndex == 0)
			{
				t1Habitants += fortress->fHabitants[tribeIndex];
			}
			else
			{
				t2Habitants += fortress->fHabitants[tribeIndex];
			}
		}
	}

	printf("t1 habitants : %d\n", t1Habitants);
	printf("t2 habitants : %d\n", t2Habitants);
	int Consumption = FortressConsumptionCalculation(*fortress, false)[0];
	printf("consumption %d\n", Consumption);
	fortress->fFood = ApplyConsumption(fortress->fFood, Consumption, &habitantsLoss, 0.5f);
	fortress->fWater = ApplyConsumption(fortress->fWater, Consumption, &habitantsLoss, 0.5f);
	printf("habitant loss : %d\n", habitantsLoss);

	if (habitantsLoss > 0)
	{
		int t1Loss = (int)roundf(((float)t1Habitants / (float)(t1Habitants + t2Habitants)) * habitantsLoss);
		int t2Loss = (int)roundf(((float)t2Habitants / (float)(t1Habitants + t2Habitants)) * habitantsLoss);
		printf("t1 loss percent : %f\n", roundf(((float)t1Habitants / (float)(t1Habitants + t2Habitants))));

		printf("t1 loss : %d\n", t1Loss);
		printf("t2 loss : %d\n", t2Loss);

		if (fortress->fHabitants[0] > t1Loss)
		{
			fortress->fHabitants[0] -= t1Loss;
			t1Loss = 0;
		}
		else
		{
			t1Loss -= fortress->fHabitants[0];
			fortress->fHabitants[0] = 0;
		}

		if (fortress->fHabitants[1] > t2Loss)
		{
			fortress->fHabitants[1] -= t2Loss;
			t2Loss = 0;
		}
		else
		{
			t2Loss -= fortress->fHabitants[1];
			fortress->fHabitants[1] = 0;
		}

		if (fortress->fHabitants[1] == 0 || fortress->fHabitants[0] == 0)
		{
			for (int knowledgeIndex = 0; knowledgeIndex < 3; knowledgeIndex++)
			{
				if (fortress->fPartsBuildState[knowledgeIndex].builder > 0)
				{
					if (fortress->fPartsBuildState[knowledgeIndex].tribeIndex == 0) 
					{
						fortress->fPartsBuildState[knowledgeIndex].builder = ApplyConsumption(fortress->fPartsBuildState[knowledgeIndex].builder, t1Loss, &t1Loss, 0.0f);
					}
					else
					{
						fortress->fPartsBuildState[knowledgeIndex].builder = ApplyConsumption(fortress->fPartsBuildState[knowledgeIndex].builder, t2Loss, &t2Loss, 0.0f);
					}
				}
			}
		}
	}
}

// Make the end turn calculation for the fortress (knowledges only for the moment)
// Parameters : ( Fortress *fortress )
void FortressCalcul(Fortress *fortress)
{
	printf("\n || CALCUL RECAP || \n\n");

	if(!FortressIsCompleted(fortress->fPartsBuildState))
	{
		// Building fortress parts
		for (int i = 0; i < 3; i++)
		{
			if ((fortress->fPartsBuildState[i].knowledge.name != Empty) && (!fortress->fPartsBuildState[i].buildDone))
			{
				if (fortress->fPartsBuildState[i].turnProgress < fortress->fPartsBuildState[i].knowledge.numberOfTurnToBuild)
				{
					AllowResourcesToBuildPart(&fortress->fPartsBuildState[i], fortress);
				}
				if (fortress->fPartsBuildState[i].turnProgress == fortress->fPartsBuildState[i].knowledge.numberOfTurnToBuild)
				{
					fortress->fPartsBuildState[i].buildDone = true;
				}
			}
		}
	}
	// Villagers in fortress consumption
	FortressConsumption(fortress);
}