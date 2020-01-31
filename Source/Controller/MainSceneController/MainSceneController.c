#include "MainSceneController.h"
#include <math.h>
#include "../Animation/Animation.h"

typedef struct TribeUpgades
{
	bool isBuild;
	sfSprite* upgrades[3];
}TribeUpgades;

typedef struct MainScene
{
	sfSprite* backGround[3][3];
	int actualMapIndex;
	bool isFadingMap;
	bool isPlayingAnim;
	int animID[3];
	float fadeTime;
	float currentFadeTime;
	sfSprite* maps[4];
	TribeUpgades tribeUpgrades[2][3];
	sfSprite* playersGods[2];
	sfSprite* fortressParts[3][2][2];
	Animation fortressReconstructionAnim[3][2];
	Animation flashCataclysm;
	Animation mapCataclysm[2];
	sfSprite* treeAboveFortress[2];
	sfSprite* mapDemolished[2];
	bool fortressPartReconstruct[3];
	bool delayFade;
	bool isCataclysm;
	bool isWin;
	bool animReady;
}MainScene;

MainScene gameScene;
float time = 0;

void InitMainScene(bool isReload)
{
	time = 0;
	gameScene.actualMapIndex = 0;
	gameScene.isFadingMap = false;
	gameScene.fadeTime = 10.0f;
	gameScene.currentFadeTime = 0.0f;
	gameScene.delayFade = false;
	gameScene.isPlayingAnim = false;
	gameScene.isCataclysm = false;
	gameScene.isWin = false;
	gameScene.animReady = false;
	for (int i = 0; i < 3; i++)
	{
		gameScene.animID[i] = -1;
	}

	if (isReload)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					sfSprite_setColor(gameScene.fortressParts[i][j][k], sfWhite);
				}
				sfSprite_setColor(gameScene.treeAboveFortress[j], sfWhite);
				sfSprite_setColor(gameScene.backGround[i][j], sfWhite);
				if(gameScene.playersGods[i] != NULL)
				{
					sfSprite_setColor(gameScene.playersGods[i], sfWhite);
				}

				sfSprite_setColor(gameScene.tribeUpgrades[0][i].upgrades[j], sfWhite);
				sfSprite_setColor(gameScene.tribeUpgrades[1][i].upgrades[j], sfWhite);
			}
			sfSprite_setColor(gameScene.maps[i], sfWhite);
			gameScene.fortressPartReconstruct[i] = false;

			gameScene.tribeUpgrades[0][i].isBuild = false;
			gameScene.tribeUpgrades[1][i].isBuild = false;

			
		}

		sfColor temp = sfSprite_getColor(gameScene.treeAboveFortress[1]);
		temp.a = 0;
		sfSprite_setColor(gameScene.treeAboveFortress[1], temp);
		sfColor color = sfSprite_getColor(gameScene.backGround[0][3]);
		color.a = 0;
		sfSprite_setColor(gameScene.backGround[0][3], color);
		return;
	}
	
	char* backGroundsDirectories[3][3] =
	{
		{ "Resources/Map/BackGround/Fond0.png", "Resources/Map/BackGround/Fond1.png", "Resources/Map/BackGround/Fond2.png" },
		{ "Resources/Map/BackGround/Summer_AR.png", "Resources/Map/BackGround/Winter_AR.png", "Resources/Map/BackGround/Doom_AR.png" },
		{ "Resources/Map/BackGround/Summer_PP.png", "Resources/Map/BackGround/Winter_PP.png", "Resources/Map/BackGround/Doom_PP.png" }
	};
		
	char* mapFilesDirectories[4] = { "Resources/Map/Map_Summer.png" , "Resources/Map/Map_Winter.png" , "Resources/Map/Fortress_Cataclysm/Map_Loose.png", "Resources/Map/Fortress_Cataclysm/Map_BeforeDestruction.png"  }; 
	
	char* mapFortressParts[3][2][2] = 
	{
		{{"Resources/Map/Fortress_Summer/Mur_lvl_1.png", "Resources/Map/Fortress_Winter/Mur_lvl_1.png"}, {"Resources/Map/Fortress_Summer/Mur_lvl_2.png", "Resources/Map/Fortress_Winter/Mur_lvl_2.png"} },
		{{"Resources/Map/Fortress_Summer/Bouclier_lvl-1.png", "Resources/Map/Fortress_Winter/Bouclier_lvl-1.png" }, {"Resources/Map/Fortress_Summer/Bouclier_lvl-2.png", "Resources/Map/Fortress_Winter/Bouclier_lvl-2.png" }},
		{{"Resources/Map/Fortress_Summer/Porte_lvl-1.png", "Resources/Map/Fortress_Winter/Porte_lvl_1.png"}, {"Resources/Map/Fortress_Summer/Porte_lvl-2.png", "Resources/Map/Fortress_Winter/Porte_lvl_2.png"}}
	};

	char* tribeUpgradesFilesDirectories[6][3] = 
	{ 
		{"Resources/Map/Builds/Village_Left/Champs.png", "Resources/Map/Builds/Village_Left/Champs_Neige.png" , "Resources/Map/Builds/Village_Left/Champs.png" },
		{"Resources/Map/Builds/Village_Left/Scierie.png" , "Resources/Map/Builds/Village_Left/Scierie_Neige.png" , "Resources/Map/Builds/Village_Left/Scierie.png" },
		{ "Resources/Map/Builds/Village_Left/Puit.png", "Resources/Map/Builds/Village_Left/Puit_Neige.png" , "Resources/Map/Builds/Village_Left/Puit.png" },
		{"Resources/Map/Builds/Village_Right/Champs.png", "Resources/Map/Builds/Village_Right/Champs_Neige.png" , "Resources/Map/Builds/Village_Right/Champs.png" },
		{"Resources/Map/Builds/Village_Right/Scierie.png", "Resources/Map/Builds/Village_Right/Scierie_Neige.png" , "Resources/Map/Builds/Village_Right/Scierie.png" },
		{"Resources/Map/Builds/Village_Right/Puit.png", "Resources/Map/Builds/Village_Right/Puit_Neige.png" , "Resources/Map/Builds/Village_Right/Puit.png" }
	};

	char* mapDemolishedDirectories[2] =
	{
		"Resources/Map/Fortress_Cataclysm/Map_GameOver.png",
		"Resources/Map/Fortress_Cataclysm/Map_Yousurvived.png"
	};

	for (int m = 0; m < 4; m++)
	{
		gameScene.maps[m] = LoadSprite(mapFilesDirectories[m], 0);
	}
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			gameScene.tribeUpgrades[0][i].isBuild = false;
			gameScene.tribeUpgrades[1][i].isBuild = false;

			gameScene.tribeUpgrades[0][i].upgrades[j] = LoadSprite(tribeUpgradesFilesDirectories[i][j], 0);
			gameScene.tribeUpgrades[1][i].upgrades[j] = LoadSprite(tribeUpgradesFilesDirectories[i+3][j], 0);

			gameScene.backGround[i][j] = LoadSprite(backGroundsDirectories[i][j], 0);
			if (i == 2)
			{
				sfVector2f frontCloudPos = { 0.0f, +50.0f };
				sfSprite_setPosition(gameScene.backGround[i][j], frontCloudPos);
			}
		}

		gameScene.fortressPartReconstruct[i] = false;
	}

	sfColor color = sfSprite_getColor(gameScene.backGround[0][3]);
	color.a = 0;
	sfSprite_setColor(gameScene.backGround[0][3], color);

	gameScene.treeAboveFortress[0] = LoadSprite("Resources/Map/Fortress_Summer/Arbre_Ete.png", 0);
	gameScene.treeAboveFortress[1] = LoadSprite("Resources/Map/Fortress_Winter/Arbre_Hiver.png", 0);
	sfColor temp = sfSprite_getColor(gameScene.treeAboveFortress[1]);
	temp.a = 0;
	sfSprite_setColor(gameScene.treeAboveFortress[1], temp);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				gameScene.fortressParts[i][j][k] = LoadSprite(mapFortressParts[i][j][k], 0);
			}	
			gameScene.mapDemolished[j] = LoadSprite(mapDemolishedDirectories[j], 0);
		}
	}


}

void UnLoadWinterAnim()
{
	if (gameScene.fortressReconstructionAnim[0][1].spriteArray != NULL && gameScene.fortressReconstructionAnim[0][1].isLoaded)
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[0][1]);
	}
	if (gameScene.fortressReconstructionAnim[1][1].spriteArray != NULL && gameScene.fortressReconstructionAnim[1][1].isLoaded)
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[1][1]);
	}
	if (gameScene.fortressReconstructionAnim[2][1].spriteArray != NULL && gameScene.fortressReconstructionAnim[2][1].isLoaded)
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[2][1]);
	}
}

DWORD WINAPI UnloadWinterAnimThrdFunc(void *data)
{
	UnLoadWinterAnim();
	return 0;
}

void SwitchToCata(bool isWin)
{
	gameScene.isWin = isWin;
	gameScene.actualMapIndex++;
	gameScene.isCataclysm = true;
	gameScene.isFadingMap = true;
	gameScene.isPlayingAnim = true;
	HANDLE threadUnloadWinter = CreateThread(NULL, 0, UnloadWinterAnimThrdFunc, NULL, 0, NULL);
	LoadCataclysmAnim(isWin);
}

DWORD WINAPI UnLoadAnimCata(void *data)
{
	for (int i = 0; i < 2; i++)
	{
		if (gameScene.mapCataclysm[i].spriteArray != NULL && gameScene.mapCataclysm[i].isLoaded)
		{
			UnLoadAnimation(&gameScene.mapCataclysm[i]);
		}
	}

	if (gameScene.flashCataclysm.spriteArray != NULL && gameScene.flashCataclysm.isLoaded)
	{
		UnLoadAnimation(&gameScene.flashCataclysm);
	}
	return 0;
}


void LoadMainSceneAnimSummer(bool isReload)
{
	sfVector2f posNull = { 0.0f, 0.0f };
	if (!isReload)
	{
		// 0 is wall; 1 is Shield; 2 Door
		gameScene.fortressReconstructionAnim[0][0] = LoadAnimation(61, 2.0f, "Resources/Map/Fortress_Summer/Animation_Forteresse/Mur_reconstruction_01/Mur_Reconstruction0000_", "png", posNull, 0);
		gameScene.fortressReconstructionAnim[1][0] = LoadAnimation(48, 2.0f, "Resources/Map/Fortress_Summer/Animation_Forteresse/Anim_Bouclier_01/Bouclier_reconstruction_", "png", posNull, 0);
		gameScene.fortressReconstructionAnim[2][0] = LoadAnimation(30, 2.0f, "Resources/Map/Fortress_Summer/Animation_Forteresse/Porte_reconstruction/POrte_recon 2_", "png", posNull, 0);
	}
	else
	{
		if (!gameScene.fortressReconstructionAnim[0][0].isLoaded)
		{
			ReloadAnimation(&gameScene.fortressReconstructionAnim[0][0], "Resources/Map/Fortress_Summer/Animation_Forteresse/Mur_reconstruction_01/Mur_Reconstruction0000_", "png", posNull);
		}
		if (!gameScene.fortressReconstructionAnim[1][0].isLoaded)
		{
			ReloadAnimation(&gameScene.fortressReconstructionAnim[1][0], "Resources/Map/Fortress_Summer/Animation_Forteresse/Anim_Bouclier_01/Bouclier_reconstruction_", "png", posNull);
		}
		if (!gameScene.fortressReconstructionAnim[2][0].isLoaded)
		{
			ReloadAnimation(&gameScene.fortressReconstructionAnim[2][0], "Resources/Map/Fortress_Summer/Animation_Forteresse/Porte_reconstruction/POrte_recon 2_", "png", posNull);
		}

		//Check to unload Winter if they are still stocked
		UnLoadWinterAnim();
	}
}

void UnLoadMainSceneAnimSummer()
{
	if (!gameScene.fortressPartReconstruct[0])
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[0][0]);
	}
	if (!gameScene.fortressPartReconstruct[1])
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[1][0]);
	}
	if (!gameScene.fortressPartReconstruct[2])
	{
		UnLoadAnimation(&gameScene.fortressReconstructionAnim[2][0]);
	}
}

DWORD WINAPI LoadCataFlashThdFunc(void* data)
{
	gameScene.flashCataclysm = LoadAnimation(40, 1.6f, "Resources/Map/Fortress_Cataclysm/Transition_Cata/transition_Cata", "png", (sfVector2f) { 0.0f, 0.0f }, false);
	printf("FINISHED\n");
	return 0;
}


DWORD WINAPI LoadCataclysmAnimThreadFunc(void* data)
{
	int temp = data == 0 ? 0 : 1;
	if (temp == 1)
	{
		gameScene.mapCataclysm[1] = LoadAnimationSpecifiedIndexStart(101, 4.0f, "Resources/Map/Fortress_Cataclysm/Anim_Victory/Map_Yousurvived", "png", (sfVector2f) { 0.0f, 0.0f }, false, 40);
		printf("FINISHED\n");
		gameScene.animReady = true;
	}
	else if (temp == 0)
	{
		gameScene.mapCataclysm[0] = LoadAnimation(250, 10.0f, "Resources/Map/Fortress_Cataclysm/Anim_Loose/Anim_Loose", "png", (sfVector2f) { 0.0f, 0.0f }, false);
		printf("FINISHED\n");
		gameScene.animReady = true;
	}
	
	return 0;
}



void LoadCataclysmAnim(int id)
{
	if (id == 0)
	{
		HANDLE threadLoadCataclysm2 = CreateThread(NULL, 0, LoadCataclysmAnimThreadFunc, (void*)0, 0, NULL);
	}
	else if (id == 1)
	{
		HANDLE threadLoadCataclysm = CreateThread(NULL, 0, LoadCataclysmAnimThreadFunc, (void*)1, 0, NULL);
	}
	else
	{
		HANDLE threadLoadCataclysm3 = CreateThread(NULL, 0, LoadCataFlashThdFunc, NULL, 0, NULL);
	}
	
	
}



DWORD WINAPI LoadMainSceneAnimWinter(void* data)
{
	sfVector2f posNull = { 0.0f, 0.0f };
	if (!gameScene.fortressPartReconstruct[0])
	{
		gameScene.fortressReconstructionAnim[0][1] = LoadAnimation(61, 2.0f, "Resources/Map/Fortress_Winter/Animation_Forteresse/Mur_reconstruction_Hiver/Bouclier_reconstruction_", "png", posNull, 0);
	}
	if (!gameScene.fortressPartReconstruct[1])
	{
		gameScene.fortressReconstructionAnim[1][1] = LoadAnimation(101, 3.0f, "Resources/Map/Fortress_Winter/Animation_Forteresse/Bouclier_reconstruction_Hiver/Bouclier_reconstruction_", "png", posNull, 0);
	}
	if (!gameScene.fortressPartReconstruct[2])
	{
		gameScene.fortressReconstructionAnim[2][1] = LoadAnimation(30, 2.0f, "Resources/Map/Fortress_Winter/Animation_Forteresse/Porte_reconstruction_Hiver/Bouclier_reconstruction_", "png", posNull, 0);
	}
	
	UnLoadMainSceneAnimSummer();
	return 0;
}

DWORD WINAPI UnLoadAnimForto(void *data)
{
	int tmp = data == 0 ? 0 : 1;
	for (int i = 0; i < 3; i++)
	{
		if (gameScene.fortressPartReconstruct[i] && gameScene.fortressReconstructionAnim[i][tmp].isLoaded)
		{
			UnLoadAnimation(&gameScene.fortressReconstructionAnim[i][tmp]);
		}
	}
	return 0;
}

void SetGodPlayer(int playerId, int godID)
{
	char* godFilesDirectories [6] = 
	{
		"Resources/Gods/AmaterasuV2.png", "Resources/Gods/AphroditeV2.png", "Resources/Gods/kulkulkanV2.png",
		"Resources/Gods/odinV2.png", "Resources/Gods/OsirisV2.png", "Resources/Gods/ShivaV2.png",
	};
	
	sfVector2f godsPos[2] = { {300, 350}, {1600, 350} };
	sfVector2f godScale = { 0.5f, 0.5f };

	sfColor inactiveColor = { 80, 80, 100, 220 };

	gameScene.playersGods[playerId] = LoadSprite(godFilesDirectories[godID], 1);

	if (playerId == 1)
	{
		sfSprite_setColor(gameScene.playersGods[playerId], inactiveColor);
		godScale.x *= -1;
	}

	sfSprite_setScale(gameScene.playersGods[playerId], godScale);
	
	sfSprite_setPosition(gameScene.playersGods[playerId], godsPos[playerId]);
}

void ChangeMap()
{
	if (!gameScene.isPlayingAnim)
	{
		HANDLE loadAnimThread = CreateThread(NULL, 0, LoadMainSceneAnimWinter, NULL, 0, NULL);
		gameScene.actualMapIndex++;
		gameScene.isFadingMap = true;
	}
	else
	{
		gameScene.delayFade = true;
	}
	
}

void ChangeActiveGod(int actualPlayerID)
{
	sfColor activeColor = { 255, 255, 255, 255 };
	sfColor inactiveColor = { 80, 80, 100, 220 };
	if (actualPlayerID == 0)
	{
		sfSprite_setColor(gameScene.playersGods[0], activeColor);
		sfSprite_setColor(gameScene.playersGods[1], inactiveColor);
	}
	else
	{
		sfSprite_setColor(gameScene.playersGods[0], inactiveColor);
		sfSprite_setColor(gameScene.playersGods[1], activeColor);
	}
}

bool ReturnEndGameAnimIsDone()
{
	bool temp = (gameScene.mapCataclysm[gameScene.isWin].isLoaded && gameScene.mapCataclysm[gameScene.isWin].actualFrameToShow == gameScene.mapCataclysm[gameScene.isWin].numberOfFrame - 1);
	if (temp)
	{
		HANDLE UnLoad = CreateThread(NULL, 0, UnLoadAnimCata, NULL, 0, NULL);
	}
	return(temp);
}

void UpdateMainScene(float deltatime)
{
	sfVector2f frontCloudPos;
	time += deltatime;
	
	if (gameScene.isCataclysm)
	{
		if (gameScene.isFadingMap)
		{
			if(gameScene.currentFadeTime <= gameScene.fadeTime)
			{
				sfColor color = sfSprite_getColor(gameScene.maps[gameScene.actualMapIndex - 1]);
				sfColor color2 = sfSprite_getColor(gameScene.maps[gameScene.actualMapIndex - 1]);
				float alphaRatio = (float)((gameScene.fadeTime - gameScene.currentFadeTime) / gameScene.fadeTime);
				color.a = (int)(alphaRatio * 255);
				alphaRatio = gameScene.currentFadeTime / gameScene.fadeTime;
				color2.a = (int)(alphaRatio * 255);

				for (int i = 0; i < 3; i++)
				{
					sfSprite_setColor(gameScene.backGround[i][gameScene.actualMapIndex - 1], color);
				}
				for (int i = 0; i < 2; i++)
				{
					sfColor godColor = sfSprite_getColor(gameScene.playersGods[i]);
					godColor.a = color.a;
					sfSprite_setColor(gameScene.playersGods[i], godColor);
				}

				sfSprite_setColor(gameScene.backGround[0][3], color2);
				gameScene.currentFadeTime += deltatime;
			}
			else
			{
				gameScene.isFadingMap = false;
				gameScene.currentFadeTime = 0.0f;
			}
			return;
		}
		else if(!gameScene.animReady)
		{
			return;
		}
		else if (gameScene.isPlayingAnim)
		{
			if (gameScene.flashCataclysm.actualDuration < gameScene.flashCataclysm.totalDuration)
			{
				if (gameScene.flashCataclysm.actualFrameToShow >= 28)
				{
					UpdateAnimation(&gameScene.mapCataclysm[gameScene.isWin], deltatime);
				}
				UpdateAnimation(&gameScene.flashCataclysm, deltatime);
			}
			else
			{
				UpdateAnimation(&gameScene.mapCataclysm[gameScene.isWin], deltatime);
				if (gameScene.mapCataclysm[gameScene.isWin].actualDuration >= gameScene.mapCataclysm[gameScene.isWin].totalDuration)
				{
					gameScene.isPlayingAnim = false;
				}
			}
		}
		return;
	}

	if (gameScene.isPlayingAnim)
	{
		int cpt = 0;
		for (int i = 0; i < 3; i++)
		{
			if (gameScene.animID[i] > -1)
			{
				UpdateAnimation(&gameScene.fortressReconstructionAnim[i][gameScene.actualMapIndex], deltatime);
				if (gameScene.fortressReconstructionAnim[i][gameScene.actualMapIndex].actualDuration
					>= gameScene.fortressReconstructionAnim[i][gameScene.actualMapIndex].totalDuration
					)
				{
					gameScene.animID[i] = -1;
				}
			}
			else
			{
				cpt++;
			}
		}

		if (cpt == 3)
		{
			//Use adress as a container of a int
			long long int temp2 = gameScene.actualMapIndex;
			void* temp = (void*)temp2;
			HANDLE UnLoadAnim = CreateThread(NULL, 0, UnLoadAnimForto, temp, 0, NULL);
			gameScene.isPlayingAnim = false;
			if (gameScene.delayFade)
			{
				HANDLE loadAnimThread = CreateThread(NULL, 0, LoadMainSceneAnimWinter, NULL, 0, NULL);
				gameScene.actualMapIndex++;
				gameScene.isFadingMap = true;
				gameScene.delayFade = false;
			}
		}
		return;
	}

	if (gameScene.isFadingMap)
	{
		if (gameScene.currentFadeTime <= gameScene.fadeTime)
		{
			sfColor color = sfSprite_getColor(gameScene.maps[gameScene.actualMapIndex - 1]);
			sfColor color2 = sfSprite_getColor(gameScene.maps[gameScene.actualMapIndex]);
			float alphaRatio = (float)((gameScene.fadeTime - gameScene.currentFadeTime) / gameScene.fadeTime);
			color.a = (int)(alphaRatio * 255);
			alphaRatio = gameScene.currentFadeTime / gameScene.fadeTime;
			color2.a = (int)(alphaRatio * 255);
			
			sfSprite_setColor(gameScene.maps[gameScene.actualMapIndex - 1], color);

			for (int i = 0; i < 3; i++)
			{
				if (gameScene.tribeUpgrades[0][i].isBuild)
				{
					sfSprite_setColor(gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex], color2);
					sfSprite_setColor(gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex - 1], color);
				}
				if (gameScene.tribeUpgrades[1][i].isBuild)
				{
					sfSprite_setColor(gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex], color2);
					sfSprite_setColor(gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex - 1], color);
				}

				sfSprite_setColor(gameScene.backGround[i][gameScene.actualMapIndex - 1], color);
			}

			sfSprite_setColor(gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex - 1], color);
			sfSprite_setColor(gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex], color2);
			sfSprite_setColor(gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex], color2);

			sfSprite_setColor(gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex - 1], color);
			sfSprite_setColor(gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex - 1], color);

			sfSprite_setColor(gameScene.treeAboveFortress[gameScene.actualMapIndex], color2);
			sfSprite_setColor(gameScene.treeAboveFortress[gameScene.actualMapIndex - 1], color);

			gameScene.currentFadeTime += deltatime;
		}
		else
		{
			gameScene.isFadingMap = false;
			gameScene.currentFadeTime = 0.0f;
		}
	}


	frontCloudPos = sfSprite_getPosition(gameScene.backGround[2][gameScene.actualMapIndex]);
	frontCloudPos.y = sinf(time) * 20.f + 50.0f;
	sfSprite_setPosition(gameScene.backGround[2][gameScene.actualMapIndex], frontCloudPos);
}

void UpdateFortressParts(int id)
{
	gameScene.fortressPartReconstruct[id] = true;
	gameScene.isPlayingAnim = true;
	gameScene.animID[id] = 1;
}

void UpdateTribesUpgradesState(int* upgradesOfTribe, int id)
{
	for (int i = 0; i < 3; i++)
	{
		if (!gameScene.tribeUpgrades[id][i].isBuild && upgradesOfTribe[i] != 0)
		{
			gameScene.tribeUpgrades[id][i].isBuild = true;
		}
	}
}

// Display all the main scene elements according to the actual game state
void DisplayMainScene(sfRenderWindow* window, GameState state)
{
	if (state.actualscene == 1 || state.actualscene == 2 || state.actualscene == 4)
	{
		if (gameScene.isCataclysm)
		{
			if (gameScene.isFadingMap)
			{
				sfRenderWindow_drawSprite(window, gameScene.backGround[0][gameScene.actualMapIndex], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[0][gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[1][gameScene.actualMapIndex], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[1][gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.playersGods[0], NULL);
				sfRenderWindow_drawSprite(window, gameScene.playersGods[1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.maps[gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.treeAboveFortress[gameScene.actualMapIndex - 1], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex - 1], NULL);
			
				// Display tribes's upgrades if they are build
				for (int i = 0; i < 3; i++)
				{
					if (gameScene.tribeUpgrades[0][i].isBuild)
					{
						sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex - 1], NULL);
					}
					if (gameScene.tribeUpgrades[1][i].isBuild)
					{
						sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex - 1], NULL);
					}
				}

				return;
			}
			
			sfRenderWindow_drawSprite(window, gameScene.backGround[0][gameScene.actualMapIndex], NULL);
			sfRenderWindow_drawSprite(window, gameScene.backGround[1][gameScene.actualMapIndex], NULL);
			
			if (gameScene.isPlayingAnim)
			{
				if (gameScene.animReady)
				{
					if (gameScene.flashCataclysm.actualDuration < gameScene.flashCataclysm.totalDuration)
					{
						if (gameScene.flashCataclysm.actualFrameToShow >= 28)
						{
							sfRenderWindow_drawSprite(window, gameScene.mapCataclysm[gameScene.isWin].spriteArray[gameScene.mapCataclysm[gameScene.isWin].actualFrameToShow], NULL);
						}
						else
						{
							sfRenderWindow_drawSprite(window, gameScene.maps[gameScene.actualMapIndex - 1], NULL);
							sfRenderWindow_drawSprite(window, gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex - 1], NULL);
							sfRenderWindow_drawSprite(window, gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex - 1], NULL);
							sfRenderWindow_drawSprite(window, gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex - 1], NULL);
							sfRenderWindow_drawSprite(window, gameScene.treeAboveFortress[gameScene.actualMapIndex - 1], NULL);
							for (int i = 0; i < 3; i++)
							{
								if (gameScene.tribeUpgrades[0][i].isBuild)
								{
									sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex - 1], NULL);
								}
								if (gameScene.tribeUpgrades[1][i].isBuild)
								{
									sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex - 1], NULL);
								}
							}
						}
						sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);
						sfRenderWindow_drawSprite(window, gameScene.flashCataclysm.spriteArray[gameScene.flashCataclysm.actualFrameToShow], NULL);
					}
					else
					{
						sfRenderWindow_drawSprite(window, gameScene.mapCataclysm[gameScene.isWin].spriteArray[gameScene.mapCataclysm[gameScene.isWin].actualFrameToShow], NULL);
						sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);
					}
				}
				else
				{
					sfRenderWindow_drawSprite(window, gameScene.maps[gameScene.actualMapIndex - 1], NULL);
					sfRenderWindow_drawSprite(window, gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex - 1], NULL);
					sfRenderWindow_drawSprite(window, gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex - 1], NULL);
					sfRenderWindow_drawSprite(window, gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex - 1], NULL);
					sfRenderWindow_drawSprite(window, gameScene.treeAboveFortress[gameScene.actualMapIndex - 1], NULL);
					for (int i = 0; i < 3; i++)
					{
						if (gameScene.tribeUpgrades[0][i].isBuild)
						{
							sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex - 1], NULL);
						}
						if (gameScene.tribeUpgrades[1][i].isBuild)
						{
							sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex - 1], NULL);
						}
					}
					sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);
				}
			
			}
			else
			{
				sfRenderWindow_drawSprite(window, gameScene.mapDemolished[gameScene.isWin], NULL);
				sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);
			}	
			return;
		}

		sfRenderWindow_drawSprite(window, gameScene.backGround[0][gameScene.actualMapIndex], NULL);
		sfRenderWindow_drawSprite(window, gameScene.backGround[1][gameScene.actualMapIndex], NULL);

		if (gameScene.isFadingMap)
		{
			sfRenderWindow_drawSprite(window, gameScene.backGround[0][gameScene.actualMapIndex - 1], NULL);
			sfRenderWindow_drawSprite(window, gameScene.backGround[1][gameScene.actualMapIndex - 1], NULL);
		}

		// Display the right god sprite for each player if they are set
		if (gameScene.playersGods[0] != NULL && gameScene.playersGods[1] != NULL)
		{
			sfRenderWindow_drawSprite(window, gameScene.playersGods[0], NULL);
			sfRenderWindow_drawSprite(window, gameScene.playersGods[1], NULL);
		}
		
		// Display the right map according to the state of the game ( season )
		sfRenderWindow_drawSprite(window, gameScene.maps[gameScene.actualMapIndex], NULL);
		sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex], NULL);

		if (gameScene.isFadingMap)
		{
			sfRenderWindow_drawSprite(window, gameScene.maps[gameScene.actualMapIndex - 1], NULL);
			sfRenderWindow_drawSprite(window, gameScene.backGround[2][gameScene.actualMapIndex - 1], NULL);
		}

		if (gameScene.isPlayingAnim)
		{
			for (int i = 4; i >= 2 ; i--)
			{
				int temp = i % 3;
				if (gameScene.animID[temp] > -1)
				{
					sfRenderWindow_drawSprite(window, gameScene.fortressReconstructionAnim[temp][gameScene.actualMapIndex].spriteArray[gameScene.fortressReconstructionAnim[temp][gameScene.actualMapIndex].actualFrameToShow], NULL);
				}
				else
				{
					sfRenderWindow_drawSprite(window, gameScene.fortressParts[temp][gameScene.fortressPartReconstruct[temp]][gameScene.actualMapIndex], NULL);
				}
			}
			
		}
		else
		{
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex], NULL);
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex], NULL);
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex], NULL);
		}

		if (gameScene.isFadingMap)
		{
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[1][gameScene.fortressPartReconstruct[1]][gameScene.actualMapIndex - 1], NULL);
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[0][gameScene.fortressPartReconstruct[0]][gameScene.actualMapIndex - 1], NULL);
			sfRenderWindow_drawSprite(window, gameScene.fortressParts[2][gameScene.fortressPartReconstruct[2]][gameScene.actualMapIndex - 1], NULL);
		}
				
		sfRenderWindow_drawSprite(window, gameScene.treeAboveFortress[gameScene.actualMapIndex], NULL);

		if (gameScene.isFadingMap)
		{
			sfRenderWindow_drawSprite(window, gameScene.treeAboveFortress[gameScene.actualMapIndex - 1], NULL);
		}

		// Display tribes's upgrades if they are build
		for (int i = 0; i < 3; i++)
		{
			if (gameScene.tribeUpgrades[0][i].isBuild)
			{		
				sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex], NULL);	
				if (gameScene.isFadingMap)
				{
					sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[0][i].upgrades[gameScene.actualMapIndex - 1], NULL);
				}
			}
			if (gameScene.tribeUpgrades[1][i].isBuild)
			{		
				sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex], NULL);
				if (gameScene.isFadingMap)
				{
					sfRenderWindow_drawSprite(window, gameScene.tribeUpgrades[1][i].upgrades[gameScene.actualMapIndex - 1], NULL);
				}
			}
		}
	}
}