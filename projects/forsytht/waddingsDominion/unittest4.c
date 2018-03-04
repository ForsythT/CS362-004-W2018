#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
	int p, hand, num, turn;
	int seed = 1000;
	int numPlayers = 4;
	int playerHands[4];
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G;
	
	initializeGame(numPlayers, k, seed, &G);

	printf("TESTING numHandCards():\n");

	for (p = 0; p < numPlayers; p++){
		for (hand = 0; hand < 50; hand++){
			G.handCount[p] = hand;
			playerHands[p] = hand;
			for (turn = 0; turn < numPlayers; turn++){
				G.whoseTurn = turn;
				num = numHandCards(&G);
#if (NOISY_TEST == 1)
				printf("Player Going: %d, Num Cards: %d, Expected Num Cards: %d\n", turn, num, playerHands[turn]);
#endif
				assert(num == playerHands[turn]);
			}
		}
	}

	printf("All tests passed!\n");

	return 0;
}
