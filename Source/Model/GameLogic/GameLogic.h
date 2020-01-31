#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Fortress/Fortress.h"
#include "../Tribes/Tribes.h"
#include "../Utilities/Utilities.h"
#include "../Cards/Cards.h"

//Struct with Tribes stat and bool to determines if player is dead or not
struct Player
{
	Tribes playerTribes;
	bool LastChanceUsed;
	bool isDead;
	bool hasExpComingBack;
	int lastExpCreatedID;
	int lastIntelFoundNameID;
	int* lastExpStats;
	int diceResult;
};

bool CheckGameGoToNextTurn(struct Player players[]);
void ShowTurnStatus(struct Player players[], Fortress fort);
void CheckIfIsDead(struct Player players[]);
int CheckAvailableExpeditionSlot(OnGoingExpedition playersExpeditionArray[]);
bool ExpeditionsManager(int* expDataToreturn, Tribes* playerTribe, int expGoingBackRessources[], int idExp, int input, ExpeditionCards* expCardsPile);
bool ActionManager(struct Player players[], Fortress *fortress, int playerID, int selection, int *dataInput, ExpAndKnowPilePack cardsPile);
void BeginTurnCalculation(int turn, struct Player players[], Fortress *fortress);
void EventCalculation(struct Player players[], EventCard* eventCardPile);
void EndTurnCalculation(int turn, struct Player players[], Fortress *fortress);
void AddExpeditionGain(struct Player *players, int saveOfExpedition[]);
//void ProcessTurn(int turn, struct Player players[], Fortress *fortress, int playerID, int selection);

#endif