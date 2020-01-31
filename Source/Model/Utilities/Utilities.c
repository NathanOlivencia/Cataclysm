#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Utilities.h"


char* enumToString(int index)
{
	switch (index)
	{
	case 0:
		return("Fortress' Wall");
	case 1:
		return("Fortress' Shield");
	case 2:
		return("Fortress' Door");
	case 3:
		return("Field");
	case 4:
		return("Sawmill");
	case 5:
		return("Well");
	default:
		return("ERROR WRONG INDEX lign 23 file Utilities.c");
		break;
	}
}

//Basic clamp to avoid negative values
void ClampBetweenZAndCap(int *value, int max)
{
  if(*value <= 0)
  {
    *value = 0;
  }
  else if (*value > max && max != -1)
  {
	  *value = max;
  }
}

//Check if valueToCheck € [min, max]
bool CheckIntIsBetween(int valueToCheck, int min, int max)
{
	if (valueToCheck < min || valueToCheck > max)
	{
		printf("Mauvaise valeur rentree veuillez recommencer\n");
		return false;
	}
	else
	{
		return true;
	}
}

//Collect effects of various event
void AskEffects(int tmp[], int typeOfEvent)
{
	char* msg;
	switch (typeOfEvent)
	{
	case 0:
		msg = "Veuillez rentrer l'effet de l'evenement";
		break;
	case 1:
		msg = "Veuillez rentrer l'effet de l'expedition (*(nbre hab sent/5))";
		break;
	case 2:
		msg = "No need";
		break;
	case 3:
		msg = "Veuillez rentrer le montant de la donation";
		break;
	case 4:
		msg = "Veuillez renter le montant a envoyer dans la forteresse";
		break;
	default:
		printf("Error lign 61\n");
		return;
	}

	if (typeOfEvent != 3)
	{
		printf(msg); printf(" sur le nombre d'habitant \n");
		scanf("%d", &tmp[0]);
	}
	else
	{
		tmp[0] = 0;
	}

	printf(msg); printf(" sur la quantite de nourriture \n");
	scanf("%d", &tmp[1]);
	printf(msg); printf(" sur la quantite d'eau \n");
	scanf("%d", &tmp[2]);
	printf(msg); printf(" sur la quantite de materiaux \n");
	scanf("%d", &tmp[3]);
	printf("***************************************************\n");
}