#ifndef _FORM_H
#define _FORM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "../Button/Buttons.h"
#include "../../GraphicsUtilities/GraphicsUtilities.h"
#include "../Text/Text.h"

// the resource to send
typedef enum RESOURCE
{
	villager,
	food,
	water,
	materials
}Resource;

// is it allowed to send several types of resources or does the player have to choose one
typedef enum FORMTYPE
{
	severalResources, // several types of resources
	uniqueResource // only one type
}FormType;

// the sliders used to choose how many resources are sent
typedef struct SLIDER
{
	Resource type; // the type of resource controlled by the slider
	sfSprite* sliderSprite;
	sfSprite* cursorSprite;
	sfSprite* icon;
	sfFloatRect rect; // the slider's hit-box
	sfVector2f cursorPosition;
	sfText* value; // the value currently pointed with the slider
	int minValue;
	int maxValue;
	int differenceBetweenValues; // the step between each value taken by the slider
	int currentValue;
}Slider;

// the struct containing all the info of the form
typedef struct FORM
{
	FormType type; // the type of form (several resources at the same time or only one)
	bool isActive; // false : disables the form; true : enables it
	Button buttons[3]; // the cancel and confirm buttons
	sfSprite* sprite;
	sfSprite* illu;
	sfFloatRect formRect; // the form's position and size
	sfVector2f illuPosition;
	sfText* title; // the form's main title
	sfFont* font; // the font of all the form's texts (including the sliders belonging to the form)
	Slider sliders[4]; // the different sliders of the form
	int activeSlidersNumber; // the number of sliders of the form
	int numberOfButtons; // the number of buttons of the form
	bool isCanceled;
}Form;

// creates a form
Form InitForm(char* title, sfVector2f position, char* pathOfSpriteToLoad, int numberOfSliders, FormType type);

void SetFormTitle(Form* form, char* newTitle);

// Reset Slider Type
void SetSliderType(Form* form, int PlaceOfSliderToInit, Resource type);

// sets or resets the values of the slider
void SetSliderValues(Form* form, int numberOfSliderToSetTheValues, int minValue, int maxValue, int differenceBetweenValues);

// enables or disables the form
void SwitchFormState(Form* form);

// manages all inputs for the form
void ManageForms(sfRenderWindow* window, Form forms[], int numberOfFormsToManage, sfVector2i mousePosition);

void InputToForms(Form *form, int value);

// updates the cursor position and the texts for each slider of the active form
void UpdateForms(Form forms[], int numberOfFormsToManage);

// displays the form that is active
void DisplayForms(sfRenderWindow* window, Form forms[], int numberOfForms);

// manages the action of the form when the player confirms their action
int* CommitFormAction(Form* form);

// associates a value with a string (0 : villagers; 1 : food; 2 : water; 3 : materials)
char* IndexToType(int value);

// set the values of all the form's sliders
void ChangeFormValues(Form *form, int data[]);

// moves the form
void MoveForm(Form* form, sfVector2f newPosition);

void SetFormIllu(Form* form, sfSprite* illu);

#endif // !_FORM_H