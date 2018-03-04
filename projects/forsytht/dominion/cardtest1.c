#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int checkAdventurer(struct gameState *testG, struct gameState *G){
	int player = 0;
	int i, gainedTreasures, numCards, numCardsG, cardDrawn;
	int z = 0;
	int drawntreasure = 0;
	int numtreasures = 0;
	int numtreasuresafter = 0;
	int temphand[MAX_HAND];


	for(i = 0; i < testG->handCount[whoseTurn(testG)]; i++){
		if((testG->hand[whoseTurn(testG)][i] == copper) || (testG->hand[whoseTurn(testG)][i] == silver) || (testG->hand[whoseTurn(testG)][i] == gold)){
			numtreasures++;	
		}
	}

	printf("Hello\n");
	playCard(0, 0, 0, 0, testG);
	printf("Hello\n");
	

	while(drawntreasure < 2 && G->deckCount[player] > 0){
		printf("Hello\n");
		drawCard(player, G);
		cardDrawn = G->hand[player][G->handCount[player]-1]; // top card of hand is most recent
		if(cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
			drawntreasure++;
		}
		else{
			printf("Removing card..\n\n");
			temphand[z] = cardDrawn;
			G->handCount[player]--;
			z++;
		}
	}
	while(z-1 >= 0){
		printf("Hello\n");
		G->discard[player][G->discardCount[player]++] = temphand[z-1]; // discard all cards that were drawn
		z = z-1;
	}

	discardCard(0, player, G, 0);
	G->numActions--;
	if(drawntreasure == 3){
		drawntreasure--;
	}

	for(i = 0; i < testG->handCount[whoseTurn(testG)]; i++){
		if((testG->hand[whoseTurn(testG)][i] == copper) || (testG->hand[whoseTurn(testG)][i] == silver) || (testG->hand[whoseTurn(testG)][i] == gold)){
			numtreasuresafter++;	
		}
	}

	gainedTreasures = numtreasuresafter - numtreasures;
	
	printf("Gained Treasures: %d, Expected: %d\n\n", gainedTreasures, drawntreasure);
	if(gainedTreasures == drawntreasure){
		printf("TREASURE TEST PASSED\n\n");
	}
	else{
		printf("TREASURE TEST FAILED\n\n");
	}

	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[whoseTurn(testG)], G->handCount[player]);
	if (G->handCount[player] == testG->handCount[whoseTurn(testG)]){
		printf("HAND COUNT TEST PASSED\n\n");
	}
	else
		printf("HAND COUNT TEST FAILED\n\n");

	numCards = testG->deckCount[whoseTurn(testG)] + testG->discardCount[whoseTurn(testG)];
	numCardsG = G->deckCount[player] + G->discardCount[player];

	printf("Num Cards in Discard/Deck: %d, Expected: %d\n\n", numCards, numCardsG);
	if(numCards == numCardsG){
		printf("OTHER CARD COUNT TEST PASSED\n\n");
	}
	else{
		printf("OTHER CARD COUNT TEST FAILED\n\n");
	}
	
	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
	if (G->numActions == testG->numActions){
		printf("ACTIONS COUNT TEST PASSED\n\n");
	}
	else
		printf("ACTIONS COUNT TEST FAILED\n\n");

	return 0;
}

int main() {
	int seed = 1000;
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;

	initializeGame(numPlayers, k, seed, &testG);

	memcpy(&G, &testG, sizeof(struct gameState));
	
	printf("TESTING Adventurer Card:\n");

	testG.hand[whoseTurn(&testG)][0] = adventurer; // make adventurer the first card in hand

	checkAdventurer(&testG, &G);

	return 0;
}
