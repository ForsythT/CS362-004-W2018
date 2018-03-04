#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
	srand(time(NULL));
	int i;
	int p, who;
	int seed = 1000;
	int numPlayers = 4;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G;
	
	initializeGame(numPlayers, k, seed, &G);

	printf("TESTING whoseTurn():\n");

	for (i = 0; i < 100; i++){ // iterate it 100 times
		p = rand() % 4; // get a random player
		G.whoseTurn = p; // make it their turn
		
		who = whoseTurn(&G);
#if (NOISY_TEST == 1)
		printf("Whose Turn: %d, Expected: %d\n", who, p);
#endif
		assert(who == p);	
	}

	printf("All tests passed!\n");

	return 0;
}
