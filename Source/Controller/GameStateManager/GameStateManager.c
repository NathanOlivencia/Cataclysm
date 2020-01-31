#include "GameStateManager.h"
#include "../HUD/HUDController.h"
#include "../../Model/GameLogicManager/GameLogicManager.h"
#include "../MainSceneController/MainSceneController.h"
#include <time.h>
#include <Windows.h>

GameState gameState;

void ResetGameState()
{
	srand((unsigned int)time(NULL));
	
	gameState.onGoingGameStatus.gameIsPaused = false;
	gameState.onGoingGameStatus.hasMadeAnAction = false;
	gameState.onGoingGameStatus.hasExpToHandle = false;
	gameState.onGoingGameStatus.hasToChangeState = false;
	gameState.onGoingGameStatus.hasOpenExpPanel = false;
	gameState.onGoingGameStatus.hasOpenBuildPanel = false;
	gameState.onGoingGameStatus.hasOpenFortressPanel = false;
	gameState.onGoingGameStatus.gameIsFinishing = false;
	gameState.onGoingGameStatus.hasBuilderToManage = false;
	gameState.onGoingGameStatus.hasUsedLastChance = false;
	gameState.onGoingGameStatus.hisManagingExp = -1;
	gameState.onGoingGameStatus.actualPlayerID = -1;
	gameState.onGoingGameStatus.turn = 0;
	gameState.onGoingGameStatus.playerSelectionID = -1;
}

void InitGameState()
{
	gameState.isReload = false;
	gameState.actualscene = mainMenu;
	ResetGameState();
}


#if _DEBUG
void TurnIncr()
{
	gameState.onGoingGameStatus.turn++;
	printf("turn : %d\n", gameState.onGoingGameStatus.turn);
	UpdateTurnDisplayHUD(gameState.onGoingGameStatus.turn);
}
#endif

void GameIsLoose()
{
	gameState.actualscene = gameOverScene;
	ReloadAnimLoading();
}

int GetActualScene()
{
	return(gameState.actualscene);
}

GameState GetActualState()
{
	return gameState;
}

DWORD WINAPI WaitForAnim(void *data)
{
	while (!ReturnEndGameAnimIsDone())
	{
		Sleep(200);
	}
	Sleep(1000);
	gameState.actualscene = gameOverScene;
	ReloadAnimLoading();
	return 0;
}

//Private Functions
void UpdateGameState()
{
	gameState.onGoingGameStatus.hasMadeAnAction = false;
	gameState.onGoingGameStatus.playerSelectionID = -1;
	
	ChangeExPanelState(gameState.onGoingGameStatus.actualPlayerID, 0);
	gameState.onGoingGameStatus.hasOpenExpPanel = false;
	
	ChangeFortressPanelState(0);
	gameState.onGoingGameStatus.hasOpenFortressPanel = false;

	gameState.onGoingGameStatus.actualPlayerID = (gameState.onGoingGameStatus.actualPlayerID + 1) % 2;

	if (gameState.onGoingGameStatus.actualPlayerID == 0)
	{
		ProcessEndTurnSimulation(gameState.onGoingGameStatus.turn, UpdateText, UpdateIntelsPanel, UpdateFortressParts, GameIsLoose);

		gameState.onGoingGameStatus.turn++;
		UpdateTurnDisplayHUD(gameState.onGoingGameStatus.turn);
		SetInfoText(3);

		if (gameState.onGoingGameStatus.turn == 11)
		{
			ChangeMap();
		}

		if (gameState.onGoingGameStatus.turn == 21)
		{
			LoadCataclysmAnim(2);
			gameState.onGoingGameStatus.gameIsFinishing = true;
			SwitchToCata(ProcessCataclysmSimulation(UpdateText));
			HANDLE waitForAnim = CreateThread(NULL, 0, WaitForAnim, NULL, 0, NULL);
		}
		else
		{
			gameState.onGoingGameStatus.hasToChangeState = true;
			ProcessBeginTurnSimulation(gameState.onGoingGameStatus.turn, UpdateText);
			gameState.onGoingGameStatus.hasToChangeState = false;
		}
	}

	ChangeNewTurnHUD(gameState, CheckIfCanUseLastChance(gameState.onGoingGameStatus.actualPlayerID));

	ResetExpeditionsHUD(gameState.onGoingGameStatus.actualPlayerID);

	ChangeActiveGod(gameState.onGoingGameStatus.actualPlayerID);

	if (CheckIfHasBuilderToManage(gameState.onGoingGameStatus.actualPlayerID, &gameState.onGoingGameStatus.playerSelectionID))
	{
		gameState.onGoingGameStatus.hasBuilderToManage = true;
		ChangeExpChoiceState(2);
	}
}

DWORD WINAPI LoadAllData(void* data)
{
	InitGodSelectionMenu(sfVideoMode_getDesktopMode(), gameState.isReload);
	ChangeLoadingProgress(0.25);
	HANDLE myThread42 = CreateThread(NULL, 0, UnLoadAnimFrameByFrame, (void*)0, 0, NULL);
	InitMainScene(gameState.isReload);
	ChangeLoadingProgress(0.5);
	InitData(gameState.isReload);
	ChangeLoadingProgress(0.25);

	while (!GetLoadingProgess())
	{
		Sleep(1);
	}

	if (!gameState.isReload)
	{
		InitMainSceneHUD(sfVideoMode_getDesktopMode());
	}
	else
	{
		ResetGameState();
		ResetMainSceneHud();
	}

	gameState.onGoingGameStatus.actualPlayerID = 0;
	gameState.onGoingGameStatus.turn = 1;
	ChangeNewTurnHUD(gameState, false);
	gameState.actualscene = godSelection;

	HANDLE myThread2 = CreateThread(NULL, 0, UnLoadAnim, NULL, 0, NULL);
	ChangeLoadingProgress(-1.0);

	DWORD holder;
	do
	{
		GetExitCodeThread(myThread2, &holder);
		Sleep(5);
	} while (holder == STILL_ACTIVE);
	
	Sleep(500);
	LoadMainSceneAnimSummer(gameState.isReload);

	if (!gameState.isReload)
	{
		gameState.isReload = true;
	}
	return 0;
}

void HandleMainMenu(int value)
{
	switch (value)
	{
		//Starting New Game	
		case 0:
			gameState.actualscene = loadingScene;
			HANDLE myThread = CreateThread(NULL, 0, LoadAllData, NULL, 0, NULL);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			exit(0);
			break;
	}
}

void CheckExpToManage()
{
	gameState.onGoingGameStatus.hasExpToHandle = CheckExp(gameState.onGoingGameStatus.actualPlayerID);
	gameState.onGoingGameStatus.playerSelectionID = -1;

	if (!gameState.onGoingGameStatus.hasExpToHandle)
	{
		//Means that the request is not queued
		if (!gameState.onGoingGameStatus.hasToChangeState)
		{
			UpdateGameState();
		}
	}
	else
	{
		gameState.onGoingGameStatus.hasToChangeState = false;
		gameState.onGoingGameStatus.hasOpenFortressPanel = false;
		ChangeFortressPanelState(0);
		gameState.onGoingGameStatus.hasOpenExpPanel = true;
		ChangeExPanelState(gameState.onGoingGameStatus.actualPlayerID, 1);
	}
	gameState.onGoingGameStatus.hasOpenBuildPanel = false;
}

void GetDataForm(int* dataForm)
{	
	bool dataWentWell;
	
	if (gameState.onGoingGameStatus.playerSelectionID == 0)
	{
		dataWentWell = ManagerInput(gameState, gameState.onGoingGameStatus.playerSelectionID, dataForm, UpdateText, UpdateExpeditionsPanels, UpdateCardDisplayerPanel);
		gameState.onGoingGameStatus.hasToChangeState = true;
	}
	else if (gameState.onGoingGameStatus.playerSelectionID == 1)
	{
		dataWentWell = ManagerInput(gameState, gameState.onGoingGameStatus.playerSelectionID, dataForm, UpdateText, UpdateIntelsPanel, UpdateCardDisplayerPanel);
		gameState.onGoingGameStatus.hasToChangeState = true;
	}
	else
	{
		dataWentWell = ManagerInput(gameState, gameState.onGoingGameStatus.playerSelectionID, dataForm, UpdateText, UpdateTribesUpgradesState, NULL);
	}
	
	if (dataWentWell)
	{
		gameState.onGoingGameStatus.hasToFillAForm = false;
		CheckExpToManage();
	}
	else
	{
		printf("Input send?\n");
		system("PAUSE");
	}
}

void UpdateState(sfRenderWindow* mainWindow, float deltaTime)
{
	switch (gameState.actualscene)
	{
		case(mainMenu):
		case(pause):
		case(gameOverScene):
			UpdateHUD(mainWindow, &gameState, NULL, NULL, deltaTime, NULL);
			break;
		case (mainScene):
			UpdateMainScene(deltaTime);
			UpdateHUD(mainWindow, &gameState, GetDataForm, NULL, deltaTime, UpdateGameState);
			if (gameState.onGoingGameStatus.hasUsedLastChance)
			{
				UpdateText(GetTribesData(gameState.onGoingGameStatus.actualPlayerID), gameState.onGoingGameStatus.actualPlayerID);
				gameState.onGoingGameStatus.hasUsedLastChance = false;
				gameState.onGoingGameStatus.hasMadeAnAction = true;
				CheckExpToManage();
			}
			break;
		case(loadingScene):
		case (godSelection):
			UpdateHUD(mainWindow, &gameState, SendTribesDatas, SetGodPlayer, deltaTime, UpdateGameState);
			if (gameState.actualscene == mainScene)
			{
				UpdateEveryInfos(InitTextHUD);
			}
				break;

		default:
			printf("Error\n GameStateManager.c");
			break;
	}
}

void ManageStateInputKeyboard()
{
	switch (gameState.actualscene)
	{
		case(mainMenu):
			return;
		case(mainScene):
			gameState.onGoingGameStatus.gameIsPaused = true;
			gameState.actualscene = pause;
			return;
		case(pause):
			if (gameState.onGoingGameStatus.gameIsPaused)
			{
				gameState.onGoingGameStatus.gameIsPaused = false;
				gameState.actualscene = mainScene;
			}
			return;
		default:
			return;
	}
}

DWORD WINAPI WaitToChangeScene(void* data)
{
	Sleep(3000);
	int temp = data == 0 ? 0 : 1;
	gameState.actualscene = temp;
	return 0;
}

void ManageState(sfVector2i mousePos)
{
	int value;
	switch (gameState.actualscene)
	{
		case (mainMenu):
			 value = CheckHitBoxWithHUD(mousePos, gameState);
			 if (value != -1)
			 {
				HandleMainMenu(value);
			 }
			break;
		case (mainScene):			
			value = CheckHitBoxWithHUD(mousePos, gameState);
			if (value != -1)
			{
				if (!gameState.onGoingGameStatus.hasToFillAForm)
				{
					if (!gameState.onGoingGameStatus.hasMadeAnAction)
					{
						if (value == 3)
						{
							gameState.onGoingGameStatus.hasOpenFortressPanel = !gameState.onGoingGameStatus.hasOpenFortressPanel;
							ChangeFortressPanelState(-1);
							return;
						}
						else if (value == 4)
						{
							gameState.onGoingGameStatus.hasOpenBuildPanel = !gameState.onGoingGameStatus.hasOpenBuildPanel;
							ChangeBuildPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
							if (gameState.onGoingGameStatus.hasOpenExpPanel)
							{
								gameState.onGoingGameStatus.hasOpenExpPanel = false;
								ChangeExPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
							}
							return;
						}
						else if (value == 5)
						{
							ManagerInput(gameState, 5, NULL, UpdateText, ProceedDiceThrow, NULL);
							return;
						}
						else if (value == 6)
						{
							gameState.onGoingGameStatus.hasOpenExpPanel = !gameState.onGoingGameStatus.hasOpenExpPanel;
							ChangeExPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
							if (gameState.onGoingGameStatus.hasOpenBuildPanel)
							{
								gameState.onGoingGameStatus.hasOpenBuildPanel = false;
								ChangeBuildPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
							}
							return;
						}

						if (gameState.onGoingGameStatus.hasBuilderToManage)
						{
							GetBackBuilderLink(gameState.onGoingGameStatus.actualPlayerID, value, gameState.onGoingGameStatus.playerSelectionID, UpdateText);

							if (!CheckIfHasBuilderToManage(gameState.onGoingGameStatus.actualPlayerID, &gameState.onGoingGameStatus.playerSelectionID))
							{
								gameState.onGoingGameStatus.hasBuilderToManage = false;
								gameState.onGoingGameStatus.playerSelectionID = -1;
								ChangeExpChoiceState(2);
							}
							return;
						}
						
						if (gameState.onGoingGameStatus.hasOpenFortressPanel)
						{			
							if (value == 0)
							{
								gameState.onGoingGameStatus.hasOpenFortressPanel = !gameState.onGoingGameStatus.hasOpenFortressPanel;
								ChangeFortressPanelState(-1);
							}
							else
							{
								if (CheckInput(gameState, 3, UpdateForm))
								{
									gameState.onGoingGameStatus.playerSelectionID = 3;
									gameState.onGoingGameStatus.hasMadeAnAction = true;
									gameState.onGoingGameStatus.hasToFillAForm = true;

									if (gameState.onGoingGameStatus.hasOpenBuildPanel)
									{
										gameState.onGoingGameStatus.hasOpenBuildPanel = false;
										ChangeBuildPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
									}
								}
							}
							return;
						}

						if (gameState.onGoingGameStatus.hasOpenBuildPanel)
						{
							if (gameState.onGoingGameStatus.hisManagingExp == -1)
							{
								if (CheckIfCanBuildUpgrade(gameState.onGoingGameStatus.actualPlayerID, value))
								{
									ChangeExpChoiceState(1);
									gameState.onGoingGameStatus.hisManagingExp = value;
								}
								else
								{
									SetInfoText(1);
								}
							}
							else
							{
								ChangeExpChoiceState(1);
								if (value == 0)
								{
									if (ManagerInput(gameState, 4, &gameState.onGoingGameStatus.hisManagingExp, UpdateText, UpdateTribesUpgradesState, NULL))
									{
										BuildsPanelIsBuild(gameState.onGoingGameStatus.hisManagingExp, gameState.onGoingGameStatus.actualPlayerID);
										gameState.onGoingGameStatus.hasMadeAnAction = true;
										gameState.onGoingGameStatus.hasOpenBuildPanel = false;
										ChangeBuildPanelState(gameState.onGoingGameStatus.actualPlayerID, -1);
										CheckExpToManage();
									}									
								}
								gameState.onGoingGameStatus.hisManagingExp = -1;
							}
							return;
						}

						if (value >= 0 && value < 3)
						{
							if (CheckInput(gameState, value, UpdateForm))
							{
								gameState.onGoingGameStatus.playerSelectionID = value;
								gameState.onGoingGameStatus.hasMadeAnAction = true;
								gameState.onGoingGameStatus.hasToFillAForm = true;
							}
							else
							{
								if (value == 0)
								{
									SetInfoText(2);
								}
							}
						}
					}
					else
					{
						if (gameState.onGoingGameStatus.hisManagingExp == -1)
						{
							ChangeExpChoiceState(0);
							gameState.onGoingGameStatus.hisManagingExp = value;
						}
						else
						{
							ChangeExpChoiceState(0);
							gameState.onGoingGameStatus.hasExpToHandle = ManagerExpeditions(gameState, value, &gameState.onGoingGameStatus.hasToChangeState, UpdateExpeditionsPanels, UpdateCardDisplayerPanel);
							gameState.onGoingGameStatus.hisManagingExp = -1;
						}
					}
				}
				else
				{
					SendInputToForm(gameState.onGoingGameStatus.playerSelectionID, value);
				}
			}
			break;
		case pause:
			value = CheckHitBoxWithHUD(mousePos, gameState);
			if (value != -1)
			{
				if (value == 0)
				{
					gameState.onGoingGameStatus.gameIsPaused = false;
					gameState.actualscene = mainScene;
					
				}
				else if (value == 1)
				{
					gameState.onGoingGameStatus.gameIsPaused = false;
					HANDLE FadeDelay = CreateThread(NULL, 0, WaitToChangeScene, NULL, 0, NULL);
				}
				else
				{
					exit(0);
				}
			}
			break;
		case godSelection:
			value = CheckHitBoxWithHUD(mousePos, gameState);
			if (value != -1)
			{
				if (value == 10)
				{
					HANDLE FadeDelay = CreateThread(NULL, 0, WaitToChangeScene, (void*)0, 0, NULL);
				}
				else
				{
					if (SelectGodAndStarter(value))
					{
						HANDLE FadeDelay = CreateThread(NULL, 0, WaitToChangeScene, (void*)1, 0, NULL);
					}
				}
			}
			break;
		case gameOverScene:
			value = CheckHitBoxWithHUD(mousePos, gameState);
				if (value == 0)
				{
					if (gameState.onGoingGameStatus.gameIsFinishing)
					{
						HANDLE thread4622 = CreateThread(NULL, 0, UnLoadAnimCata, NULL, 0, NULL);
					}
					HANDLE FadeDelay = CreateThread(NULL, 0, WaitToChangeScene, NULL, 0, NULL);
				}
				else if (value == 1)
				{
					exit(0);
				}

		default:
			break;
	}
}
