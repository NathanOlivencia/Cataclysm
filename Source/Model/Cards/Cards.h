#ifndef _CARDS_H
#define _CARDS_H

// the number of cards in the piles
#define EVENT_CARDS_NUMBER 12
#define KNOWLEDGE_CARDS_NUMBER 18
#define EXPEDITION_CARDS_NUMBER 37

// the events picked at the end of the turn
typedef struct
{
	char* name;
	int foodLost;
	int waterLost;
	int materialsLost;
	int villagersLost;
}EventCard;

enum Knowledge
{
	Empty = -1,
	MurF = 0,
	BouclierF = 1,
	DoorF = 2,
	Champs = 3,
	Scierie = 4,
	Puit = 5
};

typedef struct
{
	enum Knowledge name;
	int materialsToBuild;
	int numberOfTurnToBuild;
	int habitantsToFullBuildPotential;
} KnowledgeCards;

typedef enum
{
	t0,
	t1,
	t2,
	t3
}TurnOfExpedition;

typedef enum ExpeditionResource
{
	No_Card = -1,
	looseAGuy,
	SomeFood,
	Usefull,
	SomeFoodAndMaterials,
	Doom,
	SomeFoodAndPeople,
	HardlyEarned,
	NotBad,
	BestCard,
	AlmostAllDead,
	HardSacrifices,
}ExpeditionResource;

//First piece need to add proto values of different cards of different tier of expeditions
typedef struct
{
	TurnOfExpedition tierOfTurn;
	ExpeditionResource type;
	int foodGained;
	int waterGained;
	int materialGained;
	int habitantFluctuation;
} ExpeditionCards;

KnowledgeCards GetProto(int index);

typedef enum
{
	LaMeuf,
	LeMec,
	LeSerpent
}God;

typedef struct 
{
	God godID;
	int habitantStart;
	int foodStart;
	int waterStart;
	int materialsStart;
	char* enumToString;
}GodCards;

typedef struct
{
	KnowledgeCards *knowledgeCardsPile;
	ExpeditionCards *expeditionCardsPile;
}ExpAndKnowPilePack;

char* GetKnowledgeCardName(enum Knowledge type);

char* GetExpeditionCardName(ExpeditionResource type);

// creates the event cards pile
EventCard* InitEventCardsPile();

// creates a new event card
EventCard EventCardCreate(char* name, int foodLost, int waterLost, int materialsLost, int villagersLost);

// picks a random card from the pile
EventCard PickEventCard(EventCard cardsPile[]);

// returns the resources lost with an event
int* EventCardEffect(EventCard card);

// creates the knowledge cards pile
KnowledgeCards* InitKnowledgeCardsPile();

// picks a random card from the pile
KnowledgeCards PickKnowledgeCard(KnowledgeCards cardsPile[], int index, int *idInArray);

//Do things
void removeCardFromPile(KnowledgeCards cardsPile[], int id);

// creates the expedition cards pile
ExpeditionCards* InitExpeditionCardsPile();

// create a new expedition card
ExpeditionCards ExpeditionCardCreate(ExpeditionResource type, TurnOfExpedition tierOfTurn, int resourcesAdded[4]);

// adds one or multiple copies of a card to the card pile
void ExpeditionCardsAddToPile(ExpeditionCards card, ExpeditionCards cardsPile[], int placeOfNewCard, int numberOfCopies);

// picks a random card from the pile according to the specified turn
ExpeditionCards PickExpeditionCard(ExpeditionCards cardsPile[], int tierOfTurn);

// returns the resources gains of the expedition
int* ExpeditionCardEffect(ExpeditionCards card, int numberOfVillagersInExp);

#endif