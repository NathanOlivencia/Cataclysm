#include "../GameLogic/GameLogic.h"
#include "GameLogicManager.h"

#define NUMBEROFPLAYER 2

struct Player players[NUMBEROFPLAYER];
Fortress fortress;
int expeditionsGains[2][4];
ExpAndKnowPilePack expAndKnowCardsPile;
EventCard *eventCardsPile;


void InitData(bool isReload)
{	
	fortress.fFood = 0;
	fortress.fHabitants[0] = 0;
	fortress.fHabitants[1] = 0;
	fortress.fMaterials = 0;
	fortress.fWater = 0;
	fortress.fPartsBuildState[0] = (BuildState) { GetProto(6), 0, 0, false, false };
	fortress.fPartsBuildState[1] = (BuildState) { GetProto(6), 0, 0, false, false };
	fortress.fPartsBuildState[2] = (BuildState) { GetProto(6), 0, 0, false, false };

	for (int i = 0; i < 2; i++)
	{
		players[i].isDead = false;
		players[i].LastChanceUsed = false;
		players[i].playerTribes.hasExpComingBack = false;
		players[i].playerTribes = InitTribes();
		players[i].lastExpCreatedID = -1;
		players[i].lastIntelFoundNameID = -1;
		players[i].diceResult = -1;

		if (!isReload)
		{
			players[i].lastExpStats = calloc(6, sizeof(int));
		}
		else
		{
			for (int j = 0; j < 6; j++)
			{
				players[i].lastExpStats[j] = 0;
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			expeditionsGains[i][j] = 0;
		}
	}

	if (isReload)
	{
		free(expAndKnowCardsPile.expeditionCardsPile);
		free(expAndKnowCardsPile.knowledgeCardsPile);
		free(eventCardsPile);
	}

	expAndKnowCardsPile.expeditionCardsPile = InitExpeditionCardsPile();
	expAndKnowCardsPile.knowledgeCardsPile = InitKnowledgeCardsPile();
	eventCardsPile = InitEventCardsPile();
}

void SendTribesDatas(int* data)
{
	players[data[0]].playerTribes.habitants = data[1];
	players[data[0]].playerTribes.food = data[2];
	players[data[0]].playerTribes.water = data[3];
	players[data[0]].playerTribes.materials = data[4];
}

void UpdateEveryInfos(void(*callback)(int*, int))
{
	callback(GetTribesData(0), 0);
	callback(GetTribesData(1), 1);
	callback(GetFortressData(), 2);
	callback(ConsumptionCalculation(players[0].playerTribes, 1, false), 0 + 3);
	callback(ConsumptionCalculation(players[1].playerTribes, 1, false), 1 + 3);
	callback(FortressConsumptionCalculation(fortress, false), 2 + 3);
}

bool CheckInput(GameState actualState, int inputIndex, void(*cbPopUpForm)(int, int*))
{
	if (inputIndex == 0 && CheckAvailableExpeditionSlot(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.onGoingExpedition) == -1)
	{
		return false;
	}
	
	cbPopUpForm(inputIndex, GetTribesData(actualState.onGoingGameStatus.actualPlayerID));
	return true;
}

bool ManagerInput(GameState actualState, int inputIndex, int *dataField, void(*cbHUDVisualUpdate)(int*, int), void(*cbMainSceneVisualUpdate)(int*, int), void(*cbHudDisplayCard)(int*, int))
{
	if (actualState.actualscene == 0)
	{
		//Not interaction with the logics when we are on the mainMenu
		return false;
	}

	//0 is New exp ressources
	//1 is New exp knowledge
	//2 is Gift to other tribe
	//3 is Send resources to fortress
	//4 is Upgrade his tribe
	//5 is Last chance
	switch (inputIndex)
	{
		case 0:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{
				cbHUDVisualUpdate(GetTribesData(actualState.onGoingGameStatus.actualPlayerID), actualState.onGoingGameStatus.actualPlayerID);
				cbHUDVisualUpdate(ConsumptionCalculation(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes, actualState.onGoingGameStatus.turn, true),
					actualState.onGoingGameStatus.actualPlayerID + 3);
				
				int id = actualState.onGoingGameStatus.actualPlayerID;
				cbMainSceneVisualUpdate(GetTribesExpData(id, players[id].lastExpCreatedID), id);

				for (int i = 0; i < 6; i++)
				{
					printf("value lastExpStats : %d\n", players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats[i]);
				}
				
				cbHudDisplayCard(players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats, 0);
			}
			else
			{
				printf("Something went wrong when going in expedition seeking for resources.\n");
				return false;
			}
			break;
		case 1:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{				
				cbHUDVisualUpdate(GetTribesData(actualState.onGoingGameStatus.actualPlayerID), actualState.onGoingGameStatus.actualPlayerID);
				cbHUDVisualUpdate(ConsumptionCalculation(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes, actualState.onGoingGameStatus.turn, true),
					actualState.onGoingGameStatus.actualPlayerID + 3);

				int id = actualState.onGoingGameStatus.actualPlayerID;
				
				if (players[id].lastIntelFoundNameID >= 3)
				{
					for (int i = 2; i >= 0; i--)
					{
						if (players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.intel[i].name != Empty)
						{
							cbMainSceneVisualUpdate(GetTribesIntel(id, i, players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.intel[i].name), id);
							i = 0;
						}
					}
				}
				else if (players[id].lastIntelFoundNameID >= 0)
				{
					for (int i = 2; i >= 0; i--)
					{
						if (fortress.fPartsBuildState[i].knowledge.name != Empty)
						{
							cbMainSceneVisualUpdate(GetFortressIntel(i, fortress.fPartsBuildState[i].knowledge.name), id);
							i = 0;
						}
					}
				}

				cbHudDisplayCard(players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats, 1);

				players[id].lastIntelFoundNameID = -1;
				

				return true;
			}
			else
			{
				printf("Something went wrong when going in expedition seeking for knowledge.\n");
				return false;
			}
			break;
		case 2:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{
				cbHUDVisualUpdate(GetTribesData(0), 0);
				cbHUDVisualUpdate(GetTribesData(1), 1);
				cbHUDVisualUpdate(ConsumptionCalculation(players[0].playerTribes, actualState.onGoingGameStatus.turn, true), 0 + 3);
				cbHUDVisualUpdate(ConsumptionCalculation(players[1].playerTribes, actualState.onGoingGameStatus.turn, true), 1 + 3);
			}
			else
			{
				printf("Something went wrong when giving to the other tribe.\n");
				return false;
			}
			break;
		case 3:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{
				cbHUDVisualUpdate(GetTribesData(actualState.onGoingGameStatus.actualPlayerID), actualState.onGoingGameStatus.actualPlayerID);
				cbHUDVisualUpdate(ConsumptionCalculation(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes, actualState.onGoingGameStatus.turn, true),
					actualState.onGoingGameStatus.actualPlayerID + 3);
				cbHUDVisualUpdate(GetFortressData(), 2);
				
				int h[2] = { fortress.fHabitants[0], fortress.fHabitants[1] };
				for (int i = 0; i < 3; i++)
				{
					if (fortress.fPartsBuildState[i].knowledge.name != Empty)
					{
						h[fortress.fPartsBuildState[i].tribeIndex] += fortress.fPartsBuildState[i].builder;
					}
				}
				cbHUDVisualUpdate(FortressConsumptionCalculation(fortress, true), 2 + 3);
			}
			else
			{
				printf("Something went wrong when giving to the fortress.\n");
				return false;
			}
			break;
		case 4:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{
				cbMainSceneVisualUpdate(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.resourcesGenerationPerTurn, actualState.onGoingGameStatus.actualPlayerID);
				cbHUDVisualUpdate(GetTribesData(actualState.onGoingGameStatus.actualPlayerID), actualState.onGoingGameStatus.actualPlayerID);
				cbHUDVisualUpdate(ConsumptionCalculation(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes, actualState.onGoingGameStatus.turn, true),
					actualState.onGoingGameStatus.actualPlayerID + 3);
			}
			else
			{
				printf("Something went wrong when upgrading the tribe.\n");
				return false;
			}
			break;
		case 5:
			if (ActionManager(players, &fortress, actualState.onGoingGameStatus.actualPlayerID, inputIndex, dataField, expAndKnowCardsPile))
			{
				cbMainSceneVisualUpdate(NULL, players[actualState.onGoingGameStatus.actualPlayerID].diceResult);
				cbHUDVisualUpdate(ConsumptionCalculation(players[actualState.onGoingGameStatus.actualPlayerID].playerTribes, actualState.onGoingGameStatus.turn, true),
					actualState.onGoingGameStatus.actualPlayerID + 3);
				players[actualState.onGoingGameStatus.actualPlayerID].LastChanceUsed = true;
			}
			else
			{
				printf("Something went wrong when using last chance.\n");
				return false;
			}

			break;
		default:
			break;
	}
	return true;
}

bool CheckExp(int playerID)
{
	for (int i = 0; i < 3; i++)
	{
		if (players[playerID].playerTribes.onGoingExpedition[i].actualTurn != t0 && !players[playerID].playerTribes.onGoingExpedition[i].isManaged)
		{
			return true;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (players[playerID].playerTribes.onGoingExpedition[i].actualTurn != t0)
		{
			players[playerID].playerTribes.onGoingExpedition[i].isManaged = false;
		}
	}
	return false;
}

bool ManagerExpeditions(GameState actualState, int value, bool *hasToChange, void(*cbUpdateExpPanel)(int*, int), void(*cbHudDisplayCard)(int*, int))
{	
	if (ExpeditionsManager(players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats, 
		&players[actualState.onGoingGameStatus.actualPlayerID].playerTribes,
		expeditionsGains[actualState.onGoingGameStatus.actualPlayerID],
		actualState.onGoingGameStatus.hisManagingExp, value, expAndKnowCardsPile.expeditionCardsPile))
	{
		if (players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.onGoingExpedition[actualState.onGoingGameStatus.hisManagingExp].isDead)
		{
			players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.onGoingExpedition[actualState.onGoingGameStatus.hisManagingExp].isDead = false;
			players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats[0] = 13;
		}	
		
		cbUpdateExpPanel(GetTribesExpData(actualState.onGoingGameStatus.actualPlayerID, actualState.onGoingGameStatus.hisManagingExp),
			actualState.onGoingGameStatus.actualPlayerID);

		int id = actualState.onGoingGameStatus.actualPlayerID;
		int indexExp = actualState.onGoingGameStatus.hisManagingExp;

		cbHudDisplayCard(players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats, 0);
		return true;
	}
	else
	{
		if (players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.onGoingExpedition[actualState.onGoingGameStatus.hisManagingExp].isDead)
		{
			players[actualState.onGoingGameStatus.actualPlayerID].playerTribes.onGoingExpedition[actualState.onGoingGameStatus.hisManagingExp].isDead = false;
			players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats[0] = 13;
		}

		cbUpdateExpPanel(GetTribesExpData(actualState.onGoingGameStatus.actualPlayerID, actualState.onGoingGameStatus.hisManagingExp),
			actualState.onGoingGameStatus.actualPlayerID);
		
		*hasToChange = true;
		int id = actualState.onGoingGameStatus.actualPlayerID;
		int indexExp = actualState.onGoingGameStatus.hisManagingExp;
		

		cbHudDisplayCard(players[actualState.onGoingGameStatus.actualPlayerID].lastExpStats, 0);
		return false;
	}
}

void ProcessBeginTurnSimulation(int turn, void(*cbHUDVisualUpdate)(int*, int))
{
	BeginTurnCalculation(turn, players, &fortress);
	cbHUDVisualUpdate(GetTribesData(0), 0);
	cbHUDVisualUpdate(GetTribesData(1), 1);
	cbHUDVisualUpdate(ConsumptionCalculation(players[0].playerTribes, turn, true), 0 + 3);
	cbHUDVisualUpdate(ConsumptionCalculation(players[1].playerTribes, turn, true), 1 + 3);
}

bool ProcessCataclysmSimulation(void(*callback)(int*, int))
{
	bool isWin = true;
	for (int i = 0; i < 3; i++)
	{
		if (!fortress.fPartsBuildState[i].buildDone)
		{
			isWin = false;
		}
	}

	if (isWin)
	{
		ActionManager(players, &fortress, 0, 3, GetTribesData(0), expAndKnowCardsPile);
		ActionManager(players, &fortress, 1, 3, GetTribesData(1), expAndKnowCardsPile);
		callback(GetTribesData(0), 0);
		callback(GetTribesData(1), 1);
		callback(GetFortressData(), 2);
		callback(ConsumptionCalculation(players[0].playerTribes, 20, true), 0 + 3);
		callback(ConsumptionCalculation(players[1].playerTribes, 20, true), 1 + 3);
		callback(FortressConsumptionCalculation(fortress, false), 2 + 3);
		//Callback to do the fade in the HUDCONTROLLER
		callback(NULL, 22);
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			players[i].playerTribes.habitants = 0;
			players[i].playerTribes.food = 0;
			players[i].playerTribes.water = 0;
			players[i].playerTribes.materials = 0;
		}

		fortress.fHabitants[0] = 0;
		fortress.fHabitants[1] = 0;
		fortress.fFood = 0; 
		fortress.fMaterials = 0;
		fortress.fWater = 0;

		callback(GetTribesData(0), 0);
		callback(GetTribesData(1), 1);
		callback(GetFortressData(), 2);
		callback(ConsumptionCalculation(players[0].playerTribes, 20, true), 0 + 3);
		callback(ConsumptionCalculation(players[1].playerTribes, 20, true), 1 + 3);
		callback(FortressConsumptionCalculation(fortress, false), 2 + 3);
		callback(NULL, 21);
	}

	return isWin;
}

void ProcessEndTurnSimulation(int turn, void(*cbHUDVisualUpdate)(int*, int), void(*cbHUDFortressUpdatePanel)(int*, int), void(*cbFortressMapUpdate)(int), void(*cbGameLost)())
{	
	EndTurnCalculation(turn, players, &fortress);
	cbHUDVisualUpdate(GetTribesData(0), 0);
	cbHUDVisualUpdate(GetTribesData(1), 1);
	cbHUDVisualUpdate(GetFortressData(), 2);
	
	if (turn > 10)
	{
		EventCalculation(players, eventCardsPile);
		cbHUDVisualUpdate(GetTribesData(0), 0);
		cbHUDVisualUpdate(GetTribesData(1), 1);
	}

	cbHUDVisualUpdate(ConsumptionCalculation(players[0].playerTribes, turn, true), 0 + 3);
	cbHUDVisualUpdate(ConsumptionCalculation(players[1].playerTribes, turn, true), 1 + 3);

	
	int h[2] = { fortress.fHabitants[0], fortress.fHabitants[1] };
	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name != Empty)
		{
			h[fortress.fPartsBuildState[i].tribeIndex] += fortress.fPartsBuildState[i].builder;
		}
	}
	cbHUDVisualUpdate(FortressConsumptionCalculation(fortress, true), 2 + 3);

	for (int i = 0; i < 2; i++)
	{
		if (players[i].isDead)
		{
			cbGameLost();
			return;
		}
	}
	
	for (int i = 0; i < 2; i++)
	{
		if (expeditionsGains[i][0] != 0)
		{
			AddExpeditionGain(&players[i], expeditionsGains[i]);
			cbHUDVisualUpdate(GetTribesData(i), i);
			cbHUDVisualUpdate(ConsumptionCalculation(players[i].playerTribes, turn, true), i + 3);
		}

		for (int j = 0; j < 4; j++)
		{
			expeditionsGains[i][j] = 0;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name != Empty)
		{
			cbHUDFortressUpdatePanel(GetFortressIntel(i, fortress.fPartsBuildState[i].knowledge.name), i);
			if (fortress.fPartsBuildState[i].buildDone && !fortress.fPartsBuildState[i].haveBeenManaged)
			{
				cbFortressMapUpdate(fortress.fPartsBuildState[i].knowledge.name);
				fortress.fPartsBuildState[i].haveBeenManaged = true;
			}
		}
		else
		{
			return;
		}
	}
}

bool CheckIfCanUseLastChance(int playerID)
{
	return(!players[playerID].LastChanceUsed && players[playerID].playerTribes.habitants <= 10);
}


bool CheckIfHasBuilderToManage(int playerID, int *IdSave)
{
	for (int i = 0; i < 3; i++)
	{
		if ((fortress.fPartsBuildState[i].tribeIndex == playerID) && (fortress.fPartsBuildState[i].buildDone == true) && (fortress.fPartsBuildState[i].builder > 0))
		{
			*IdSave = i;
			return true;
		}
	}
	
	return false;
	//GetBackBuilder(&fortress, &players[playerID].playerTribes.habitants, playerID);
}

void GetBackBuilderLink(int playerID, int value, int index, void(*callback)(int*, int))
{
	GetBackBuilder(&fortress, &players[playerID].playerTribes.habitants, value, index);
	callback(GetTribesData(playerID), playerID);
	callback(GetFortressData(), 2);
}

int* GetTribesData(int index)
{
	int *arrayToReturn = malloc(4 * sizeof(int));
	arrayToReturn[0] = players[index].playerTribes.habitants;
	arrayToReturn[1] = players[index].playerTribes.food;
	arrayToReturn[2] = players[index].playerTribes.water;
	arrayToReturn[3] = players[index].playerTribes.materials;

	return(arrayToReturn);
}

int* GetFortressData()
{
	int *arrayToReturn = malloc(5 * sizeof(int));
	arrayToReturn[0] = fortress.fHabitants[0];
	arrayToReturn[1] = fortress.fHabitants[1];

	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name != Empty)
		{
			arrayToReturn[fortress.fPartsBuildState[i].tribeIndex] += fortress.fPartsBuildState[i].builder;
		}
	}
	arrayToReturn[2] = fortress.fFood;
	arrayToReturn[3] = fortress.fWater;
	arrayToReturn[4] = fortress.fMaterials;

	return(arrayToReturn);
}

int* GetTribesExpData(int index, int expIndex)
{
	int *arrayToReturn = malloc(6 * sizeof(int));
	arrayToReturn[0] = expIndex;
	arrayToReturn[1] = players[index].playerTribes.onGoingExpedition[expIndex].actualTurn;
	arrayToReturn[2] = players[index].playerTribes.onGoingExpedition[expIndex].habitantLeft;
	arrayToReturn[3] = players[index].playerTribes.onGoingExpedition[expIndex].foodGained;
	arrayToReturn[4] = players[index].playerTribes.onGoingExpedition[expIndex].waterGained;
	arrayToReturn[5] = players[index].playerTribes.onGoingExpedition[expIndex].materialsGained;

	return(arrayToReturn);
}

int* GetTribesIntel(int index, int buildIndex, int intelNameID)
{
	int* arrayToReturn = malloc(4 * sizeof(int));
	
	arrayToReturn[0] = intelNameID;
	arrayToReturn[1] = buildIndex;
	arrayToReturn[2] = players[index].playerTribes.intel[buildIndex].materialsToBuild;
	arrayToReturn[3] = intelNameID == 4 ? 15 : 30;

	return(arrayToReturn);
}

int* GetFortressIntel(int buildFortressIndex, int intelNameID)
{
	int* arrayToReturn = malloc(9 * sizeof(int));

	arrayToReturn[0] = intelNameID;
	arrayToReturn[1] = buildFortressIndex;
	arrayToReturn[2] = fortress.fPartsBuildState[buildFortressIndex].tribeIndex + 1;
	arrayToReturn[3] = fortress.fPartsBuildState[buildFortressIndex].builder;
	arrayToReturn[4] = fortress.fPartsBuildState[buildFortressIndex].knowledge.habitantsToFullBuildPotential;
	arrayToReturn[5] = fortress.fPartsBuildState[buildFortressIndex].materialsUsed;
	arrayToReturn[6] = fortress.fPartsBuildState[buildFortressIndex].knowledge.materialsToBuild;
	arrayToReturn[7] = fortress.fPartsBuildState[buildFortressIndex].turnProgress;
	arrayToReturn[8] = fortress.fPartsBuildState[buildFortressIndex].knowledge.numberOfTurnToBuild;

	return(arrayToReturn);
}

bool CheckIfCanBuildUpgrade(int playerID, int index)
{
	int gain = players[playerID].playerTribes.intel[index].name - 3;
	if (players[playerID].playerTribes.resourcesGenerationPerTurn[gain] != 0)
	{
		return false;
	}
	else
	{
		return(players[playerID].playerTribes.materials >= players[playerID].playerTribes.intel[index].materialsToBuild);
	}
}


#if _DEBUG
int GiveFortressUpgrades(int idUpgrades)
{
	for (int i = 0; i < 3; i++)
	{
		if (fortress.fPartsBuildState[i].knowledge.name == idUpgrades)
		{
			return 0;
		}
		
		if (fortress.fPartsBuildState[i].knowledge.name == Empty)
		{
			AddKnowledgeToFort(0, &fortress.fPartsBuildState[i], GetProto(idUpgrades));
			return i+1;
		}
	}

	return 0;
}

void GiveResources(void(*cbConso)(int*, int))
{
	for (int i = 0; i < 2; i++)
	{
		players[i].playerTribes.food += 100;
		players[i].playerTribes.water += 100;
		players[i].playerTribes.materials += 100;
	}

	cbConso(GetTribesData(0), 0);
	cbConso(GetTribesData(1), 1);
	cbConso(ConsumptionCalculation(players[0].playerTribes, 1, true), 0 + 3);
	cbConso(ConsumptionCalculation(players[1].playerTribes, 1, true), 1 + 3);
}

#endif // _DEBUG
