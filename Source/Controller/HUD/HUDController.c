#include "HUDController.h"
#include "Text\Text.h"
#include "Menu\Menu.h"
#include "Form\Form.h"
#include "ExpeditionManager\ExpeditionManager.h"
#include "BuildManager\BuildManager.h"
#include "ResourcesDisplayer\ResourcesDiplayer.h"
#include "FortressManager\FortressManager.h"
#include "CardsDisplayers\CardsDisplayers.h"
#include "GodSelectionMenu\SelectionMenu.h"
#include "../GraphicsUtilities\GraphicsUtilities.h"
#include "DiceSimulator\DiceSimulator.h"
#include "../Audio\Sounds.h"

typedef struct InfoText
{
	sfText* text;
	float displayTime;
	float currentDisplayTime;
	bool isDisplay;
}InfoText;

Menu mainMenuHUD;

//mainScene All HUDS
Menu mainSceneHUD;

Form form[3];
int formDisplayed;
ResourcesBar resourcesHUD[3];
ExpeditionPanel expeditionsPanels[2];
BuildPanel buildsPanels[2];
Menu expChoice;
Menu buildChoice;
Menu builderChoice;
FortressPanel fortressPanel;
Menu pauseMenu;
sfSprite* formIllus[3];
sfSprite* turnBackground;
sfText* turnText;
InfoText infoText;
Menu gameOverMenu;
Menu gameWonMenu;
bool endGameFade;
float endGameFadeTime;
bool isWin;
bool fadeBetweenMenu;
float fadeBetweenMenuTime;
float fadeBetweenMenuTimeMax;
sfSprite* fadeSprite;
bool isUsingLastChance;

//LoadSceneHUD
Animation loadScene;
Animation msgLoad;
sfSprite* logoCrea;
float progress;
int StatusConvertedInFrame = 0;

//TEST 
sfMusic *mainMenuBackGroundMusic;
sfMusic *mainSceneBackGroundMusic;
sfMusic *summerBGMusic;
sfMusic *winterBGMusic;
sfMusic *endMusicEpic;

bool isFadeOutMusic;

// --------------------------------------------------------------------------------------------------------
void InitMainMenu(sfVideoMode mode)
{
	mainMenuBackGroundMusic = LoadMusic("Resources/Sound/Cataclysm_Menu_Sound.flac");
	SetMusicData(mainMenuBackGroundMusic, 50.0f, 1.0f, true);
	mainSceneBackGroundMusic = LoadMusic("Resources/Sound/TZW_-_Wake_up_Camillo_Calm.flac");
	SetMusicData(mainSceneBackGroundMusic, 65.0f, 1.0f, true);
	summerBGMusic = LoadMusic("Resources/Sound/Outdoor Summer Ambience [SOUND EFFECT].flac");
	SetMusicData(summerBGMusic, 100.0f, 1.0f, true);
	winterBGMusic = LoadMusic("Resources/Sound/Howling Wind.flac");
	SetMusicData(winterBGMusic, 100.0f, 1.0f, true);
	endMusicEpic = LoadMusic("Resources/Sound/TZW_-_Wake_up_Camill_Cata.flac");
	SetMusicData(endMusicEpic, 85.0f, 1.0f, true);


	MusicPlay(mainMenuBackGroundMusic);
	mainMenuHUD = CreateMainMenuProto(mode);
	sfVector2f origin = { 0.0f , 0.0f };
	progress = 0.0f;
	sfVector2f pos = { 1500 - 938.5f, 960 - 720.0f };
	loadScene = LoadAnimation(101, 4, "Resources/HUD/LoadingScene/Chargement_Forto/Forto_Chargement_Shadow_", "png", origin, false);
	msgLoad = LoadAnimation(22, 0.88f, "Resources/HUD/LoadingScene/Chargement/Chargement_", "png", pos, true);
	logoCrea = LoadSprite("Resources/HUD/LoadingScene/creajeux_logo_cj.png", 0);
	sfVector2f temp = { 0, 830 };
	sfSprite_setPosition(logoCrea, temp);
}

DWORD WINAPI UnLoadAnimFrameByFrame(void* data)
{
	bool done = false;
	int indexRefer = 0;
	while (done)
	{
		for (int i = indexRefer; i < loadScene.actualFrameToShow - 1; i++)
		{
			UnLoadFrameOfAnim(&loadScene, i);
		}
		indexRefer = loadScene.actualFrameToShow - 1;
		Sleep(1);
		done = (indexRefer == loadScene.numberOfFrame - 2);
	}
	loadScene.isLoaded = false;
	return 0;
}

void ChangeLoadingProgress(float value)
{
	progress += value;
	StatusConvertedInFrame = (int)(progress * loadScene.numberOfFrame);
	if (value == -1.0f)
	{
		loadScene.actualDuration = 0.0f;
		msgLoad.actualDuration = 0.0f;
		loadScene.actualFrameToShow = 0;
		msgLoad.actualFrameToShow = 0;
	}
}

DWORD WINAPI UnLoadAnim(void* data)
{
	UnLoadAnimation(&loadScene);
	UnLoadAnimation(&msgLoad);
	return 0;
}

DWORD WINAPI ReLoadAnim(void* data)
{
	sfVector2f origin = { 0.0f , 0.0f };
	sfVector2f pos = { 1500 - 938.5f, 960 - 720.0f };
	if (data == 0)
	{
		ReloadAnimationOrigin(&loadScene, "Resources/HUD/LoadingScene/Chargement_Forto/Forto_Chargement_Shadow_", "png");
		printf("FINISHED\n");
	}
	else
	{
		ReloadAnimation(&msgLoad, "Resources/HUD/LoadingScene/Chargement/Chargement_", "png", pos);
	}
	return 0;
}

bool GetLoadingProgess()
{
	return(progress == 1.0f && loadScene.numberOfFrame - 1 == loadScene.actualFrameToShow);
}

void ReloadAnimLoading()
{
	HANDLE myThread5 = CreateThread(NULL, 0, ReLoadAnim, (void*)0, 0, NULL);
	HANDLE myThread6 = CreateThread(NULL, 0, ReLoadAnim, (void*)1, 0, NULL);
}

// --------------------------------------------------------------------------------------------------------
void InitGodSelectionMenu(sfVideoMode mode, bool isReload)
{
	InitSelectionScene(mode, isReload);
}

// --------------------------------------------------------------------------------------------------------
void InitMainSceneHUD(sfVideoMode mode)
{
	isFadeOutMusic = false;
	endGameFade = false;
	endGameFadeTime = 0.0f;
	isWin = false;
	fadeBetweenMenu = false;
	isUsingLastChance = false;
	fadeBetweenMenuTimeMax = 6.0f;
	mainSceneHUD = CreateMainSceneHUD(mode);
	sfVector2f turnPos = { mode.width / 2 , (float)(mode.height - 100) };
	sfVector2f infoTextPos = { 100.0f, (float)(mode.height / 2 - 50) };

	sfVector2f formPos;

	turnBackground = LoadSprite("./Resources/HUD/mainSceneHUD/globalSceneHUD/turnBackground.png", 1);
	sfSprite_setPosition(turnBackground, turnPos);
	turnText = InitText("1", turnPos, sfWhite, 48);
	sfFloatRect tempRect = sfText_getGlobalBounds(turnText);
	turnPos.x -=  sfText_getGlobalBounds(turnText).width / 2;
	turnPos.y -= (sfText_getCharacterSize(turnText) / 2 + tempRect.height) / 2;
	sfText_setPosition(turnText, turnPos);
	
	
	infoText.displayTime = 3.0f;
	infoText.currentDisplayTime = 0.0f;
	infoText.isDisplay = false;
	infoText.text = InitText("", infoTextPos, sfWhite, 60);
	sfText_setOutlineColor(infoText.text, sfColor_fromRGB(11, 88, 142));
	sfText_setOutlineThickness(infoText.text, 10.0f);

	formPos.x = (mode.width / 2) - (555.0f / 2);
	formPos.y = (mode.height / 2) - (655.0f / 2);
	form[0] = InitForm("0", formPos, "./Resources/HUD/mainSceneHUD/globalSceneHUD/info-window.png", 1, (FormType)1);
	form[1] = InitForm("2", formPos, "./Resources/HUD/mainSceneHUD/globalSceneHUD/gift-window.png", 3, (FormType)0);
	form[2] = InitForm("2", formPos, "./Resources/HUD/mainSceneHUD/globalSceneHUD/window.png", 4, (FormType)0);

	formIllus[0] = LoadSprite("Resources/HUD/mainSceneHUD/Form/Asset_Illu/StartExplo.png", false);
	formIllus[1] = LoadSprite("Resources/HUD/mainSceneHUD/Form/Asset_Illu/StartKnow.png", false);
	formIllus[2] = LoadSprite("Resources/HUD/mainSceneHUD/Form/Asset_Illu/Don.png", false);


	fadeSprite = LoadSprite("Resources/HUD/HomeMenu/Fade.png", false);

	SetSliderType(&form[0], 0, (Resource)0);
	SetSliderType(&form[2], 0, (Resource)0);

	int j = 0;
	for (int i = 1; i < 3; i++)
	{
		if (i == 2)
		{
			j++;
		}

		SetSliderType(&form[i], j, (Resource)1);
		SetSliderType(&form[i], j+1, (Resource)2);
		SetSliderType(&form[i], j+2, (Resource)3);
	}

	formDisplayed = -1;

	for (int i = 0; i < 3; i++)
	{
		resourcesHUD[i] = InfoBarProto(i, NULL);
	}

	for (int i = 0; i < 2; i++)
	{
		expeditionsPanels[i] = PanelProto(i);
		buildsPanels[i] = BuildPanelProto(i);
	}

	expChoice = CreateExpMenuManagement(mode);
	buildChoice = CreateBuildMenuManagement(mode);
	builderChoice = CreateBuildersMenuManagement(mode);

	fortressPanel = FortressPanelProto();

	InitCardUI();
	initDice(mode);

	pauseMenu = CreatePauseMenu(mode);

	gameOverMenu = CreateGameOverMenu(mode);
	gameWonMenu = CreateGameWonMenu(mode);
}


void ResetMainSceneHud()
{
	infoText.displayTime = 3.0f;
	infoText.currentDisplayTime = 0.0f;
	infoText.isDisplay = false;
	expChoice.isActive = false;
	buildChoice.isActive = false;
	builderChoice.isActive = false;
	isUsingLastChance = false;
	isFadeOutMusic = false;

	for (int i = 0; i < 2; i++)
	{
		ResetExpPanel(&expeditionsPanels[i]);
		ResetBuildPanel(&buildsPanels[i]);
	}
	ResetFortressPanel(&fortressPanel);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < resourcesHUD[i].modifDisplayerActive; j++)
		{
			resourcesHUD[i].modifDisplayers[j].activeTime = 0.0f;
			resourcesHUD[i].modifDisplayers[j].isActive = false;
		}	
		resourcesHUD[i].modifDisplayerActive = 0;
	}
	hideCardUI();

	UpdateTurnDisplayHUD(1);

	fadeBetweenMenu = false;
	isWin = false;
	endGameFade = false;
	endGameFadeTime = 0.0f;
	sfColor color = sfWhite;

	sfText_setColor(turnText, color);

	for (int i = 0; i < mainSceneHUD.buttonNumber; i++)
	{
		if (mainSceneHUD.button[i].sprite != NULL)
		{
			sfSprite_setColor(mainSceneHUD.button[i].sprite, color);
		}
	}
	sfSprite_setColor(mainSceneHUD.background, color);
	sfSprite_setColor(turnBackground, color);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < resourcesHUD[i].elementsNumber; j++)
		{
			sfSprite_setColor(resourcesHUD[i].resourcesElements[j].image, color);
			sfText_setColor(resourcesHUD[i].resourcesElements[j].text, sfBlack);
		}
	}
}

// --------------------------------------------------------------------------------------------------------
void InitTextHUD(int *data, int index)
{
	if (index <= 2)
	{
		InitResourceBarWithData(&resourcesHUD[index], data);
	}
	else
	{
		index -= 3;
		UpdateResourcesBarPreview(&resourcesHUD[index], data);
	}
}

// --------------------------------------------------------------------------------------------------------
//SceneID : 0 is mainMenu ; 1 is mainScene ; 2 is pauseMenu
int CheckHitBoxWithHUD(sfVector2i mousePos, GameState state)
{	
	if (fadeBetweenMenu || isUsingLastChance)
	{
		return -1;
	}

	switch (state.actualscene)
	{
		int temp;
		case 0:
			temp = GetButtonsValue(mousePos, mainMenuHUD.button, mainMenuHUD.buttonNumber);
			if (temp == 0)
			{
				if (loadScene.isLoaded)
				{
					return(temp);
				}
				else
				{
					return(-1);
				}
			}
				
			return(temp);
		case mainScene:
			if (!HasFinishToDisplayEndTurnChange(resourcesHUD, 3) || IsCardDisplay() || state.onGoingGameStatus.gameIsFinishing)
			{
				return -1;
			}

			if (builderChoice.isActive)
			{
				return(GetButtonsValue(mousePos, builderChoice.button, builderChoice.buttonNumber));
			}

			if (state.onGoingGameStatus.hasToFillAForm)
			{
				int id = state.onGoingGameStatus.playerSelectionID <= 1 ? 0 : state.onGoingGameStatus.playerSelectionID - 1;
				return(GetButtonsValue(mousePos, form[id].buttons, form[id].numberOfButtons)); // checks if a button has been pressed
			}
			else if (state.onGoingGameStatus.hasOpenFortressPanel)
			{
				return(GetButtonsValue(mousePos, fortressPanel.buttonsQuitandSendRes, 2));
			}
			else if (state.onGoingGameStatus.hasExpToHandle)
			{
				if (!expChoice.isActive)
				{
					for (int i = 0; i < 3; i++)
					{
						int value = GetButtonsValue(mousePos, &expeditionsPanels[state.onGoingGameStatus.actualPlayerID].expeditionsButt[i].expeditionButton, 1);
						if (value != -1 && !expeditionsPanels[state.onGoingGameStatus.actualPlayerID].expeditionsButt[i].expIsManaged)
						{
							return(value);
						}			
					}
					return -1;
				}
				else
				{
					return(GetButtonsValue(mousePos, expChoice.button, expChoice.buttonNumber));
				}
			}
			else if (state.onGoingGameStatus.hasOpenBuildPanel)
			{
				if (!buildChoice.isActive)
				{
					for (int i = 0; i < 3; i++)
					{
						int value = GetButtonsValue(mousePos, &buildsPanels[state.onGoingGameStatus.actualPlayerID].buildsButt[i].buildButton, 1);

						if (value != -1 && !buildsPanels[state.onGoingGameStatus.actualPlayerID].buildsButt[i].isBuild)
						{
							return(value);
						}
					}
					int temp = GetButtonsValue(mousePos, mainSceneHUD.button, mainSceneHUD.buttonNumber);
					if (temp >= 4)
					{
						return temp;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return(GetButtonsValue(mousePos, buildChoice.button, buildChoice.buttonNumber));
				}
			}

			return(GetButtonsValue(mousePos, mainSceneHUD.button, mainSceneHUD.buttonNumber));
		case pause:
			temp = GetButtonsValue(mousePos, pauseMenu.button, pauseMenu.buttonNumber);

			if (temp == 1)
			{
				HANDLE myThread3 = CreateThread(NULL, 0, ReLoadAnim, (void*)0, 0, NULL);
				HANDLE myThread4 = CreateThread(NULL, 0, ReLoadAnim, (void*)1, 0, NULL);

				fadeBetweenMenu = true;
				fadeBetweenMenuTime = 0.0f;
				//Add Fade in Fade Out
			}
			return (temp);
		case godSelection:
			temp = GetGodSelectionButtonsIndex(mousePos);

			if (temp == 10)
			{
				HANDLE myThread3 = CreateThread(NULL, 0, ReLoadAnim, (void*)0, 0, NULL);
				HANDLE myThread4 = CreateThread(NULL, 0, ReLoadAnim, (void*)1, 0, NULL);

				fadeBetweenMenu = true;
				fadeBetweenMenuTime = 0.0f;
			}

			return temp;
		case gameOverScene:
			if (isWin)
			{
				temp = GetButtonsValue(mousePos, gameWonMenu.button, gameWonMenu.buttonNumber);
			}
			else
			{
				temp = GetButtonsValue(mousePos, gameOverMenu.button, gameOverMenu.buttonNumber);
			}

			if (temp == 0)
			{
				fadeBetweenMenu = true;
				fadeBetweenMenuTime = 0.0f;
			}
			return temp;
		default:
			printf("Error scene state doesn't actualy exist.\n");
			return -1;
	}
}

//Return if selection is finished
// --------------------------------------------------------------------------------------------------------
bool SelectGodAndStarter(int godID)
{
	GetGodPlayer(godID);
	bool temp = CheckSelectionIsFinished();
	if (temp)
	{
		fadeBetweenMenu = true;
		fadeBetweenMenuTime = 0.0f;
	}
	return(temp);
}

// --------------------------------------------------------------------------------------------------------
void ChangeBuildPanelState(int id, int valueOverride)
{
	if (valueOverride == -1)
	{
		buildsPanels[id].isActive = !buildsPanels[id].isActive;
	}
	else
	{
		buildsPanels[id].isActive = valueOverride;
	}
}

// --------------------------------------------------------------------------------------------------------
void ChangeExPanelState(int id, int valueOverride)
{
	if (valueOverride == -1)
	{
		expeditionsPanels[id].isActive = !expeditionsPanels[id].isActive;
	}
	else
	{
		expeditionsPanels[id].isActive = valueOverride;
		if (valueOverride == 1)
		{
			for (int i = 0; i < 3; i++)
			{
				ChangeExpButtonsColor(&expeditionsPanels[id].expeditionsButt[i], true);
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------
void ChangeFortressPanelState(int valueOverride)
{
	if (valueOverride == -1)
	{
		fortressPanel.isActive = !fortressPanel.isActive;
	}
	else
	{
		fortressPanel.isActive = valueOverride;
	}
}

// --------------------------------------------------------------------------------------------------------
void ChangeExpChoiceState(int id)
{
	if (id == 0)
	{
		expChoice.isActive = !expChoice.isActive;
	}
	else if(id == 1)
	{
		buildChoice.isActive = !buildChoice.isActive;
	}
	else
	{
		builderChoice.isActive = !builderChoice.isActive;
	}
}

// --------------------------------------------------------------------------------------------------------
void BuildsPanelIsBuild(int index, int actualPlayerID)
{
	UpdateBuildButtonsUpgradeBuild(&buildsPanels[actualPlayerID].buildsButt[index]);
}

// --------------------------------------------------------------------------------------------------------
void UpdateIntelsPanel(int* data, int actualPlayerID)
{
	if(data[0] >= 3)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!buildsPanels[actualPlayerID].buildsButt[i].buildButton.isWorking)
			{
				UpdateBuildButtons(&buildsPanels[actualPlayerID].buildsButt[i], data);
				return;
			}
		}
	}
	else
	{
		UpdateFortressDisplayButton(&fortressPanel.fortressDisplayButton[data[1]], data);
	}
}

// --------------------------------------------------------------------------------------------------------
void UpdateCardDisplayerPanel(int*data, int id)
{
	if (id == 0)
	{
		SetExpeditionCardUI(data);
	}
	else
	{
		SetKnoledgeCardUI(data);
	}
}

// --------------------------------------------------------------------------------------------------------
//The first index of data is the actual index of button
void UpdateExpeditionsPanels(int* data, int actualPlayerID)
{
	expeditionsPanels[actualPlayerID].expeditionsButt[data[0]].expIsManaged = true;
	UpdateExpeditionsButton(expeditionsPanels[actualPlayerID].expeditionsButt, expeditionsPanels[actualPlayerID].higherButtonInPanel, data, actualPlayerID);
	ChangeExpButtonsColor(&expeditionsPanels[actualPlayerID].expeditionsButt[data[0]], true);
}

// --------------------------------------------------------------------------------------------------------
//Reset Expedition Status that avoid the user to click to them to managed them when they are actualy already managed
void ResetExpeditionsHUD(int playerID)
{
	for (int i = 0; i < 3; i++)
	{
		expeditionsPanels[playerID].expeditionsButt[i].expIsManaged = false;
		ChangeExpButtonsColor(&expeditionsPanels[playerID].expeditionsButt[i], false);
	}
}

// --------------------------------------------------------------------------------------------------------
void SendInputToForm(int idForm, int value)
{
	switch (idForm)
	{
		case 0:
		case 1:
			InputToForms(&form[0], value);
			break;
		case 2:
			InputToForms(&form[1], value);
			break;
		case 3:
			InputToForms(&form[2], value);
			break;
	}
}

// --------------------------------------------------------------------------------------------------------
void UpdateForm(int inputIndex, int* data)
{
	switch (inputIndex)
	{
		case 0:
			formDisplayed = 0;
			SetFormTitle(&form[formDisplayed], "Expedition for resources:");
			break;
		case 1:
			formDisplayed = 0;
			SetFormTitle(&form[formDisplayed], "Expedition for knowledges:");
			break;
		case 2:
			formDisplayed = 1;
			SetFormTitle(&form[formDisplayed], "Gift to the other tribe:");
			for (int i = 0; i < 3; i++)
			{
				data[i] = data[i + 1];
			}
			break;
		case 3:
			formDisplayed = 2;
			SetFormTitle(&form[formDisplayed], "Sending to fortress:");
		default:
		break;
	}

	form[formDisplayed].isActive = true;
	ChangeFormValues(&form[formDisplayed], data);

	if (inputIndex != 3) // the "sending to fortress" form doesn't have an illustration
	{
		SetFormIllu(&form[formDisplayed], formIllus[inputIndex]);
	}
}

DWORD WINAPI FadeDelay(void* data)
{
	Sleep(4000);
	endGameFade = true;
	return 0;
}

// --------------------------------------------------------------------------------------------------------
void UpdateText(int* data, int index)
{
	if (index <= 2)
	{
		UpdateResourcesBarInfos(&resourcesHUD[index], data);
	}
	else if (index == 21 || index == 22)
	{
		isWin = index - 21;
		HANDLE myThread = CreateThread(NULL, 0, FadeDelay, NULL, 0, NULL);
	}
	else
	{
		index -= 3;
		UpdateResourcesBarPreview(&resourcesHUD[index], data);
	}
}

// --------------------------------------------------------------------------------------------------------
void ChangeNewTurnHUD(GameState actualState, bool canUseLastChance)
{
	int playerID = actualState.onGoingGameStatus.actualPlayerID;
	SwitchButtonState(&mainSceneHUD.button[2], playerID == 1 ? true : false);
	SwitchButtonState(&mainSceneHUD.button[3], playerID == 0 ? true : false);
	SwitchButtonState(&mainSceneHUD.button[5], playerID == 0 ? true : false);
	SwitchButtonState(&mainSceneHUD.button[6], playerID == 1 ? true : false);
	SwitchButtonState(&mainSceneHUD.button[9], playerID == 0 ? true : false);
	SwitchButtonState(&mainSceneHUD.button[10], playerID == 1 ? true : false);
	
	if (canUseLastChance)
	{
		SwitchButtonState(&mainSceneHUD.button[7 + playerID], true);
	}

	if (mainSceneHUD.button[8 - playerID].isWorking)
	{
		SwitchButtonState(&mainSceneHUD.button[8 - playerID], false);
	}
}

// --------------------------------------------------------------------------------------------------------
void UpdateTurnDisplayHUD(int actualTurn)
{
	sfVector2f newTextPos = sfText_getPosition(turnText);
	UpdateTextDisplay(turnText, actualTurn);
	newTextPos.x = 1920 / 2 - sfText_getGlobalBounds(turnText).width / 2;
	sfText_setPosition(turnText, newTextPos);
}

void ProceedDiceThrow(int *Nullable, int toFinishOn)
{
	setDiceValue(toFinishOn);
	LaunchDice();
	isUsingLastChance = true;
}

// --------------------------------------------------------------------------------------------------------
void SetInfoText(int type)
{
	sfVector2f currentTextPos;
	sfVector2f textPos;
	
	switch (type)
	{
	case 1:
		sfText_setString(infoText.text, "YOU DON'T HAVE ENOUGH RESOURCES\n\t\t\t\t\t\t\tTO BUILD THIS");
		sfText_setCharacterSize(infoText.text, 55);
		break;
	case 2:
		sfText_setString(infoText.text, "YOU ALREADY HAVE 3 EXPEDITIONS IN PROGRESS");
		sfText_setCharacterSize(infoText.text, 60);
		break;
	case 3:
		sfText_setString(infoText.text, "NEW TURN");
		sfText_setCharacterSize(infoText.text, 80);
		break;
	default:
		break;
	}

	textPos.x = 1920 / 2 - sfText_getGlobalBounds(infoText.text).width / 2;
	textPos.y = 1080 / 2;

	printf("text pos x : %f\n", textPos.x);
	sfText_setPosition(infoText.text, textPos);
	
	infoText.isDisplay = true;
}

// --------------------------------------------------------------------------------------------------------
void UpdateInfoText(float deltaTime)
{
	if (infoText.isDisplay)
	{
		if (infoText.currentDisplayTime < infoText.displayTime)
		{
			infoText.currentDisplayTime += deltaTime;
		}
		else
		{
			infoText.currentDisplayTime = 0.0f;
			infoText.isDisplay = false;
		}
	}
}

void Fade(float deltaTime)
{
	sfColor color = sfWhite;
	sfColor color2 = sfBlack;
	float alphaRatio = (float)((3.0 - endGameFadeTime) / 3.0);
	color2.a = color.a = (int)(alphaRatio * 255);
	for (int i = 0; i < mainSceneHUD.buttonNumber; i++)
	{
		if(mainSceneHUD.button[i].sprite != NULL)
		{
			sfSprite_setColor(mainSceneHUD.button[i].sprite, color);
		}
	}

	sfText_setColor(turnText, color);
	sfSprite_setColor(mainSceneHUD.background, color);
	sfSprite_setColor(turnBackground, color);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < resourcesHUD[i].elementsNumber; j++)
		{
			sfSprite_setColor(resourcesHUD[i].resourcesElements[j].image, color);
			sfText_setColor(resourcesHUD[i].resourcesElements[j].text, color2);
		}
	}

	endGameFadeTime += deltaTime;
}


DWORD WINAPI DelayMusicIn(void* data)
{
	sfMusic_setVolume(mainSceneBackGroundMusic, 0.0f);
	MusicPlay(mainSceneBackGroundMusic);
	sfMusic_setVolume(summerBGMusic, 0.0f);
	MusicPlay(summerBGMusic);
	while (sfMusic_getVolume(summerBGMusic) < 100.0f)
	{
		if (sfMusic_getVolume(mainSceneBackGroundMusic) < 65.0f)
		{
			sfMusic_setVolume(mainSceneBackGroundMusic, sfMusic_getVolume(mainSceneBackGroundMusic) + 1.0f);
		}
		sfMusic_setVolume(summerBGMusic, sfMusic_getVolume(summerBGMusic) + 1.0f);
		Sleep(80);
	}
	return 0;
}


DWORD WINAPI DelayMusicOut(void* data)
{
	while (sfMusic_getVolume(winterBGMusic) > 1.0f)
	{
		sfMusic_setVolume(mainSceneBackGroundMusic, sfMusic_getVolume(mainSceneBackGroundMusic) - 1.0f);
		sfMusic_setVolume(winterBGMusic, sfMusic_getVolume(winterBGMusic) - 1.0f);
		Sleep(40);
	}

	sfMusic_stop(mainSceneBackGroundMusic);
	sfMusic_stop(winterBGMusic);
	sfMusic_setVolume(mainSceneBackGroundMusic, 65.0f);
	sfMusic_setVolume(winterBGMusic, 100.0f);

	MusicPlay(endMusicEpic);
	isFadeOutMusic = false;
	return 0;
}


// --------------------------------------------------------------------------------------------------------
void UpdateHUD(sfRenderWindow* mainWindow, GameState *actualState, void(*cbData)(int*), void(*cbSetGodHUD)(int, int), float deltaTime, void(*cbUpdateGameState)())
{
	if (fadeBetweenMenu)
	{
		if(fadeBetweenMenuTime <= fadeBetweenMenuTimeMax)
		{
			sfColor temp = sfSprite_getColor(fadeSprite);
			if (fadeBetweenMenuTime >= fadeBetweenMenuTimeMax / 2.0f)
			{
				temp.a = (int)(((fadeBetweenMenuTimeMax - fadeBetweenMenuTime) /3.0f) * 255);
			}
			else
			{
				temp.a = (int)(((fadeBetweenMenuTime) / 3.0f) * 255);
			}
			sfSprite_setColor(fadeSprite, temp);

			fadeBetweenMenuTime += deltaTime;

			if (sfMusic_getStatus(mainMenuBackGroundMusic) == sfPlaying && actualState->actualscene != mainMenu && actualState->actualscene != godSelection)
			{
				sfMusic_setVolume(mainMenuBackGroundMusic, sfMusic_getVolume(mainMenuBackGroundMusic) - deltaTime * 25.0f);
			}
			if (sfMusic_getStatus(mainSceneBackGroundMusic) == sfPlaying)
			{
				sfMusic_setVolume(mainSceneBackGroundMusic, sfMusic_getVolume(mainSceneBackGroundMusic) - deltaTime * 17.0f) ;
			}
			if (sfMusic_getStatus(summerBGMusic) == sfPlaying)
			{
				sfMusic_setVolume(summerBGMusic, sfMusic_getVolume(summerBGMusic) - deltaTime * 33.0f);
			}
			if (sfMusic_getStatus(winterBGMusic) == sfPlaying)
			{
				sfMusic_setVolume(winterBGMusic, sfMusic_getVolume(winterBGMusic) - deltaTime * 33.0f);
			}

			if (sfMusic_getStatus(endMusicEpic) == sfPlaying && actualState->actualscene != mainScene)
			{
				sfMusic_setVolume(endMusicEpic, sfMusic_getVolume(endMusicEpic) - deltaTime * 17.0f);
			}
		}
		else
		{
			fadeBetweenMenu = false;
			if (sfMusic_getStatus(mainMenuBackGroundMusic) == sfPlaying && actualState->actualscene != mainMenu)
			{
				sfMusic_stop(mainMenuBackGroundMusic);
				sfMusic_setVolume(mainMenuBackGroundMusic, 50.0f);
			}
			if (sfMusic_getStatus(mainSceneBackGroundMusic) == sfPlaying)
			{
				sfMusic_stop(mainSceneBackGroundMusic);
				sfMusic_setVolume(mainSceneBackGroundMusic, 65.0f);

			}
			if (sfMusic_getStatus(summerBGMusic) == sfPlaying)
			{
				sfMusic_stop(summerBGMusic);
				sfMusic_setVolume(summerBGMusic, 100.0f);

			}
			if (sfMusic_getStatus(winterBGMusic) == sfPlaying)
			{
				sfMusic_stop(winterBGMusic);
				sfMusic_setVolume(winterBGMusic, 100.0f);

			}
			if (sfMusic_getStatus(endMusicEpic) == sfPlaying)
			{
				sfMusic_stop(endMusicEpic);
				sfMusic_setVolume(endMusicEpic, 100.0f);
			}

			if (actualState->actualscene == mainMenu && sfMusic_getStatus(mainMenuBackGroundMusic) != sfPlaying)
			{
				MusicPlay(mainMenuBackGroundMusic);
			}
			if (actualState->actualscene == mainScene)
			{
				HANDLE musicThread = CreateThread(NULL, 0, DelayMusicIn, NULL, 0, NULL);
			}
		
		}
	}
	
	
	switch (actualState->actualscene)
	{
	case mainScene:		
		if (endGameFade && endGameFadeTime <= 3.0f)
		{
			Fade(deltaTime);
		}

		UpdateInfoText(deltaTime);
		
		if (!endGameFade && !isUsingLastChance)
		{
			for (int i = 0; i < 3; i++)
			{
				ConsumptionPreview(&resourcesHUD[i], sfMouse_getPositionRenderWindow(mainWindow), i);
			}
		}

		if (actualState->onGoingGameStatus.hasToFillAForm)
		{
			ManageForms(mainWindow, &form[formDisplayed], 1, sfMouse_getPositionRenderWindow(mainWindow));
			UpdateForms(&form[formDisplayed], 1);
			if (!form[formDisplayed].isActive)
			{
				actualState->onGoingGameStatus.hasToFillAForm = false;

				if (form[formDisplayed].isCanceled)
				{
					actualState->onGoingGameStatus.hasMadeAnAction = false;
				}
				else
				{
					cbData(CommitFormAction(&form[formDisplayed]));
				}
				formDisplayed = -1;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			UpdateResourcesBar(&resourcesHUD[i], deltaTime);
		}

		if (IsCardDisplay())
		{
			UpdateCardUI(deltaTime);
			if (!IsCardDisplay() && actualState->onGoingGameStatus.hasToChangeState)
			{
				actualState->onGoingGameStatus.hasToChangeState = false;
				cbUpdateGameState();
			}
		}

		if (isUsingLastChance)
		{
			UpdateDice(deltaTime);
			isUsingLastChance = !IsFinalFace();
			if (!isUsingLastChance)
			{
				actualState->onGoingGameStatus.hasUsedLastChance = true;
			}
		}


		if (actualState->onGoingGameStatus.turn > 10 && sfMusic_getStatus(summerBGMusic) == sfPlaying)
		{
			MusicPlay(winterBGMusic);
			sfMusic_stop(summerBGMusic);
		}

		if(actualState->onGoingGameStatus.turn > 18 && sfMusic_getStatus(endMusicEpic) != sfPlaying && !isFadeOutMusic)
		{
			HANDLE fadeMusicThread = CreateThread(NULL, 0, DelayMusicOut, NULL, 0, NULL);
			isFadeOutMusic = true;
		}

		break;
	case godSelection:
		GodHoverAnimation(sfMouse_getPositionRenderWindow(mainWindow), deltaTime);
		UpdateGodSelectionMenu(cbData, cbSetGodHUD, deltaTime);
		break;

	case loadingScene:
		if (loadScene.actualFrameToShow < StatusConvertedInFrame)
		{
			if(deltaTime )
			UpdateAnimation(&loadScene, deltaTime);
			UpdateAnimation(&msgLoad, deltaTime);
		}
		break;
	case gameOverScene:
		if (!endGameFade && !isUsingLastChance)
		{
			for (int i = 0; i < 3; i++)
			{
				ConsumptionPreview(&resourcesHUD[i], sfMouse_getPositionRenderWindow(mainWindow), i);
			}
		}

		for (int i = 0; i < 3; i++)
		{
			UpdateResourcesBar(&resourcesHUD[i], deltaTime);
		}
		break;
	default:
		break;
	}
}

// --------------------------------------------------------------------------------------------------------
void DisplayHUD(sfRenderWindow* mainWindow, Scene actualScene)
{	
	switch (actualScene)
	{
		case 0:
			DisplayMenu(mainMenuHUD, mainWindow);
			break;
		case 2:
		case 1:
			DisplayMenu(mainSceneHUD, mainWindow);
			DisplayExpeditionPanel(expeditionsPanels, mainWindow);		
			DisplayBuildPanel(buildsPanels, mainWindow);
			DisplayFortressPanel(fortressPanel, mainWindow);
			DisplayCardUI(mainWindow);

			// Turn display
			sfRenderWindow_drawSprite(mainWindow, turnBackground, NULL);
			sfRenderWindow_drawText(mainWindow, turnText, NULL);

			if (formDisplayed != -1)
			{
				DisplayForms(mainWindow, &form[formDisplayed], 1);
			}

			for (int i = 0; i < 3; i++)
			{
				DisplayResourcesBar(mainWindow, &resourcesHUD[i]);
			}
			if (expChoice.isActive)
			{
				DisplayMenu(expChoice, mainWindow);
			}
			else if (buildChoice.isActive)
			{
				DisplayMenu(buildChoice, mainWindow);
			}
			else if (builderChoice.isActive)
			{
				DisplayMenu(builderChoice, mainWindow);
			}

			if (infoText.isDisplay)
			{
				sfRenderWindow_drawText(mainWindow, infoText.text, NULL);
			}
			
			if (isUsingLastChance)
			{
				DisplayDice(mainWindow);
			}
			
			if (actualScene == 2)
			{
				DisplayMenu(pauseMenu, mainWindow);
			}

		break;
		case 3 :
			DisplaySelectionScene(mainWindow);
			break;
		case 4 :
			for (int i = 0; i < 3; i++)
			{
				DisplayResourcesBar(mainWindow, &resourcesHUD[i]);
			}
			
			if (isWin)
			{
				DisplayMenu(gameWonMenu, mainWindow);
			}
			else
			{
				DisplayMenu(gameOverMenu, mainWindow);
			}
			break;
		case 5:
			sfRenderWindow_drawSprite(mainWindow, logoCrea, NULL);
			sfRenderWindow_drawSprite(mainWindow, msgLoad.spriteArray[msgLoad.actualFrameToShow], NULL);
			sfRenderWindow_drawSprite(mainWindow, loadScene.spriteArray[loadScene.actualFrameToShow], NULL);
			break;
	}

	if (fadeBetweenMenu)
	{
		sfRenderWindow_drawSprite(mainWindow, fadeSprite, NULL);
	}
}