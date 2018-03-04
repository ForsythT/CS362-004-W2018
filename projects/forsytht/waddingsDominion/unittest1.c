// MUCH OF THIS CODE IS NOT MINE, it is largely the code posted on Canvas. 
// Initially, I was only going to just use it for reference, but it already accomplishes most everything I was
// going to test for...
// I did, however, make some changes such as increasing the lower bound of the hand size to 0 
// (to account for having no treasure cards) as well as increase the max handsize to MAX_HAND rather than 5 and num players to 4 (max).
// I also added an extra test case for each iteration that just checks a random combination of treasure cards.
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
	int i, rNum;
	int seed = 1000;
	int numPlayer = 4;
	int maxBonus = 10;
	int p, handCount;
	int bonus;
	int coins;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int maxHandCount = MAX_HAND;
	// arrays of all coppers, silvers, and golds
	int coppers[MAX_HAND];
	int silvers[MAX_HAND];
	int golds[MAX_HAND];
	for (i = 0; i < MAX_HAND; i++){
		coppers[i] = copper;
		silvers[i] = silver;
		golds[i] = gold;
	}

	printf ("TESTING updateCoins():\n\n");
	for (p = 0; p < numPlayer; p++){
		for (handCount = 0; handCount <= maxHandCount; handCount++){ // check for num treasures being from 0 to MAX
			for (bonus = 0; bonus <= maxBonus; bonus++){
#if (NOISY_TEST == 1)
				printf("Test player %d with %d treasure card(s) and %d bonus.\n", p, handCount, bonus);
#endif
				memset(&G, 23, sizeof(struct gameState));   // clear the game state
				initializeGame(numPlayer, k, seed, &G); // initialize a new game
				G.handCount[p] = handCount;                 // set the number of cards on hand
				memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
				updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
				printf("Copper:\nG.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);
#endif
				assert(G.coins == handCount * 1 + bonus); // check if the number of coins is correct

				memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
				updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
				printf("Silver:\nG.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);
#endif
				assert(G.coins == handCount * 2 + bonus); // check if the number of coins is correct

				memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
				updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
				printf("Gold:\nG.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);
#endif
				assert(G.coins == handCount * 3 + bonus); // check if the number of coins is correct
				// Set the cards to random treasure cards
				for (i = 0; i < handCount; i++){
					rNum = rand() % 3;
					if(rNum == 0){
						G.hand[p][i] = copper;
					}
					else if(rNum == 1){
						G.hand[p][i] = silver;
					}
					else{
						G.hand[p][i] = gold;
					}
				}

				updateCoins(p,&G, bonus);

				coins = 0;
					
				// What we expect from the function
				for (i = 0; i < handCount; i++){
					if (G.hand[p][i] == copper){
						coins += 1;
					}
					else if (G.hand[p][i] == silver){
						coins += 2;
					}
					else if (G.hand[p][i] == gold){
						coins += 3;
					}
				}
				
				// add the bonus
				coins += bonus;
#if (NOISY_TEST == 1)	
				printf("Random:\nG.coins = %d, expected = %d\n", G.coins, coins);
#endif				
				assert(G.coins == coins); // check if the number of coins is correct
			}
		}
	}

	printf("All tests passed!\n");

	return 0;
}
