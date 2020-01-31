#include "Form.h"

#define DEFAULTDIFFERENCE 10000 // used for the precision of the sliders
#define STRINGLIMIT 20 // the max number of characters for the strings

Form InitForm(char* formTitle, sfVector2f position, char* pathOfSpriteToLoad, int numberOfSliders, FormType type)
{
	Form form;
	sfFloatRect cursorRect;
	sfVector2f tempPosition;
	sfVector2f sliderPosition;
	sfVector2f cursorOrigin;
	sfVector2f buttonSize;
	sfVector2f sliderSize;
	sfVector2f sliderScale;
	float scaleElementsRatio;


	sfColor color = sfBlack; // the texts' color
	int fontSize = 35; // the texts' size

	/////////////////////////////////////
	form.font = sfFont_createFromFile("./Resources/Font/Action of the Time II.ttf"); // the texts' font
	////////////////////////////////////

	// init the form
	form.isActive = false; // the form is disabled by default
	form.type = type;
	form.activeSlidersNumber = numberOfSliders;
	form.numberOfButtons = 3;

	form.sprite = LoadSprite(pathOfSpriteToLoad, false);
	sfSprite_setPosition(form.sprite, position);

	form.formRect = sfSprite_getGlobalBounds(form.sprite);

	tempPosition.x = position.x + 50.0f;
	tempPosition.y = position.y + 20.0f;

	sliderPosition.x = position.x;
	sliderPosition.y = position.y;

	form.title = InitText(formTitle, tempPosition, sfBlack, fontSize);

	scaleElementsRatio = 400.0f / 500.0f;
	sliderScale.x = scaleElementsRatio;
	sliderScale.y = scaleElementsRatio;

	sliderSize.x = 500.0f * scaleElementsRatio;
	sliderSize.y = 45.0f * scaleElementsRatio;

	if (numberOfSliders == 3)
	{
		sliderPosition.y += (form.formRect.height / 4.0f);
	}

	// init the sliders
	for (int i = 0; i < numberOfSliders; i++)
	{
		// load the sprites
		form.sliders[i].sliderSprite = LoadSprite("./Resources/HUD/mainSceneHUD/Form/slider.png", false);
		form.sliders[i].cursorSprite = LoadSprite("./Resources/HUD/mainSceneHUD/Form/cursor.png", false);

		// init the slider

		sliderPosition.x = form.formRect.left + ((form.formRect.width / 2) - (sliderSize.x / 2));

		switch (numberOfSliders)
		{
		case 1:
			sliderPosition.y += (form.formRect.height / 2.0f) + 150.0f;
			break;
		case 3 :
			sliderPosition.y += form.formRect.height / 6.0f;
			break;
		case 4:
			sliderPosition.y += form.formRect.height / 6.0f;
			break;
		default:
			printf("NTM ! l.57 Form.c\n");
			break;
		}
		sfSprite_setScale(form.sliders[i].sliderSprite, sliderScale);

		sfSprite_setPosition(form.sliders[i].sliderSprite, sliderPosition);
		form.sliders[i].rect = sfSprite_getGlobalBounds(form.sliders[i].sliderSprite);

		// init the cursor
		sfSprite_setScale(form.sliders[i].cursorSprite, sliderScale);
		cursorRect = sfSprite_getLocalBounds(form.sliders[i].cursorSprite);
		cursorOrigin.x = cursorRect.width / 2.0f;
		cursorOrigin.y = cursorRect.top;
		sfSprite_setOrigin(form.sliders[i].cursorSprite, cursorOrigin);
		form.sliders[i].cursorPosition.y = sliderPosition.y + 3.f;
		sfSprite_setPosition(form.sliders[i].cursorSprite, form.sliders[i].cursorPosition);
		sfSprite_setColor(form.sliders[i].cursorSprite, (sfColor) { 150, 150, 150, 255 });

		// init the icon pos

		tempPosition.x = position.x + 20;
		tempPosition.y = sliderPosition.y - 3.0f;
		form.sliders[i].icon = sfSprite_create();
		sfSprite_setPosition(form.sliders[i].icon, tempPosition);
		
		// init the texts
		/////////////////////////////////////////
		fontSize = 25;
		////////////////////////////////////////
		tempPosition.x = form.sliders[i].rect.left + form.sliders[i].rect.width + 10.0f;
		tempPosition.y = sliderPosition.y + 3.0f;

		////////////////////////////////////////
		//tempPosition.y = form.sliders[i].rect.top;
		////////////////////////////////////////
		form.sliders[i].value = InitText("", tempPosition, color, fontSize);
	}

	// init the buttons
	buttonSize.x = 70.0f;
	buttonSize.y = 70.0f;

	// the cancel button
	///////////////////////
	//tempPosition.x = form.formRect.left;
	tempPosition.x = form.formRect.left + form.formRect.width / 2 - buttonSize.x - 50.0f ;
	tempPosition.y = form.formRect.top + form.formRect.height - buttonSize.y - 20.0f;
	//////////////////////
	form.buttons[0] = InitButton(buttonSize, tempPosition, 0, "./Resources/HUD/mainSceneHUD/Form/cancel.png", false);

	// the confirm button
	/////////////////////
	//tempPosition.x = form.formRect.left + form.formRect.width / 2.0f;
	//tempPosition.y = form.formRect.top + form.formRect.height - buttonSize.y;
	tempPosition.x = form.formRect.left + form.formRect.width / 2 + 50.0f;
	tempPosition.y = form.formRect.top + form.formRect.height - buttonSize.y - 20.0f;
	////////////////////
	form.buttons[1] = InitButton(buttonSize, tempPosition, 1, "./Resources/HUD/mainSceneHUD/Form/valid.png", false);

	// the move button
	tempPosition.x = 0.0f;
	tempPosition.y = 0.0f;
	buttonSize.x = 30.0f;
	buttonSize.y = 30.0f;
	form.buttons[2] = InitButton(buttonSize, tempPosition, 2, "./Resources/HUD/mainSceneHUD/Form/move-button.png", false);

	//////////////////////////////////////////
	//SwitchButtonDisplay(&form.buttons[2], false);
	/////////////////////////////////////////

	tempPosition.x = form.formRect.left + form.formRect.width - form.buttons[2].rect.width;
	tempPosition.y = form.formRect.top;
	ChangeButtonPosition(&form.buttons[2], tempPosition);

	// the illu
	form.illu = sfSprite_create();
	form.illuPosition.x = form.formRect.left + (55 / 2) - 1.5f;
	form.illuPosition.y = form.formRect.top + 58.f;

	return form;
}

void SetFormTitle(Form* form, char* newTitle)
{
	sfText_setString(form->title, newTitle);
}

void SetSliderType(Form* form, int PlaceOfSliderToInit, Resource type)
{
	sfTexture* tmpTexture;
	form->sliders[PlaceOfSliderToInit].type = type;
	switch (type)
	{
	case villager :
		tmpTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/people.png", NULL);
		break;
	case food:
		tmpTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/food.png", NULL);
		break;
	case water:
		tmpTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/water.png", NULL);
		break;
	case materials:
		tmpTexture = sfTexture_createFromFile("./Resources/HUD/mainSceneHUD/Asset_Icon/wood.png", NULL);
		break;
	default:
		break;
	}
	sfTexture_setSmooth(tmpTexture, sfTrue);
	sfSprite_setTexture(form->sliders[PlaceOfSliderToInit].icon, tmpTexture, 1);
}

void SetSliderValues(Form* form, int PlaceOfSliderToSetTheValues, int minValue, int maxValue, int differenceBetweenValues)
{
	form->sliders[PlaceOfSliderToSetTheValues].minValue = minValue;
	form->sliders[PlaceOfSliderToSetTheValues].currentValue = minValue; // init the slider's default value at its min value
	form->sliders[PlaceOfSliderToSetTheValues].maxValue = maxValue;
	form->sliders[PlaceOfSliderToSetTheValues].differenceBetweenValues = differenceBetweenValues;
}

void SwitchFormState(Form* form)
{
	form->isActive = !form->isActive; // changes the form's state

	if (form->isActive) // if the form has just been turned on
	{

		for (int i = 0; i < form->activeSlidersNumber; i++)
		{
			form->sliders[i].currentValue = form->sliders[i].minValue; // set the sliders' default values at their min value
		}
	}
}

void ManageForms(sfRenderWindow* window, Form forms[], int numberOfFormsToManage, sfVector2i mousePosition)
{
	static int slider = -1; // used to remember which slider has been clicked on (-1 for "no slider pressed")
	static bool isMoving = false; // used to remember if the player is moving the form
	int buttonPressed; // used to know when the player clicks on a button, and which one is clicked
	sfVector2f position = { (float)mousePosition.x, (float)mousePosition.y }; // used to move the form
	sfVector2u windowSize; // the window size
	sfFloatRect windowRect = { forms[0].buttons[2].rect.width / 2.0f , forms[0].buttons[2].rect.height / 2.0f, 0.0f, 0.0f }; // the area containing the forms

	windowSize = sfRenderWindow_getSize(window);
	windowRect.width = (float)windowSize.x /*- forms[0].formRect.width*/ + forms[0].buttons[2].rect.width / 2.0f;
	windowRect.height = (float)windowSize.y /*- forms[0].formRect.height*/ + forms[0].buttons[2].rect.height / 2.0f;

	for (int i = 0; i < numberOfFormsToManage; i++)
	{
		if (forms[i].isActive) // if the form is active
		{
			if (sfMouse_isButtonPressed(sfMouseLeft))
			{
				for (int j = 0; j < forms[i].activeSlidersNumber; j++) // checks all sliders of the form
				{
					if (sfFloatRect_contains(&forms[i].sliders[j].rect, (float)mousePosition.x, (float)mousePosition.y) && slider == -1) // if the player clicked on the slider
					{
						slider = j; // registers which slider has been clicked on
						sfSprite_setColor(forms[i].sliders[slider].cursorSprite, (sfColor) { 255, 100, 100, 255 });
					}
				}

				if (slider == -1)
				{
					buttonPressed = GetButtonsValue(mousePosition, forms[i].buttons, forms[i].numberOfButtons); // checks if a button has been pressed
					if (buttonPressed == 2)
					{
						isMoving = true; // set the form as moving
					}
				}
			}
			else // when the player releases the mouse left button
			{
				if (slider != -1)
				{
					sfSprite_setColor(forms[i].sliders[slider].cursorSprite, (sfColor) { 150, 150, 150, 255 });
				}
				
				slider = -1; // removes the slider that was clicked on of the memory
				isMoving = false; // set the form as not moving
			}

			if (slider != -1 && !isMoving) // if the player is currently clicking on a slider
			{
				// changes the value of the slider according to the mouse position (a kind of lerp)
				forms[i].sliders[slider].currentValue = forms[i].sliders[slider].minValue + forms[i].sliders[slider].differenceBetweenValues * ((forms[i].sliders[slider].maxValue - forms[i].sliders[slider].minValue) / forms[i].sliders[slider].differenceBetweenValues) * (mousePosition.x - (int)(forms[i].sliders[slider].rect.left + 10.0f)) / ((int)(forms[i].sliders[slider].rect.width - 20.0f));

				if (forms[i].type == uniqueResource) // if only one type of resource can be sent
				{
					for (int k = 0; k < forms[i].activeSlidersNumber; k++)
					{
						// set all sliders' values to their default value except the slider that is being clicked on
						if (k != slider)
						{
							forms[i].sliders[k].currentValue = forms[i].sliders[k].minValue;
						}
					}
				}
			}
			if (isMoving && sfFloatRect_contains(&windowRect, position.x, position.y)) // if the form is set as moving
			{
				MoveForm(&forms[i], position); // moves the form
			}
		}
	}
}


void InputToForms(Form *form, int value)
{
	int cpt = 0;
	switch (value)
	{	
		case 0: // if the player clicks on the cancel button
			SwitchFormState(form);
			form->isCanceled = true;// disables the form
			return;
		case 1: // if the player clicks on the confirm button	
			for (int i = 0; i < form->activeSlidersNumber; i++)
			{
				if (form->sliders[i].currentValue == 0)
				{
					cpt++;
				}
			}
			if (cpt == form->activeSlidersNumber)
			{
				return;
			}
			SwitchFormState(form);
			form->isCanceled = false;
			return;
		default:
			break;
	}
}


void UpdateForms(Form forms[], int numberOfFormsToManage)
{
	int difference = 0; // used for the slider's precision
	int value = 0; // used to remember which value is the closest to the value pointed by the mouse

	for (int i = 0; i < numberOfFormsToManage; i++)
	{
		if (forms[i].isActive) // if the form is active
		{
			for (int j = 0; j < forms[i].activeSlidersNumber; j++)
			{
				difference = DEFAULTDIFFERENCE; // the difference is reset to its default value for each slider (the default value is very high to be sure that at least one value is taken by the slider)

				for (int k = forms[i].sliders[j].minValue; k <= forms[i].sliders[j].maxValue; k += forms[i].sliders[j].differenceBetweenValues) // tests all possibly taken values of the slider
				{
					if (abs(forms[i].sliders[j].currentValue - k) < difference) // if the difference between the value pointed by the mouse and the tested value is the smallest so far
					{
						difference = abs(forms[i].sliders[j].currentValue - k); // register it to compair it with other differences
						value = k; // registers the value
					}
				}

				forms[i].sliders[j].currentValue = value < 0 ? 0 : value; // the slider takes the value of the closest possible position

				// the cursor position is reset according to the value taken (a kind of lerp)
				forms[i].sliders[j].cursorPosition.x = (forms[i].sliders[j].rect.left + 10.0f) + (forms[i].sliders[j].rect.width - 20.0f) * (float)(forms[i].sliders[j].currentValue - forms[i].sliders[j].minValue) / (float)(forms[i].sliders[j].maxValue - forms[i].sliders[j].minValue);
				sfSprite_setPosition(forms[i].sliders[j].cursorSprite, forms[i].sliders[j].cursorPosition);

				UpdateTextDisplay(forms[i].sliders[j].value, forms[i].sliders[j].currentValue);
			}
		}
	}
}

void DisplayForms(sfRenderWindow* window, Form forms[], int numberOfForms)
{
	for (int i = 0; i < numberOfForms; i++)
	{
		if (forms[i].isActive)
		{
			BlitSprite(window, forms[i].illu, forms[i].illuPosition.x, forms[i].illuPosition.y); // the form's illu
			sfRenderWindow_drawSprite(window, forms[i].sprite, NULL); // the form's sprite
			sfRenderWindow_drawText(window, forms[i].title, NULL); // the form's title
			DisplayButtons(window, forms[i].buttons, forms[i].numberOfButtons); // the buttons

			// display all sliders
			for (int j = 0; j < forms[i].activeSlidersNumber; j++)
			{
				sfRenderWindow_drawSprite(window, forms[i].sliders[j].icon, NULL);
				sfRenderWindow_drawSprite(window, forms[i].sliders[j].sliderSprite, NULL); // the slider's sprite
				sfRenderWindow_drawSprite(window, forms[i].sliders[j].cursorSprite, NULL); // its cursor
				sfRenderWindow_drawText(window, forms[i].sliders[j].value, NULL); // it's value
			}
		}
	}
}

int* CommitFormAction(Form* form)
{
	int *loss = calloc(4, sizeof(int)); // the amount of resources lost (1 : villagers; 2 : food; 3 : water; 4 : materials)

	for (int i = 0; i < form->activeSlidersNumber; i++) // checks all the form's sliders
	{
		// add the slider's value to the correct amount according to the slider's type
		switch (form->sliders[i].type)
		{
		case villager:
			loss[0] += form->sliders[i].currentValue;
			break;
		case food:
			loss[1] += form->sliders[i].currentValue;
			break;
		case water:
			loss[2] += form->sliders[i].currentValue;
			break;
		case materials:
			loss[3] += form->sliders[i].currentValue;
			break;
		}
	}
	return loss;
}

char* IndexToType(int value)
{
	switch (value)
	{
	case 0:
		return "Villagers :";
	case 1:
		return "Food :";
	case 2:
		return "Water :";
	case 3:
		return "Materials :";
	default:
		return "";
	}
}

void ChangeFormValues(Form *form, int data[])
{
	int maxValue = 20;
	if (data[0] <= maxValue)
	{
		int villagersIgnored = data[0] % 5;
		
		maxValue = data[0] - ((villagersIgnored == 0) ? 5 : villagersIgnored);
	}

	for (int i = 0; i < form->activeSlidersNumber; i++)
	{
		if (form->activeSlidersNumber == 1 || form->activeSlidersNumber == 4)
		{
			
			SetSliderValues(form, i, 0, i == 0 ? maxValue : data[i], i == 0 ? 5 : 1);
		}
		else
		{
			SetSliderValues(form, i, 0, data[i], 1);
		}
	}
}

void MoveForm(Form* form, sfVector2f newPosition)
{
	sfVector2f moveVector; // the vector used to move the form
	moveVector.x = newPosition.x - (form->formRect.left + form->formRect.width - 0.5f * form->buttons[2].rect.width);
	moveVector.y = newPosition.y - (form->formRect.top + form->buttons[2].rect.height / 2.0f);

	// moves the form
	form->formRect.left += moveVector.x;
	form->formRect.top += moveVector.y;
	newPosition.x = form->formRect.left;
	newPosition.y = form->formRect.top;
	sfSprite_setPosition(form->sprite, newPosition);
	form->illuPosition.x += moveVector.x;
	form->illuPosition.y += moveVector.y;

	// moves the title
	newPosition = sfText_getPosition(form->title);
	newPosition.x += moveVector.x;
	newPosition.y += moveVector.y;
	sfText_setPosition(form->title, newPosition);

	// moves the buttons
	for (int i = 0; i < form->numberOfButtons; i++)
	{
		form->buttons[i].rect.left += moveVector.x;
		form->buttons[i].rect.top += moveVector.y;
		newPosition.x = form->buttons[i].rect.left;
		newPosition.y = form->buttons[i].rect.top;
		ChangeButtonPosition(&form->buttons[i], newPosition);
	}

	// moves the sliders
	for (int i = 0; i < form->activeSlidersNumber; i++)
	{
		// the sliders
		form->sliders[i].rect.left += moveVector.x;
		form->sliders[i].rect.top += moveVector.y;
		newPosition.x = form->sliders[i].rect.left;
		newPosition.y = form->sliders[i].rect.top;
		sfSprite_setPosition(form->sliders[i].sliderSprite, newPosition);

		// the cursors
		form->sliders[i].cursorPosition.y += moveVector.y;
		sfSprite_setPosition(form->sliders[i].cursorSprite, form->sliders[i].cursorPosition);

		// the texts
		newPosition = sfText_getPosition(form->sliders[i].value);
		newPosition.x += moveVector.x;
		newPosition.y += moveVector.y;
		sfText_setPosition(form->sliders[i].value, newPosition);

		// the icon
		newPosition = sfSprite_getPosition(form->sliders[i].icon);
		newPosition.x += moveVector.x;
		newPosition.y += moveVector.y;
		sfSprite_setPosition(form->sliders[i].icon, newPosition);
	}
}

void SetFormIllu(Form* form, sfSprite* illu)
{
	form->illu = illu;
}