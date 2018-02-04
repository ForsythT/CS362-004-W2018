#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int checkSeaHag(struct gameState *testG, struct gameState *G, int numPlayers){
	int player = 0;
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
		printf("Top Deck: %d, Curse: %d\n\n", G->deck[i][G->deckCount[i]-1], curse);
	}

	discardCard(0, player, G, 0);
	G->numActions--;

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

	for(i = 0; i < numPlayers; i++){
		printf("Discard Count: %d, Expected Discard: %d\n", testG->discardCount[i], G->discardCount[i]);
		if (G->discardCount[i] == testG->discardCount[i]){
			printf("DISCARD COUNT TEST FOR PLAYER %d PASSED\n\n", i);
		}
		else{
			printf("DISCARD COUNT TEST FOR PLAYER %d FAILED\n\n", i);
			check = 1;
		}
	}
	if(check == 0){
		printf("DISCARD TEST PASSED\n\n");	
	}
	else{
		printf("DISCARD TEST FAILED\n\n");
	}

	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
	if (G->numActions == testG->numActions){
		printf("ACTIONS COUNT TEST PASSED\n\n");
	}
	else
		printf("ACTIONS COUNT TEST FAILED\n\n");

	printf("Checking Top Deck of Players...\n\n");
	
	check = 0;

	for(i = 0; i < numPlayers; i++){
		if(testG->deckCount[i] > 0){
			printf("Top Deck of Player %d: %d, Expected: %d\n\n", i, testG->deck[i][testG->deckCount[i]-1], G->deck[i][G->deckCount[i]-1]);
			if(i == whoseTurn(testG)){
				if(testG->deck[i][testG->deckCount[i]-1] == curse){
					printf("TOP DECK OF CURRENT PLAYER IS A CURSE, TEST FAILED\n\n");
					check = 1;
				}
				else{
					printf("TOP DECK OF CURRENT PLAYER IS NOT A CURSE, TEST PASSED\n\n");
				}
			}
			else{
				if(testG->deck[i][testG->deckCount[i]-1] == curse){
					printf("TOP DECK OF PLAYER %d IS A CURSE, TEST PASSED\n\n", i);
				}
				else{
					printf("TOP DECK OF PLAYER %d IS NOT A CURSE, TEST FAILED\n\n", i);
					check = 1;
				}	
			}
		}
		else if(i == whoseTurn(testG)){	
			printf("TOP DECK OF CURRENT PLAYER IS NOT A CURSE, TEST PASSED\n\n");
		}
		else{
			printf("TOP DECK OF PLAYER %d IS NOT A CURSE, TEST FAILED\n\n", i);
		}
	}
	if(check == 0){
		printf("CURSE TEST PASSED\n\n");	
	}
	else{
		printf("CURSE TEST FAILED\n\n");
	}
	
	printf("Curse Supply Count: %d, Expected Supply: %d\n\n", testG->supplyCount[curse], G->supplyCount[curse]);
	if(testG->supplyCount[curse] == G->supplyCount[curse]){
		printf("CURSE COUNT TEST PASSED\n\n");
	}
	else{
		printf("CURSE COUNT TEST FAILED\n\n");
	}

	return 0;
}

int main() {
	int seed = 1000;
	int numPlayers;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	
	printf("\nTESTING Sea Hag Card:\n\n");
	
	for(numPlayers = 2; numPlayers <= 4; numPlayers++){
		printf("TESTING WITH %d PLAYERS\n\n", numPlayers);
		
		memset(&testG, 23, sizeof(struct gameState)); // clear the game state
		memset(&G, 23, sizeof(struct gameState)); // clear the game state
		initializeGame(numPlayers, k, seed, &testG);

		memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game

		testG.hand[whoseTurn(&testG)][0] = sea_hag; // make sea hag the first card in hand

		printf("With 5 Cards on Deck, 0 on Discard\n========================================\n");	
		checkSeaHag(&testG, &G, numPlayers);

		memset(&testG, 23, sizeof(struct gameState)); // clear the game state
		memset(&G, 23, sizeof(struct gameState)); // clear the game state
		initializeGame(numPlayers, k, seed, &testG); // make a new game
	
		memcpy(testG.discard[whoseTurn(&testG)], testG.deck[whoseTurn(&testG)], sizeof(int) * testG.deckCount[whoseTurn(&testG)]); // move deck into discard

		while(testG.deckCount[whoseTurn(&testG)] > 0){ //empty deck and fill discard
			testG.deckCount[whoseTurn(&testG)]--;
			testG.discardCount[whoseTurn(&testG)]++;
		}

		testG.hand[whoseTurn(&testG)][0] = sea_hag; // make sea hag the first card in hand
	
		memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game
		
		printf("With 0 Cards on Deck, 5 on Discard\n========================================\n");
		checkSeaHag(&testG, &G, numPlayers);
	
		memset(&testG, 23, sizeof(struct gameState)); // clear the game state
		memset(&G, 23, sizeof(struct gameState)); // clear the game state
		initializeGame(numPlayers, k, seed, &testG); // make a new game
	
		while(testG.deckCount[whoseTurn(&testG)] > 0){ //empty deck
			testG.deckCount[whoseTurn(&testG)]--;
		}
	
		testG.hand[whoseTurn(&testG)][0] = sea_hag; // make sea hag the first card in hand
	
		memcpy(&G, &testG, sizeof(struct gameState)); // create a copy of the game
		
		printf("With 0 Cards on Deck, 0 on Discard\n========================================\n");
		checkSeaHag(&testG, &G, numPlayers);
	}

	return 0;
}
