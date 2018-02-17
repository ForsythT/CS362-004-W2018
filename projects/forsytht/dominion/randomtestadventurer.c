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

	playCard(0, 0, 0, 0, testG);
	

	while(drawntreasure < 2 && G->deckCount[player] > 0){
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
		G->discard[player][G->discardCount[player]++] = temphand[z-1]; // discard all cards that were drawn
		z = z-1;
	}

	discardCard(0, player, G, 0);
	G->numActions--;
	if(drawntreasure == 3){
		drawntreasure--;
