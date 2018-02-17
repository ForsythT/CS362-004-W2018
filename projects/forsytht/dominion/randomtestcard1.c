#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int checkVillage(struct gameState *testG, struct gameState *G){
	int player = 0;

	playCard(0, 0, 0, 0, testG);	
	
	drawCard(player, G); //draw 3 cards
	
	discardCard(0, player, G, 0);
	G->numActions--;
	G->numActions += 2;


	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[whoseTurn(testG)], G->handCount[player]);
	if (G->handCount[player] == testG->handCount[whoseTurn(testG)]){
		printf("HAND COUNT TEST PASSED\n\n");
	}
	else
		printf("HAND COUNT TEST FAILED\n\n");
	
	printf("Deck Count: %d, Expected Deck: %d\n", testG->deckCount[whoseTurn(testG)], G->deckCount[player]);
	if (G->deckCount[player] == testG->deckCount[whoseTurn(testG)]){
		printf("DECK COUNT TEST PASSED\n\n");
	}
	else
		printf("DECK COUNT TEST FAILED\n\n");

	printf("Discard Count: %d, Expected Discard: %d\n", testG->discardCount[whoseTurn(testG)], G->discardCount[player]);
	if (G->discardCount[player] == testG->discardCount[whoseTurn(testG)]){
		printf("DISCARD COUNT TEST PASSED\n\n");
	}
	else
		printf("DISCARD COUNT TEST FAILED\n\n");

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
	int numPlayer = 4;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	
	initializeGame(numPlayer, k, seed, &testG);

	memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game

	printf("\nTESTING Village Card:\n\n");

	testG.hand[whoseTurn(&testG)][0] = village; // make smithy the first card in hand

	printf("With 5 Cards on Deck, 0 on Discard\n========================================\n");	
	checkVillage(&testG, &G);

	memset(&testG, 23, sizeof(struct gameState)); // clear the game state
	memset(&G, 23, sizeof(struct gameState)); // clear the game state
	initializeGame(numPlayer, k, seed, &testG); // make a new game
	
	memcpy(testG.discard[whoseTurn(&testG)], testG.deck[whoseTurn(&testG)], sizeof(int) * testG.deckCount[whoseTurn(&testG)]); // move deck into discard

	while(testG.deckCount[whoseTurn(&testG)] > 0){ //empty deck and fill discard
		testG.deckCount[whoseTurn(&testG)]--;
		testG.discardCount[whoseTurn(&testG)]++;
	}

	testG.hand[whoseTurn(&testG)][0] = village; // make smithy the first card in hand

	memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game
	
	printf("With 0 Cards on Deck, 5 on Discard\n========================================\n");
	checkVillage(&testG, &G);

	memset(&testG, 23, sizeof(struct gameState)); // clear the game state
	memset(&G, 23, sizeof(struct gameState)); // clear the game state
	initializeGame(numPlayer, k, seed, &testG); // make a new game

	while(testG.deckCount[whoseTurn(&testG)] > 0){ //empty deck
		testG.deckCount[whoseTurn(&testG)]--;
	}

	testG.hand[whoseTurn(&testG)][0] = village; // make smithy the first card in hand

	memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game
	
