#include "Cards.h"
#include <stdio.h>
#include <stdlib.h>

const KnowledgeCards CardsProto[] = {
	{ MurF, 60, 3, 10 },
	{ BouclierF, 100, 4, 10 },
	{ DoorF, 30, 2, 10 },
	{ Champs, 60, 1, 10 },
	{ Scierie, 60, 1, 10 },
	{ Puit, 60, 1, 10 },
	{ Empty, 0, 0, 0 }
};


int numberCardsEvt;
int numberCardsExp;
int numberCardsKno;

//Accessor of Proto of Cards
KnowledgeCards GetProto(int index)
{
	return(CardsProto[index]);
}

//char* GetKnowledgeCardName();

char* GetExpeditionCardName(ExpeditionResource type)
{
	char* name = NULL;
	switch (type)
	{
	case looseAGuy:
		name = "When we saw the danger, it was too late for Billy...";
		break;
	case SomeFood:
		name = "We have found some food near a cliff\n but we lost some of us...";
		break;
	case Usefull:
		name = "Come back sound and safe !";
		break;
	case SomeFoodAndMaterials:
		name = "It was a good day !";
		break;
	case Doom:
		name = "Nobody came back today...";
		break;
	case SomeFoodAndPeople:
		name = "Hey, we brought some new friends !";
		break;
	case HardlyEarned:
		name = "It was hard but we succeeded, at least most of us.";
		break;
	case NotBad:
		name = "We found those woodcutters,\n and invited them to come here.";
		break;
	case BestCard:
		name = "This expedition was amazing, look what we have found !";
		break;
	case AlmostAllDead:
		name = "This expedition was hard,\n our crew was lost and our mates are almost all dead...";
		break;
	case HardSacrifices:
		name = "This expedition needed hard sacrifices...";
		break;
	case 11:
		name = "Look what we have here !";
		break;
	case 12:
		name = "We came back with nothing...";
		break;
	case 13:
		name = "Everyone Died ...";
		break;
	default:
		break;
	}
	return name;
}
// creates the event cards pile

char* GetKnowledgeCardName(enum Knowledge type)
{
	switch (type)
	{
	case Empty:
		return("Nothing");
	case MurF:
		return("Fortress' Wall");
	case BouclierF:
		return("Fortress' Shield");
	case DoorF:
		return("Fortress' Door");
	case Champs:
		return("Field");
	case Scierie:
		return("Sawmill");
	case Puit:
		return("Well");
	default:
		return("Not Found");
	}
}

EventCard* InitEventCardsPile()
{
	EventCard *eventCards; // the cards pile
	EventCard card;

	eventCards = malloc(EVENT_CARDS_NUMBER * sizeof(EventCard));
	if (eventCards == NULL)
	{
		printf("Error can't alloc this array lign approx 70 Cards.c\n");
		return NULL;
	}

	numberCardsEvt = EVENT_CARDS_NUMBER;

	// creates the card and adds them to the pile
	card = EventCardCreate("hail", 6, 6, 4, 0);
	eventCards[0] = card;
	eventCards[1] = card;

	card = EventCardCreate("fight", 0, 0, 0, 2);
	eventCards[2] = card;
	eventCards[3] = card;

	card = EventCardCreate("blizzard", 6, 10, 0, 0);
	eventCards[4] = card;
	eventCards[5] = card;

	card = EventCardCreate("freeze", 5, 0, 0, 2);
	eventCards[6] = card;
	eventCards[7] = card;

	card = EventCardCreate("rats attack", 10, 0, 0, 0);
	eventCards[8] = card;
	eventCards[9] = card;

	card = EventCardCreate("avalanche", 0, 0, 5, 0);
	eventCards[10] = card;
	eventCards[11] = card;

	return eventCards;
}

// creates a new event card
EventCard EventCardCreate(char* name, int foodLost, int waterLost, int materialsLost, int villagersLost)
{
	EventCard card;

	// init the card properties
	card.name = name;
	card.foodLost = foodLost;
	card.waterLost = waterLost;
	card.materialsLost = materialsLost;
	card.villagersLost = villagersLost;

	return card;
}

// pick a random card from the event cards pile
EventCard PickEventCard(EventCard cardsPile[])
{
	EventCard cardPicked; // the card picked
	EventCard* securityArray = malloc(numberCardsEvt * sizeof(EventCard)); // a security for the cardsPile[] realloc
	int cardPickedIndex; // the index of the card picked
	// the security card returned when there is no card remaining in the pile
	EventCard noCard;
	noCard.name = "noCard";

	// if there is no card remaining
	if (!numberCardsEvt)
	{
		return noCard; // returns the security card
	}

	cardPickedIndex = rand() % numberCardsEvt; // takes a random number
	cardPicked = cardsPile[cardPickedIndex]; // gets the card corresponding to this random number
	numberCardsEvt--; // removes one card from the pile

	for (int i = cardPickedIndex; i < numberCardsEvt; i++)
	{
		cardsPile[i] = cardsPile[i + 1];
	}

	realloc(securityArray, (numberCardsEvt) * sizeof(EventCard)); // try the realloc on the security array
	if (securityArray != NULL) // if the realloc is done with no error
	{
		cardsPile = securityArray; // resize the array
	}

	return cardPicked;
}

// returns the resources lost with an event
int* EventCardEffect(EventCard card)
{
	int *resourcesLost = NULL; // the resources lost (1 : food; 2 water; 3 : materials; 4 : villagers)

	resourcesLost = malloc(4 * sizeof(int));

	resourcesLost[0] = card.foodLost;
	resourcesLost[1] = card.waterLost;
	resourcesLost[2] = card.materialsLost;
	resourcesLost[3] = card.villagersLost;

	return resourcesLost;
}

// creates the knowledge cards pile
KnowledgeCards* InitKnowledgeCardsPile()
{
	KnowledgeCards *knowledgeCards = NULL; // the cards pile

	knowledgeCards = malloc(KNOWLEDGE_CARDS_NUMBER * sizeof(KnowledgeCards));
	if (knowledgeCards == NULL)
	{
		printf("Error can't alloc this array lign approx 177 Cards.c\n");
		return NULL;
	}

	numberCardsKno = KNOWLEDGE_CARDS_NUMBER;
	
	for (int i = 0; i < 3; i++)
	{
		knowledgeCards[i] = GetProto(i); // the fortress upgrades
		knowledgeCards[2 * i + 3] = GetProto(i + 3); // the villages upgrades
		knowledgeCards[2 * i + 4] = GetProto(i + 3); // put twice into the pile for the 2 villages
	}

	for (int i = KNOWLEDGE_CARDS_NUMBER / 2; i < KNOWLEDGE_CARDS_NUMBER; i++)
	{
		knowledgeCards[i] = GetProto(6); // all the empty cards (as many as the number of other cards)
	}

	return knowledgeCards;
}

// picks a random card from the pile
KnowledgeCards PickKnowledgeCard(KnowledgeCards cardsPile[], int index, int *idInArray)
{
	KnowledgeCards noCard = GetProto(6);
	noCard.name = -1;
	
	
	if (index != 0)
	{
		numberCardsKno--;
		return noCard;
	}
	KnowledgeCards cardPicked; // the card picked
	
	KnowledgeCards* securityArray = malloc(numberCardsKno * sizeof(KnowledgeCards)); // a security for the cardsPile[] realloc
	int cardPickedIndex; // the index of the card picked
	// the security card returned when there is no card remaining in the pile

	// if there is no card remaining
	if (!numberCardsKno)
	{
		return noCard; // returns the security card
	}

	cardPickedIndex = rand() % numberCardsKno; // takes a random number

	cardPicked = cardsPile[cardPickedIndex];
	
	*idInArray = cardPickedIndex;
	return cardPicked;
}

void removeCardFromPile(KnowledgeCards cardsPile[], int id)
{
	static int numberOfCardsInPile = KNOWLEDGE_CARDS_NUMBER;
	if (numberOfCardsInPile == 0 || id == -1)
	{
		return;
	}
	numberOfCardsInPile--;
	for (int i = id; i < numberOfCardsInPile; i++)
	{
		cardsPile[i] = cardsPile[i + 1];
	}
	PickKnowledgeCard(NULL, 1, NULL);
}

// creates the expedition cards pile
ExpeditionCards* InitExpeditionCardsPile()
{
	ExpeditionCards *expeditionCards = NULL; // the cards pile
	ExpeditionCards card;
	
	expeditionCards = malloc(EXPEDITION_CARDS_NUMBER * sizeof(ExpeditionCards));
	if (expeditionCards == NULL)
	{
		printf("Error can't alloc this array lign approx 200 Cards.c\n");
		return NULL;
	}

	numberCardsExp = EXPEDITION_CARDS_NUMBER;

	// creates the cards and adds them to the pile
	card = ExpeditionCardCreate(looseAGuy, t1, (int[]) { 0, 0, 0, -1 });
	ExpeditionCardsAddToPile(card, expeditionCards, 0, 4);

	card = ExpeditionCardCreate(SomeFood, t1, (int[]) { 8, 0, 0, -1 });
	ExpeditionCardsAddToPile(card, expeditionCards, 9, 4);

	card = ExpeditionCardCreate(Usefull, t1, (int[]) { 0, 4, 6, 0 });
	ExpeditionCardsAddToPile(card, expeditionCards, 15, 4);

	card = ExpeditionCardCreate(SomeFoodAndMaterials, t1, (int[]) { 8, 0, 2, 0 });
	ExpeditionCardsAddToPile(card, expeditionCards, 22, 5);

	card = ExpeditionCardCreate(Doom, t2, (int[]) { 0, 0, 0, -5 });
	ExpeditionCardsAddToPile(card, expeditionCards, 4, 3);

	card = ExpeditionCardCreate(SomeFoodAndPeople, t2, (int[]) { 8, 0, 0, 2 });
	ExpeditionCardsAddToPile(card, expeditionCards, 13, 2);

	card = ExpeditionCardCreate(HardlyEarned, t2, (int[]) { 0, 0, 8, -2 });
	ExpeditionCardsAddToPile(card, expeditionCards, 19, 3);

	card = ExpeditionCardCreate(NotBad, t2, (int[]) { 0, 0, 6, 2 });
	ExpeditionCardsAddToPile(card, expeditionCards, 33, 4);

	card = ExpeditionCardCreate(BestCard, t3, (int[]) { 15, 15, 15, 3 });
	ExpeditionCardsAddToPile(card, expeditionCards, 7, 2);

	card = ExpeditionCardCreate(AlmostAllDead, t3, (int[]) { 0, 12, 0, -4 });
	ExpeditionCardsAddToPile(card, expeditionCards, 27, 2);

	card = ExpeditionCardCreate(HardSacrifices, t3, (int[]) { 0, 0, 10, -3 });
	ExpeditionCardsAddToPile(card, expeditionCards, 29, 4);

	return expeditionCards;
}

// create a new expedition card
ExpeditionCards ExpeditionCardCreate(ExpeditionResource type, TurnOfExpedition tierOfTurn, int resourcesAdded[4])
{
	ExpeditionCards card;

	// init the card properties
	card.type = type;
	card.tierOfTurn = tierOfTurn;
	card.foodGained = resourcesAdded[0];
	card.waterGained = resourcesAdded[1];
	card.materialGained = resourcesAdded[2];
	card.habitantFluctuation = resourcesAdded[3];

	return card;
}

// adds one or multiple copies of a card to the card pile
void ExpeditionCardsAddToPile(ExpeditionCards card, ExpeditionCards cardsPile[], int placeOfNewCard, int numberOfCopies)
{
	// repeat as many times as the number of copies there are
	for (int i = placeOfNewCard; i < placeOfNewCard + numberOfCopies; i++)
	{
		cardsPile[i] = card; // add the card to the pile
	}
}


// picks a random card from the pile according to the specified turn
ExpeditionCards PickExpeditionCard(ExpeditionCards cardsPile[], int tierOfTurn)
{
	ExpeditionCards cardPicked; // the card picked
	ExpeditionCards* securityArray = malloc(numberCardsExp * sizeof(ExpeditionCards)); // a security for the cardsPile[] realloc
	int* possibleCards = malloc(numberCardsExp * sizeof(int)); // create an array that will contain the cards positions
	int index = 0; // the index used to write on the array
	int cardPickedIndex; // the position of the card picked
	// the security card returned when there is no card corresponding to the tierOfTurn
	ExpeditionCards noCard;
	noCard.type = No_Card;
	noCard.tierOfTurn = tierOfTurn;
	noCard.foodGained = 0;
	noCard.waterGained = 0;
	noCard.materialGained = 0;
	noCard.habitantFluctuation = 0;

	for (int i = 0; i < numberCardsExp; i++) // checks all cards
	{
		if (cardsPile[i].tierOfTurn == tierOfTurn) // if the card has the required turn
		{
			possibleCards[index] = i; // put its position in the array
			index++;
		}
	}

	// if there is no corresponding card
	if (!index)
	{
		return noCard; // returns the security card
	}

	cardPickedIndex = rand() % index; // pick a random card among the possible ones;
	cardPicked = cardsPile[possibleCards[cardPickedIndex]]; // init the card picked values
	numberCardsExp--; // removes one card from the pile

	// shifts all the cards of the pile after the card picked
	for (int i = possibleCards[cardPickedIndex]; i < numberCardsExp; i++)
	{
		cardsPile[i] = cardsPile[i + 1];
	}

	free(possibleCards);
	realloc(securityArray, (numberCardsExp) * sizeof(ExpeditionCards)); // try the realloc on the security array
	if (securityArray != NULL) // if the realloc is done with no error
	{
		cardsPile = securityArray; // resize the array
	}

	return cardPicked;
}

// returns the resources gains of the expedition
int* ExpeditionCardEffect(ExpeditionCards card, int numberOfVillagersInExp)
{
	int *resourcesGain = NULL;// the effects of the expedition (1 : food; 2 : water; 3 : materials; 4 : villagers)
	
	resourcesGain = malloc(4 * sizeof(int));
	
	int multiplier = numberOfVillagersInExp / 5; // the resource multiplier

	multiplier = (multiplier == 0 ? 1 : multiplier); // if there are less than 5 people, the multiplier is set to 1; else it stays the same

	resourcesGain[0] = card.habitantFluctuation * multiplier;
	resourcesGain[1] = card.foodGained * multiplier;
	resourcesGain[2] = card.waterGained * multiplier;
	resourcesGain[3] = card.materialGained * multiplier;

	return resourcesGain;
}