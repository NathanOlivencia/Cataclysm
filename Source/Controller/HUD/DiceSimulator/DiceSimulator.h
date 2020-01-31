#ifndef _DiceSimulator_h
#define _DiceSimulator_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SFML\Graphics.h"

void initDice(sfVideoMode mode);
void LaunchDice();
bool IsFinalFace();
void setDiceValue(int value);
void UpdateDice(float deltaTime);
void DisplayDice(sfRenderWindow* window);

#endif // !_DiceSimulator_h