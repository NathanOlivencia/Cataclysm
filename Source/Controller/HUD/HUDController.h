#ifndef _HUDCONTROLLER_H
#define _HUDCONTROLLER_H

#include <SFML\Graphics.h>
#include "../GameStateManager/State.h"
#include "../Animation/Animation.h"
#include <Windows.h>

void InitMainMenu(sfVideoMode mode);
DWORD WINAPI UnLoadAnimFrameByFrame(void* data);
void ChangeLoadingProgress(float value);
DWORD WINAPI UnLoadAnim(void* data);
bool GetLoadingProgess();
void ReloadAnimLoading();
void InitGodSelectionMenu(sfVideoMode mode, bool isReload);
void InitMainSceneHUD(sfVideoMode mode);
void ResetMainSceneHud();
void InitTextHUD(int *data, int index);
int CheckHitBoxWithHUD(sfVector2i mousePos, GameState state);
bool SelectGodAndStarter(int godID);
void ChangeBuildPanelState(int id, int valueOverride);
void ChangeExPanelState(int id, int valueOverride);
void ChangeExpChoiceState(int id);
void BuildsPanelIsBuild(int index, int actualPlayerID);
void UpdateIntelsPanel(int* data, int actualPlayerID);
void ChangeFortressPanelState(int valueOverride);
void UpdateCardDisplayerPanel(int*data, int id);
void UpdateExpeditionsPanels(int* data, int actualPlayerID);
void ResetExpeditionsHUD(int playerID);
void SendInputToForm(int idForm, int value);
void UpdateForm(int inputIndex, int* data);
void UpdateText(int* data, int index);
void ChangeNewTurnHUD(GameState actualState, bool canUseLastChance);
void UpdateTurnDisplayHUD(int actualTurn);
void ProceedDiceThrow(int *Nullable, int toFinishOn);
// SET THE TEXT ACCORDING TO THE TYPE.
// 1 : Don't have enough resources ( for tribes build ).
// 2 : Already 3 expedition.
// 3 : New turn.
void SetInfoText(int type);
void UpdateInfoText(float deltaTime);
void UpdateHUD(sfRenderWindow* mainWindow, GameState *actualState, void(*cbData)(int*), void(*cbSetGodHUD)(int, int), float deltaTime, void(*cbUpdateGameState)());
void DisplayHUD(sfRenderWindow* mainWindow, Scene actualScene);

#endif // !_HUDCONTROLLER_H
