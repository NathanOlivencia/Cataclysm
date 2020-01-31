#include "GameLogic.h"

//Check if both Tribes are dead else continue to next turn
bool CheckGameGoToNextTurn(struct Player players[])
{
  for (int i = 0; i < 2; ++i)
  {
    if(!players[i].isDead && players[i].playerTribes.habitants == 0)
    {
      players[i].isDead = true;
    }
    else if (!players[i].isDead)
    {
      return true;
    }
  }

  return false;
}

//Print out Both Tribes actual stats to screen
void ShowTurnStatus(struct Player players[], Fortress fort)
{
  PrintToScreenTribes(players[0].playerTribes, 1);
  PrintToScreenTribes(players[1].playerTribes, 2);
  //PrintToScreenFortress(fort);
}

//Check if Tribes died after last calculation
void CheckIfIsDead(struct Player players[])
{
   for (int i = 0; i < 2; ++i)
   {
     if(!players[i].isDead)
     {
		 if (players[i].playerTribes.habitants == 0 || players[i].playerTribes.habitants <= 5 && players[i].playerTribes.food == 0 && players[i].playerTribes.water == 0 && players[i].playerTribes.materials == 0)
		 {
			players[i].isDead = true;
			players[i].playerTribes.food = 0;
			players[i].playerTribes.water = 0;
			players[i].playerTribes.materials = 0;
		}
     }
   }
}

//Check if there are free expeditions slot in the array referencing ongoing expeditions of the tribes
//if so return the index of the empty slot if the user want to start a new one else return -1
//to indicate that the array is full and therefore the user can not start a new expedition
int CheckAvailableExpeditionSlot(OnGoingExpedition playersExpeditionArray[])
{
	for (int i = 0; i < 3; i++)
	{
		if (playersExpeditionArray[i].actualTurn == t0)
		{
			return(i);
		}	
	}

	return -1;
}

//Simply simulate one of turn of a particular expedition passed as parameter
void SimulateTurnOfExpeditions(int* dataToReturn, OnGoingExpedition *playersExpeditionArray, ExpeditionCards* expCardsPile)
{
	int *turnEffects = NULL;

	ExpeditionCards tempCard = PickExpeditionCard(expCardsPile, playersExpeditionArray->actualTurn + 1);

	turnEffects = ExpeditionCardEffect(tempCard, playersExpeditionArray->habitantLeft);

	dataToReturn[0] = tempCard.type;
	dataToReturn[1] = turnEffects[0]; // tempCard.habitantFluctuation;
	dataToReturn[2] = turnEffects[1]; //tempCard.foodGained;
	dataToReturn[3] = turnEffects[2]; //tempCard.waterGained;
	dataToReturn[4] = turnEffects[3]; //tempCard.materialGained;

	playersExpeditionArray->habitantLeft += turnEffects[0];
	playersExpeditionArray->foodGained += turnEffects[1];
	playersExpeditionArray->waterGained += turnEffects[2];
	playersExpeditionArray->materialsGained += turnEffects[3];

	ClampBetweenZAndCap(&playersExpeditionArray->habitantLeft, -1);
	ClampBetweenZAndCap(&playersExpeditionArray->foodGained, -1);
	ClampBetweenZAndCap(&playersExpeditionArray->materialsGained, -1);

	playersExpeditionArray->actualTurn = (TurnOfExpedition)(playersExpeditionArray->actualTurn + 1);
	playersExpeditionArray->isManaged = true;
}

//Fill up a new expedition that we add to the array that register all ongoing expeditions
void startNewExpedition(struct Player *player, int indexToADD, int habitantGoneToExp, ExpeditionCards* expCardsPile)
{
	player->playerTribes.onGoingExpedition[indexToADD].habitantLeft = habitantGoneToExp;
	SimulateTurnOfExpeditions(player->lastExpStats, &player->playerTribes.onGoingExpedition[indexToADD], expCardsPile);

	for (int i = 0; i < 6; i++)
	{
		printf("Value lastExpStat before : %d\n", player->lastExpStats[i]);
	}
	player->playerTribes.habitants -= habitantGoneToExp;
}

//Manage the expedition system
bool ExpeditionsManager(int* expDataToreturn, Tribes* playerTribe, int expGoingBackRessources[], int idExp, int input, ExpeditionCards* expCardsPile)
{
	if (!HasExpeditionsOnGoing(playerTribe->onGoingExpedition))
	{
		for (int i = 0; i < 3; i++)
		{
			if (playerTribe->onGoingExpedition[i].actualTurn != t0)
			{
				playerTribe->onGoingExpedition[i].isManaged = false;
			}
		}
		return false;
	}

	int indexArrayToReturn = 0;
	int selectionAvailable = 0;
	printf("Gestion des expeditions en cours : \n");
	printf("___________________________________________________\n");

	if (input == 1 || playerTribe->onGoingExpedition[idExp].actualTurn == t3)
	{
		printf("L'expedition %d est sur le chemin du retour.\n", idExp+1);

		expGoingBackRessources[0] += playerTribe->onGoingExpedition[idExp].habitantLeft;
		expGoingBackRessources[1] += playerTribe->onGoingExpedition[idExp].foodGained;
		expGoingBackRessources[2] += playerTribe->onGoingExpedition[idExp].waterGained;
		expGoingBackRessources[3] += playerTribe->onGoingExpedition[idExp].materialsGained;

		if (playerTribe->onGoingExpedition[idExp].foodGained == 0 && playerTribe->onGoingExpedition[idExp].waterGained == 0 && playerTribe->onGoingExpedition[idExp].materialsGained == 0)
		{
			expDataToreturn[0] = 12;
		}
		else
		{
			expDataToreturn[0] = 11;
		}
		expDataToreturn[1] = playerTribe->onGoingExpedition[idExp].habitantLeft;
		expDataToreturn[2] = playerTribe->onGoingExpedition[idExp].foodGained;
		expDataToreturn[3] = playerTribe->onGoingExpedition[idExp].waterGained;
		expDataToreturn[4] = playerTribe->onGoingExpedition[idExp].materialsGained;

		playerTribe->onGoingExpedition[idExp].isManaged = true;
		playerTribe->onGoingExpedition[idExp].actualTurn = t0;

		playerTribe->hasExpComingBack = true;


		ResetExpeditions(&playerTribe->onGoingExpedition[idExp]);
	}
	else
	{
		SimulateTurnOfExpeditions(expDataToreturn, &playerTribe->onGoingExpedition[idExp], expCardsPile);
		if (playerTribe->onGoingExpedition[idExp].habitantLeft == 0)
		{
			ResetExpeditions(&playerTribe->onGoingExpedition[idExp]);
			playerTribe->onGoingExpedition[idExp].isDead = true;
		}
	}

	if (HasExpeditionsOnGoing(playerTribe->onGoingExpedition))
	{
		return true;
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (playerTribe->onGoingExpedition[i].actualTurn != t0)
		{
			playerTribe->onGoingExpedition[i].isManaged = false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (playerTribe->onGoingExpedition[i].actualTurn == t0 && playerTribe->onGoingExpedition[i].isManaged)
		{
			ResetExpeditions(&playerTribe->onGoingExpedition[i]);
		}
	}

	return false;
	
}

//Manage the action that each Tribe can do each turn 
bool ActionManager(struct Player players[], Fortress *fortress, int playerID, int selection, int *dataInput, ExpAndKnowPilePack cardsPile)
{
	int indexToCreateExpedition = -1;
	if (!players[playerID].isDead)
	{
		if (selection == 0)
		{
			indexToCreateExpedition = CheckAvailableExpeditionSlot(players[playerID].playerTribes.onGoingExpedition);
			if (indexToCreateExpedition == -1)
			{
				printf("Vous avez deja 3 expedition en cours \n");
				return false;
			}
		}
	}

	switch (selection)
	{
		case -1:
			printf("Cette tribu est morte. \n");
			break;
		case 0:
			printf("___________________________________________________\n");
			printf("Vous partez en expedition pour trouver des ressources \n");
			printf("***************************************************\n");

			players[playerID].lastExpCreatedID = indexToCreateExpedition;
			startNewExpedition(&players[playerID], indexToCreateExpedition, dataInput[0], cardsPile.expeditionCardsPile);

			//Withdraw Habitant that have been sent to the expedition

			break;
		case 1:
			printf("___________________________________________________\n");
			printf("Vous partez en expedition pour trouver un savoir \n");
			printf("***************************************************\n");

			int result;
			bool pickingDone = false;
			int itCpt = 0;
			int saveID;
			do
			{
				KnowledgeCards tempCards = PickKnowledgeCard(cardsPile.knowledgeCardsPile, 0, &saveID);
				
				if (itCpt >= 3)
				{
					result = -1;
					saveID = -1;
				}
				else
				{
					result = tempCards.name;
				}
				
				players[playerID].lastExpStats[0] = tempCards.name;
				printf("name: %d\n", tempCards.name);
				players[playerID].lastExpStats[1] = tempCards.habitantsToFullBuildPotential;
				players[playerID].lastExpStats[2] = tempCards.materialsToBuild;
				players[playerID].lastExpStats[3] = tempCards.numberOfTurnToBuild;

				if (result >= 3 && result <= 5)
				{
					if (AddKnowledgeToTribe(&players[playerID].playerTribes, result))
					{
						players[playerID].lastIntelFoundNameID = result;
						pickingDone = true;
					}
					else
					{
						players[playerID].lastIntelFoundNameID = -1;
					}
				}
				else if (result >= 0 && result <= 2)
				{
					if (AddKnowledgeToFort(playerID, fortress->fPartsBuildState, GetProto(result)))
					{
						players[playerID].lastIntelFoundNameID = result;
						pickingDone = true;
					}
					else
					{
						players[playerID].lastIntelFoundNameID = -1;
					}
				}
				else
				{
					players[playerID].lastIntelFoundNameID = -1;
					pickingDone = true;
				}
				itCpt++;
			} while (!pickingDone);

			removeCardFromPile(cardsPile.knowledgeCardsPile, saveID);
			break;
		case 2:
			printf("___________________________________________________\n");
			printf("Vous effectuez une donation a l'autre tribu \n");
			printf("***************************************************\n");

			int valueToWithDraw[4];
			int j = (playerID + 1) % 2;

			GiveRessourceToEntities(dataInput, valueToWithDraw, players[playerID].playerTribes, true);

			ApplyChangeToTribesCalculs(valueToWithDraw, &players[playerID].playerTribes, players[playerID].isDead);
			
			ApplyChangeToTribesCalculs(dataInput, &players[j].playerTribes, players[j].isDead);
			break;
		case 3:
			printf("___________________________________________________\n");
			printf("Vous envoyer des ressources vers la forteresse \n");
			printf("***************************************************\n");
			int toWithDraw[4];

			GiveRessourceToEntities(dataInput, toWithDraw, players[playerID].playerTribes, false);

			ApplyChangeToTribesCalculs(toWithDraw, &players[playerID].playerTribes, players[playerID].isDead);
			
			AddResourcesToFortress(playerID, dataInput, fortress);

			break;
		case 4:
			printf("___________________________________________________\n");
			printf("vous decidez de construire une amelioration \n");
			printf("***************************************************\n");
			
			ApplyKnowledgeToTribe(&players[playerID].playerTribes, dataInput[0]);
			break;
		case 5:
			printf("___________________________________________________\n");
			printf("Vous avez choisi d'utiliser votre dernière chance\n");
			printf("***************************************************\n");
			printf("Lancez un des. Suivant le resultat vous pourrez obtenir :\n");
			printf("1 - 3 : 10 habitants , 15 nourritures , 15 eaux , 15 materiaux\n");
			printf("4 - 5 : 15 habitants , 20 nourritures , 20 eaux , 20 materiaux\n");
			printf("6 : 20 habitants , 30 nourritures , 30 eaux , 30 materiaux\n");
			printf("---------------------------------------------------\n");
			printf("Appuyez sur une touche pour lancer le des\n");

			int randResult = 1 + rand() % 6;

			int lastChanceGain[4];

			printf("resultat du lance : %d \n", randResult);

			if (randResult == 1 || randResult == 2 || randResult == 3)
			{
				lastChanceGain[0] = 10;
				lastChanceGain[1] = 15;
				lastChanceGain[2] = 15;
				lastChanceGain[3] = 15;
			}
			else if (randResult == 4 || randResult == 5)
			{
				lastChanceGain[0] = 15;
				lastChanceGain[1] = 20;
				lastChanceGain[2] = 20;
				lastChanceGain[3] = 20;
			}
			else
			{
				lastChanceGain[0] = 20;
				lastChanceGain[1] = 30;
				lastChanceGain[2] = 30;
				lastChanceGain[3] = 30;
			}

			players[playerID].diceResult = randResult;
			printf("Ajout de %d habitants , %d nourritures , %d eaux , %d materiaux dans la tribu %d\n", lastChanceGain[0], lastChanceGain[1], lastChanceGain[2], lastChanceGain[3], playerID + 1);
			ApplyChangeToTribesCalculs(lastChanceGain, &players[playerID].playerTribes, false);
			players[playerID].LastChanceUsed = true;
			break;
	}

	printf("Statut apres que la tribu %d ait jouee\n", playerID + 1);
	printf("***************************************************\n");
	ShowTurnStatus(players, *fortress);

	return true;
}

void EventCalculation(struct Player players[], EventCard* eventCardPile)
{
	printf("Calcul effet de l'evenement \n");
	printf("***************************************************\n");

	EventCard selected = PickEventCard(eventCardPile);

	int turnEffects[4] = {
		-selected.villagersLost,
		-selected.foodLost,
		-selected.waterLost,
		-selected.materialsLost
	};
	
	for (int i = 0; i < 4; i++)
	{
		printf("turn effect : %d\n", turnEffects[i]);
	}
	
	ApplyChangeToTribesCalculs(turnEffects, &players[0].playerTribes, players[0].isDead);
	ApplyChangeToTribesCalculs(turnEffects, &players[1].playerTribes, players[1].isDead);
	CheckIfIsDead(players);
}


//Calculation of ressource consumption at then end of the turn
void EndTurnCalculation(int turn, struct Player players[], Fortress *fortress)
{
	//ENDTURN CALCULATION
	printf("Calcul de la consommation de ressources des habitants\n");
	printf("***************************************************\n");
	ApplyConsumptionCalculation(&players[0].playerTribes, turn, players[0].isDead);
	ApplyConsumptionCalculation(&players[1].playerTribes, turn, players[1].isDead);
	CheckIfIsDead(players);

	ShowTurnStatus(players, *fortress);

	FortressCalcul(fortress);

	printf("Fin du tour %d \n", turn);

	printf("***************************************************\n");
}




//Add all expedition gains
void AddExpeditionGain(struct Player *players, int saveOfExpedition[])
{
	ApplyChangeToTribesCalculs(saveOfExpedition, &players->playerTribes, players->isDead);
	players->hasExpComingBack = false;
}

//Turn calculation before both player can manage their tribes
//Typicaly it's where we procces upgrades benefits
void BeginTurnCalculation(int turn, struct Player players[], Fortress *fortress)
{
	for (int i = 0; i < 3; i++)
	{
		if (players[0].playerTribes.resourcesGenerationPerTurn[i] != 0 || players[1].playerTribes.resourcesGenerationPerTurn[i] != 0)
		{
			printf("Ajout des ressources gagner grace aux ameliorations.\n");
			printf("***************************************************\n");
			UpgradeGainCalculation(&players[0].playerTribes);
			UpgradeGainCalculation(&players[1].playerTribes);
			ShowTurnStatus(players, *fortress);
			return;
		}
	}
	
}