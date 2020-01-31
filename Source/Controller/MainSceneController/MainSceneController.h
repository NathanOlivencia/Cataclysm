#pragma once
#include <SFML\Graphics.h>
#include "../GameStateManager/State.h"
#include "../GraphicsUtilities/GraphicsUtilities.h"
#include <Windows.h>

void InitMainScene(bool isReload);
DWORD WINAPI UnLoadAnimCata(void *data);
void LoadMainSceneAnimSummer(bool isReload);
void LoadCataclysmAnim(int id);
void SetGodPlayer(int playerId, int godID);
void ChangeMap();
void ChangeActiveGod(int actualPlayerID);
void SwitchToCata(bool isWin);
bool ReturnEndGameAnimIsDone();
void UpdateMainScene(float deltatime);
void UpdateFortressParts(int id);
void UpdateTribesUpgradesState(int* upgradesOfTribe, int id);
void DisplayMainScene(sfRenderWindow* window, GameState state);
