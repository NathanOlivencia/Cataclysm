#ifndef _UTILITIES_H_
#define _UTILITIES_H_


char* enumToString(int index);
bool CheckIntIsBetween(int valueToCheck, int min, int max);
void ClampBetweenZAndCap(int *value, int max);
void AskEffects(int tmp[], int typeOfEvent);

#endif