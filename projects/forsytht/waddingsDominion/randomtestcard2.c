#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "rngs.h"

// set NOISY_TEST to 0 to include printfs from output
#define NOISY_TEST 1

int checkSeaHag(struct gameState *testG, struct gameState *G, int player){
	int numFails = 0;
	int check = 0;
	int i;

	playCard(0, 0, 0, 0, testG);	
	
	for(i = 1; i < G->numPlayers; i++){ // starting at 1 to not give the user a curse..
		G->discard[i][G->discardCount[i]] = G->deck[i][G->deckCount[i]-1];
		G->deckCount[i]--;
		G->discardCount[i]++;
		G->deck[i][G->deckCount[i]-1] = curse; // top deck a curse
		G->supplyCount[curse]--;
	}
	i--;
	if(G->deckCount[i] > 0){
#if(NOISY_TEST == 0)
		printf("Top Deck: %d, Curse: %d\n\n", G->deck[i][G->deckCount[i]-1], curse);
#endif
	}

	discardCard(0, player, G, 0);
	G->numActions--;
#if(NOISY_TEST == 0)
	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[player], G->handCount[player]);
#endif
	if (G->handCount[player] == testG->handCount[player]){
#if(NOISY_TEST == 0)
		printf("HAND COUNT TEST PASSED\n\n");
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("HAND COUNT TEST FAILED\n\n");
#endif
	}
	
#if(NOISY_TEST == 0)
	printf("Deck Count: %d, Expected Deck: %d\n", testG->deckCount[player], G->deckCount[player]);
#endif
	if (G->deckCount[player] == testG->deckCount[player]){
#if(NOISY_TEST == 0)
		printf("DECK COUNT TEST PASSED\n\n");
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("DECK COUNT TEST FAILED\n\n");
#endif
	}

	for(i = 0; i < G->numPlayers; i++){
#if(NOISY_TEST == 0)
		printf("Discard Count: %d, Expected Discard: %d\n", testG->discardCount[i], G->discardCount[i]);
#endif
		if (G->discardCount[i] == testG->discardCount[i]){
#if(NOISY_TEST == 0)
			printf("DISCARD COUNT TEST FOR PLAYER %d PASSED\n\n", i);
#endif
		}
		else{
			numFails++;
#if(NOISY_TEST == 0)
			printf("DISCARD COUNT TEST FOR PLAYER %d FAILED\n\n", i);
#endif
			check = 1;
		}
	}
	if(check == 0){
#if(NOISY_TEST == 0)
		printf("DISCARD TEST PASSED\n\n");	
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("DISCARD TEST FAILED\n\n");
#endif
	}

#if(NOISY_TEST == 0)
	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
#endif
	if (G->numActions == testG->numActions){
#if(NOISY_TEST == 0)
		printf("ACTIONS COUNT TEST PASSED\n\n");
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("ACTIONS COUNT TEST FAILED\n\n");
#endif
	}

#if(NOISY_TEST == 0)
	printf("Checking Top Deck of Players...\n\n");
#endif
	
	check = 0;

	for(i = 0; i < G->numPlayers; i++){
		if(testG->deckCount[i] > 0){
#if(NOISY_TEST == 0)
			printf("Top Deck of Player %d: %d, Expected: %d\n\n", i, testG->deck[i][testG->deckCount[i]-1], G->deck[i][G->deckCount[i]-1]);
#endif
			if(i == whoseTurn(testG)){
				if(testG->deck[i][testG->deckCount[i]-1] == curse){
					numFails++;
#if(NOISY_TEST == 0)
					printf("TOP DECK OF CURRENT PLAYER IS A CURSE, TEST FAILED\n\n");
#endif
					check = 1;
				}
				else{
#if(NOISY_TEST == 0)
					printf("TOP DECK OF CURRENT PLAYER IS NOT A CURSE, TEST PASSED\n\n");
#endif
				}
			}
			else{
				if(testG->deck[i][testG->deckCount[i]-1] == curse){
#if(NOISY_TEST == 0)
					printf("TOP DECK OF PLAYER %d IS A CURSE, TEST PASSED\n\n", i);
#endif
				}
				else{
					numFails++;
#if(NOISY_TEST == 0)
					printf("TOP DECK OF PLAYER %d IS NOT A CURSE, TEST FAILED\n\n", i);
#endif
					check = 1;
				}	
			}
		}
		else if(i == whoseTurn(testG)){	
#if(NOISY_TEST == 0)
			printf("TOP DECK OF CURRENT PLAYER IS NOT A CURSE, TEST PASSED\n\n");
#endif
		}
		else{
			numFails++;
#if(NOISY_TEST == 0)
			printf("TOP DECK OF PLAYER %d IS NOT A CURSE, TEST FAILED\n\n", i);
#endif
		}
	}
	if(check == 0){
#if(NOISY_TEST == 0)
		printf("CURSE TEST PASSED\n\n");	
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("CURSE TEST FAILED\n\n");
#endif
	}
	
#if(NOISY_TEST == 0)
	printf("Curse Supply Count: %d, Expected Supply: %d\n\n", testG->supplyCount[curse], G->supplyCount[curse]);
#endif
	if(testG->supplyCount[curse] == G->supplyCount[curse]){
#if(NOISY_TEST == 0)
		printf("CURSE COUNT TEST PASSED\n\n");
#endif
	}
	else{
		numFails++;
#if(NOISY_TEST == 0)
		printf("CURSE COUNT TEST FAILED\n\n");
#endif
	}

	return numFails;
}

int main() {
	srand(time(NULL));
	int i, n, p, r, numP;
	int numPass = 0;
	int numFails = 0;

	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

	SelectStream(2);
	PutSeed(3);

	struct gameState G, testG;
	for (n = 0; n < 20000; n++) {
		numP = rand() % 3 + 2;
		initializeGame(numP, k, 1000, &G);
		//for (i = 0; i < sizeof(struct gameState); i++) {
		//	printf("Testing for Seg Fault\n");
		//	((char*)&G)[i] = floor(Random() * 256);
		//}
	//	G.numPlayers = ;
		p = rand() % numP;
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
		testG.hand[p][0] = sea_hag; // make village the first card in hand
		numFails += checkSeaHag(&testG, &G, p);
	}

	numPass = ((n+1)*(5+(numP*2))) - numFails;

	printf("Sea Hag Results\nPassed %d Tests\nFailed %d Tests\n", numPass, numFails);

	return 0;
}	
