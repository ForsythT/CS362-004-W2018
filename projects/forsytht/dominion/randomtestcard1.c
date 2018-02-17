#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "rngs.h"

// set NOISY_TEST to 0 to include printfs from output
#define NOISY_TEST 1

int checkVillage(struct gameState *testG, struct gameState *G, int player){
	int numFails = 0;

	playCard(0, 0, 0, 0, testG);	
	
	drawCard(player, G); //draw 3 cards
	
	G->numActions--;
	G->numActions += 2;

#if(NOISY_TEST == 0)
	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[whoseTurn(testG)], G->handCount[player]);
#endif		
	if (G->handCount[player] != testG->handCount[whoseTurn(testG)]){
#if(NOISY_TEST == 0)
		printf("HAND COUNT TEST FAILED\n\n");
#endif		
		numFails++;
	}
	else{
#if(NOISY_TEST == 0)
		printf("HAND COUNT TEST PASSED\n\n");
#endif		
	}
#if(NOISY_TEST == 0)
	printf("Deck Count: %d, Expected Deck: %d\n", testG->deckCount[whoseTurn(testG)], G->deckCount[player]);
#endif		
	if (G->deckCount[player] != testG->deckCount[whoseTurn(testG)]){
#if(NOISY_TEST == 0)
		printf("DECK COUNT TEST FAILED\n\n");
#endif		
		numFails++;
	}
	else{
#if(NOISY_TEST == 0)
		printf("DECK COUNT TEST PASSED\n\n");
#endif		
	}
#if(NOISY_TEST == 0)
	printf("Discard Count: %d, Expected Discard: %d\n", testG->discardCount[whoseTurn(testG)], G->discardCount[player]);
#endif		
	if (G->discardCount[player] != testG->discardCount[whoseTurn(testG)]){
#if(NOISY_TEST == 0)
		printf("DISCARD COUNT TEST FAILED\n\n");
#endif		
		numFails++;
	}
	else{
#if(NOISY_TEST == 0)
		printf("DISCARD COUNT TEST PASSED\n\n");
#endif		
	}
#if(NOISY_TEST == 0)
//	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
#endif		
	if (G->numActions != testG->numActions){
#if(NOISY_TEST == 0)
		printf("ACTIONS COUNT TEST FAILED\n\n");
#endif		
		numFails++;
	}
	else{
#if(NOISY_TEST == 0)
		printf("ACTIONS COUNT TEST PASSED\n\n");
#endif		
	}
	
	return numFails;
}

int main() {
	srand(time(NULL));
	int i, n, p, numPass, r;
	int numFails = 0;

	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

	SelectStream(2);
	PutSeed(3);

	struct gameState G, testG;
	for (n = 0; n < 20000; n++) {
		initializeGame(2, k, 1000, &G);
		//for (i = 0; i < sizeof(struct gameState); i++) {
		//	printf("Testing for Seg Fault\n");
		//	((char*)&G)[i] = floor(Random() * 256);
		//}
	//	G.numPlayers = 2;
		p = rand() % 2;
		G.whoseTurn = p;
		G.deckCount[p] = rand() % MAX_DECK;
		G.discardCount[p] = rand() % MAX_DECK;
		G.handCount[p] = rand() % MAX_HAND;
		for(i = 0; i < G.handCount[p]; i++){
			r = rand() % 27;
			G.hand[p][i] = r;
		}
		for(i = 0; i < G.deckCount[p]; i++){
			r = rand() % 27;
			G.deck[p][i] = r;
		}
		for(i = 0; i < G.discardCount[p]; i++){
			r = rand() % 27;
			G.discard[p][i] = r;
		}
		memcpy(&testG, &G, sizeof(struct gameState));
		testG.hand[p][0] = village; // make village the first card in hand
		numFails += checkVillage(&testG, &G, p);
	}

	numPass = ((n+1)*4) - numFails;

	printf("Village Results\nPassed %d Tests\nFailed %d Tests\n", numPass, numFails);

	return 0;
}	
