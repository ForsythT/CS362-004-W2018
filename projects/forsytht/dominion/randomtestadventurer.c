#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int checkAdventurer(struct gameState *testG, struct gameState *G, int player){
	int i, gainedTreasures, numCards, numCardsG, cardDrawn;
	int numFails = 0;
	int z = 0;
	int drawntreasure = 0;
	int numtreasures = 0;
	int numtreasuresafter = 0;
	int temphand[MAX_HAND];

	printf("Testing Adventurer\n");
	// Counting treasure cards in hand of player in test game state
	for(i = 0; i < testG->handCount[whoseTurn(testG)]; i++){
		if((testG->hand[player][i] == copper) || (testG->hand[player][i] == silver) || (testG->hand[player][i] == gold)){
			numtreasures++;	
		}
	}

	playCard(0, 0, 0, 0, testG);
	
	// This is reenacting what we expect the adventurer card to do with an identical game state.
	while(drawntreasure < 2 && (G->deckCount[player]+G->discardCount[player] > 0)){
		drawCard(player, G);
		cardDrawn = G->hand[player][G->handCount[player]-1]; // top card of hand is most recent
		if(cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
			drawntreasure++;
		}
		else{
			//printf("Removing card..\n\n");
			temphand[z] = cardDrawn; // puts drawn card in temp hand
			G->handCount[player]--; // removes it from player hand
			z++;
		}
	}

	while(z-1 >= 0){
		G->discard[player][G->discardCount[player]++] = temphand[z-1]; // discard all cards that were drawn
		z = z-1;
	}

	discardCard(0, player, G, 0);
	G->numActions--;

	if(drawntreasure == 3){
		drawntreasure--;
	}

	for(i = 0; i < testG->handCount[player]; i++){
		if((testG->hand[player][i] == copper) || (testG->hand[player][i] == silver) || (testG->hand[player][i] == gold)){
			numtreasuresafter++;	
		}
	}

	gainedTreasures = numtreasuresafter - numtreasures;
	
//	printf("Gained Treasures: %d, Expected: %d\n\n", gainedTreasures, drawntreasure);
	if(gainedTreasures != drawntreasure){
//		printf("TREASURE TEST FAILED\n\n");
		numFails++;
	}
	else{
//		printf("TREASURE TEST PASSED\n\n");
	}

//	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[player], G->handCount[player]);
	if (G->handCount[player] != testG->handCount[player]){
//		printf("HAND COUNT TEST FAILED\n\n");
		numFails++;
	}
	else{
//		printf("HAND COUNT TEST PASSED\n\n");
	}

	numCards = testG->deckCount[player] + testG->discardCount[player];
	numCardsG = G->deckCount[player] + G->discardCount[player];

//	printf("Num Cards in Discard/Deck: %d, Expected: %d\n\n", numCards, numCardsG);
	if(numCards != numCardsG){
	//	printf("OTHER CARD COUNT TEST FAILED\n\n");
		numFails++;
	}
	else{
	//	printf("OTHER CARD COUNT TEST PASSED\n\n");
	}
	
//	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
	if (G->numActions != testG->numActions){
	//	printf("ACTIONS COUNT TEST FAILED\n\n");
		numFails++;
	}
	else{
	//	printf("ACTIONS COUNT TEST PASSED\n\n");
	}

	return numFails;
}

int main() {
	srand(time(NULL));
	int i, n, p, numPass;
	int numFails = 0;
	
	struct gameState G, testG;
	for (n = 0; n < 2000; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
	//		printf("Testing for Seg Fault\n");
			((char*)&G)[i] = rand() % 256;
		}
		p = rand() % 2;
		G.whoseTurn = p;
		G.deckCount[p] = rand() % MAX_DECK;
		G.discardCount[p] = rand() % MAX_DECK;
		G.handCount[p] = rand() % MAX_HAND;
		memcpy(&testG, &G, sizeof(struct gameState));
		testG.hand[p][0] = adventurer; // make adventurer the first card in hand
		numFails += checkAdventurer(&testG, &G, p);
	}

	numPass = ((n+1)*4) - numFails;

	printf("Adventurer Results\nPassed %d Tests\nFailed %d Tests\n", numPass, numFails);

	return 0;
}
