#ifndef _GAMESTATEMANAGER_H
#define _GAMESTATEMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "State.h"

	void InitGameState();
#if _DEBUG
	void TurnIncr();
#endif
	void GameIsLoose();
	int GetActualScene();
	GameState GetActualState();
	void GetDataForm(int* dataForm);
	void UpdateState(sfRenderWindow* mainWindow, float deltaTime);
	void ManageStateInputKeyboard();
	void ManageState(sfVector2i mousePos);

#endif // !_GAMESTATEMANAGER_H
