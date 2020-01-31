#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "../../Controller/GameStateManager/State.h"

//Handle Different function to call and aslo keep the data of the game
void InitData(bool isReload);
void SendTribesDatas(int* data);
void UpdateEveryInfos(void(*callback)(int*, int));
void SendTribesDatas(int* data);
bool CheckInput(GameState actualState, int inputIndex, void(*cbPopUpForm)(int, int*));
bool CheckExp(int playerID);
bool ManagerInput(GameState actualState, int inputIndex, int *dataField, void(*cbHUDVisualUpdate)(int*, int), void(*cbMainSceneVisualUpdate)(int*, int), void(*cbHudDisplayCard)(int*, int));
bool ManagerExpeditions(GameState actualState, int value, bool *hasToChange, void(*cbUpdateExpPanel)(int*, int), void(*cbHudDisplayCard)(int*, int));
void ProcessBeginTurnSimulation(int turn, void(*cbHUDVisualUpdate)(int*, int));
bool ProcessCataclysmSimulation(void(*callback)(int*, int));
void ProcessEndTurnSimulation(int turn, void(*cbHUDVisualUpdate)(int*, int), void(*cbHUDFortressUpdatePanel)(int*, int), void(*cbFortressMapUpdate)(int), void(*cbGameLost)());
bool CheckIfCanUseLastChance(int playerID);
bool CheckIfHasBuilderToManage(int playerID, int *IdSave);
void GetBackBuilderLink(int playerID, int value, int index, void(*callback)(int*, int));
int* GetTribesData(int index);
int* GetFortressData();
int* GetTribesExpData(int index, int expIndex);
int* GetTribesIntel(int index, int buildIndex, int intelNameID);
int* GetFortressIntel(int buildFortressIndex, int intelNameID);
bool CheckIfCanBuildUpgrade(int playerID, int index);

#if _DEBUG
int GiveFortressUpgrades(int idUpgrades);
void GiveResources(void(*cbConso)(int*, int));
#endif // _DEBUG


#endif // !_GAMESTATE_H