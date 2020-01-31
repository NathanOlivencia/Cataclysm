#include "BuildManager.h"
#include "../../../Model/Utilities/Utilities.h"

char* EnumTOString(int index)
{
	switch (index)
	{
		case 0:
			return("Nothing");
		case 1:
			return("Cost:");
		case 2:
			return("Gain:");
		default:
			return("NULL");
	}
}

void UpdateBuildButtonsUpgradeBuild(ButtonBuild* buttons)
{
	sfText_setString(buttons->buildText[1], "Built");
	buttons->isBuild = true;
}

void UpdateBuildButtons(ButtonBuild *buttons, int* data)
{
	if (!buttons->buildButton.isWorking)
	{
		buttons->buildButton.isWorking = true;
		buttons->isBuild = false;
	}
	
	sfText_setString(buttons->buildText[0], enumToString(data[0]));
	
	for (int i = 1; i < 3; i++)
	{
		UpdateTextWithStringDisplay(buttons->buildText[i], EnumTOString(i), data[i + 1]);
	}
}

void DisplayBuildPanel(BuildPanel panels[], sfRenderWindow* mainWindow)
{
	for (int id = 0; id < 2; id++)
	{
		if (panels[id].isActive)
		{
			sfRenderWindow_drawSprite(mainWindow, panels[id].panelBackground, NULL);
			for (int i = 0; i < 3; i++)
			{
				if (panels[id].buildsButt[i].buildButton.isWorking)
				{
					DisplayButtons(mainWindow, &panels[id].buildsButt[i].buildButton, 1);
					for (int j = 0; j < 3; j++)
					{
						sfRenderWindow_drawText(mainWindow, panels[id].buildsButt[i].buildText[j], NULL);
					}
				}
			}
		}
	}
}


void ResetBuildPanel(BuildPanel* PanelToReset)
{
	PanelToReset->isActive = false;
	for (int i = 0; i < 3; i++)
	{
		PanelToReset->buildsButt[i].buildButton.isWorking = false;
		PanelToReset->buildsButt[i].isBuild = false;
	}

}

BuildPanel BuildPanelProto(int index)
{
	BuildPanel newPanel;

	sfVector2f size = { 324.0f, 100.0f };
	sfVector2f posOrigin = { 30.0f + index * (1920.0f - 339.0f - 60.0f), 600.0f };
	char* filesDirectories = "./Resources/HUD/mainSceneHUD/BuildPanel/ExpeditionsStatsButton.png";
	for (int i = 0; i < 3; i++)
	{
		//Pos Button with whiteBackground
		sfVector2f tempPos = posOrigin;
		tempPos.x += 6.0f;
		tempPos.y += i * 115.f;
		printf("a%f\n", tempPos.y);
		newPanel.buildsButt[i].buildButton = InitButton(size, tempPos, i, filesDirectories, false);

		//Disposition text in the button
		for (int j = 0; j < 3; j++)
		{
			sfVector2f textTempPos = tempPos;
			textTempPos.x += 10.0f;
			textTempPos.y += j*21.0f;
			newPanel.buildsButt[i].buildText[j] = InitText(EnumTOString(j), textTempPos, sfBlack, 20);
		}
		newPanel.buildsButt[i].buildButton.isWorking = false;
		newPanel.buildsButt[i].isBuild = false;
	}

	newPanel.panelBackground = LoadSprite("./Resources/HUD/mainSceneHUD/BuildPanel/buildPanelBackground.png", 0);

	posOrigin.y -= 60.0f;
	sfSprite_setPosition(newPanel.panelBackground, posOrigin);
	newPanel.isActive = false;

	return newPanel;
}