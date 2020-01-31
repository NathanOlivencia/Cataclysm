#ifndef _STATE_H
#define _STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
	mainMenu,
	mainScene,
	pause,
	godSelection,
	gameOverScene,
	loadingScene
} Scene;

typedef struct
{
	int actualPlayerID;
	bool hasMadeAnAction;
	bool hasToFillAForm;
	bool hasExpToHandle;
	bool hasToChangeState;
	bool hasOpenExpPanel;
	bool hasOpenBuildPanel;
	bool hasOpenFortressPanel;
	bool hasBuilderToManage;
	bool gameIsPaused;
	bool gameIsFinishing;
	bool hasUsedLastChance;
	int hisManagingExp;
	int playerSelectionID;
	int turn;
} GameStatus;

typedef struct
{
	GameStatus onGoingGameStatus;
	Scene actualscene;
	bool isReload;
} GameState;


#endif // _STATE_H
