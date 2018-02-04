#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int main() {
	srand(time(NULL));
	int i;
	int seed = 1000;
	int numPlayer = 2; // only need 1 player for testing this, so will only use p == 0
	int p, value, cardPos, flag;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	// arrays of all coppers, silvers, and golds
	initializeGame(numPlayer, k, seed, &testG); // initialize a new game
	memcpy(&G, &testG, sizeof(struct gameState));

	p = 0;

	printf("TESTING gainCard():\n\n");

	for (cardPos = 0; cardPos < 10; cardPos++){ // this will enumerate through all kingdom cards in the supplu
		for (i = 0; i < 12; i++){ // this will deplete each card in the supply
			flag = rand() % 3; // randomly get a flag
			value = gainCard(k[cardPos], &testG, flag, p);
			if (k[cardPos] == gardens || k[cardPos] == great_hall){
				if (i >= 8){
					if (value == -1){
#if (NOISY_TEST == 1)
						printf("Successfully returns supply empty for card %d\n", k[cardPos]);
#endif
					}
				}
				else{
					if (flag == 0){ // for flag 0, should go to discard
						G.discard[p][G.discardCount[p]] = k[cardPos];
						G.discardCount[p]++;
						G.supplyCount[k[cardPos]]--;
					}
					else if (flag == 1){ // for flag 1, should go to deck
						G.deck[p][G.deckCount[p]] = k[cardPos];
						G.deckCount[p]++;
						G.supplyCount[k[cardPos]]--;
					}
					else if (flag == 2){ // for flag 2, should go to hand
						G.hand[p][G.handCount[p]] = k[cardPos];
						G.handCount[p]++;
						G.supplyCount[k[cardPos]]--;
					}
				
				}
			}
			else if (i >= 10){ // if supply pile is empty
				if (value == -1) {
#if (NOISY_TEST == 1)					
					printf("Successfully returns supply empty for card %d\n\n", k[cardPos]);
#endif
				}	
			}
			else if (flag == 0){ // for flag 0, should go to discard
				G.discard[p][G.discardCount[p]] = k[cardPos];
				G.discardCount[p]++;
				G.supplyCount[k[cardPos]]--;
			}
			else if (flag == 1){ // for flag 1, should go to deck
				G.deck[p][G.deckCount[p]] = k[cardPos];
				G.deckCount[p]++;
				G.supplyCount[k[cardPos]]--;
			}
			else if (flag == 2){ // for flag 2, should go to hand
				G.hand[p][G.handCount[p]] = k[cardPos];
				G.handCount[p]++;
				G.supplyCount[k[cardPos]]--;
			}
			
#if (NOISY_TEST == 1)					
			printf("Flag: %d, CardPos: %d, Card Supply: %d, Hand Size: %d, Deck Size: %d, Discard Size: %d\nExpected Supply: %d, Expected Hand Size: %d, Expected Deck Size: %d, Expected Discard Size: %d\n\n", flag, k[cardPos], testG.supplyCount[k[cardPos]], testG.handCount[p], testG.deckCount[p], testG.discardCount[p], G.supplyCount[k[cardPos]], G.handCount[p], G.deckCount[p], G.discardCount[p]);		
#endif				
			assert(memcmp(&G, &testG, sizeof(struct gameState)) == 0);

	/*				assert(G.discardCount[p] == testG.discardCount[p]);
					for(j = 0; j < testG.discardCount[p]; j++){	
						assert(G.discard[p][j] == testG.discard[p][j]);
					}

					assert(G.handCount[p] == testG.handCount[p]);
					for(j = 0; j < testG.handCount[p]; j++){	
						assert(G.hand[p][j] == testG.hand[p][j]);
					}

					assert(G.deckCount[p] == testG.deckCount[p]);
					for(j = 0; j < testG.deckCount[p]; j++){	
						assert(G.deck[p][j] == testG.deck[p][j]);
					}*/
		}
	}
	printf("All tests passed!\n");

	return 0;
}
