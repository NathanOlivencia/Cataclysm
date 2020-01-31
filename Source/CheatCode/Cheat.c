#include "Cheat.h"
#include <conio.h>

char c;

enum CheatState
{
	noMode,
	fortressMode,
	resourcesMode
};

enum CheatState cheatState = 0;
void CheatMain()
{
#if _DEBUG
	
	if (!_kbhit())
	{
		return;
	}
	
	if (cheatState == 0)
	{
		c = _getch();
		
		printf("entry : %c\n", c);

		if (c == 'f')
		{
			cheatState = 1;
		}
		else if (c == 'r')
		{
			GiveResources(UpdateText);
		}
		else if (c == 't')
		{
			TurnIncr();
		}
	}
	else if (cheatState == 1)
	{
		c = _getch();

		printf("entry for fortress ID Upgrades: %c\n", c);

		if (c == '0')
		{
			int index = GiveFortressUpgrades(0);
			if (!index)
			{
				printf("Can't add this\n");
			}
			else
			{
				UpdateIntelsPanel(GetFortressIntel(index-1, 0), -1);
			}
		}
		else if (c == '1')
		{
			int index = GiveFortressUpgrades(1);
			if (!index)
			{
				printf("Can't add this\n");
			}
			else
			{
				UpdateIntelsPanel(GetFortressIntel(index-1, 1), -1);
			}
		}
		else if (c == '2')
		{
			int index = GiveFortressUpgrades(2);
			if (!index)
			{
				printf("Can't add this\n");
			}
			else
			{
				UpdateIntelsPanel(GetFortressIntel(index-1, 2), -1);
			}
		}
		else
		{
			printf("Wrong value\n");
		}
	}

	if (cheatState != 0 && c == 'c')
	{
		cheatState = 0;
		printf("CheatState Reset\n");
	}
	
#endif
}
