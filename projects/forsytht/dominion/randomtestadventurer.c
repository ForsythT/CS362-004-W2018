#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
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

	// Counting treasure cards in hand of player in test game state
	for(i = 0; i < testG->handCount[whoseTurn(testG)]; i++){
		if((testG->hand[player][i] == copper) || (testG->hand[player][i] == silver) || (testG->hand[player][i] == gold)){
			numtreasures++;	
		}
	}

	playCard(0, 0, 0, 0, testG);
	
	// This is reenacting what we expect the adventurer card to do with an identical game state.
	while(drawntreasure < 2){
		if(G->deckCount[player] > 0){
			drawCard(player, G);
			cardDrawn = G->hand[player][G->handCount[player]-1]; // top card of hand is most recent
			if(cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
				drawntreasure++;
			}
			else{
		//		printf("Removing card..\n\n");
				temphand[z] = cardDrawn; // puts drawn card in temp hand
				G->handCount[player]--; // removes it from player hand
				z++;
			}
		}
		else if(G->deckCount[player] == 0 && G->discardCount[player] > 0){
			memcpy(G.deck[player], testG->deck[player], sizeof(int) * G.discardCount[player]);
			memcpy(G.discard[player], testG->discard[player], sizeof(int) * G.discardCount[player]);
			pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
			pre.handCount[p]++;
			pre.deckCount[p] = pre.discardCount[p]-1;
			pre.discardCount[p] = 0;
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
	if(gainedTreasures == drawntreasure){
		continue;
//		printf("TREASURE TEST PASSED\n\n");
	}
	else{
//		printf("TREASURE TEST FAILED\n\n");
		numFails++;
	}

//	printf("Hand Count: %d, Expected Hand: %d\n", testG->handCount[player], G->handCount[player]);
	if (G->handCount[player] == testG->handCount[player]){
		continue;
//		printf("HAND COUNT TEST PASSED\n\n");
	}
	else{
//		printf("HAND COUNT TEST FAILED\n\n");
		numFails++;
	}

	numCards = testG->deckCount[player] + testG->discardCount[player];
	numCardsG = G->deckCount[player] + G->discardCount[player];

//	printf("Num Cards in Discard/Deck: %d, Expected: %d\n\n", numCards, numCardsG);
	if(numCards == numCardsG){
		continue;
	//	printf("OTHER CARD COUNT TEST PASSED\n\n");
	}
	else{
	//	printf("OTHER CARD COUNT TEST FAILED\n\n");
		numFails++;
	}
	
//	printf("Action Count: %d, Expected Action: %d\n", testG->numActions, G->numActions);
	if (G->numActions == testG->numActions){
		continue;
	//	printf("ACTIONS COUNT TEST PASSED\n\n");
	}
	else{
	//	printf("ACTIONS COUNT TEST FAILED\n\n");
		numFails++;
	}

	return numFails;
}

int main() {
	int seed = 1000;
	int i, n, p, numPass;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	int numFails = 0;
	
	struct gameState G, testG;

	SelectStream(2);
	PutSeed(3);

	for (n = 0; n < 2000; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);
		}
		p = floor(Random() * 2);
		G.deckCount[p] = floor(Random() * MAX_DECK);
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);
		memcpy(&testG, &G, sizeof(struct gameState));
		testG.hand[whoseTurn(&testG)][0] = adventurer; // make adventurer the first card in hand
		numFails += checkAdventurer(&testG, &G, p);
	}

	numPass = ((n+1)*4) - numFails;

	printf("Adventurer Results\nPassed %d Tests\nFailed %d Tests\n", numPass, numFails);

	return 0;
}
