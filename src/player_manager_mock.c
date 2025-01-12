/**
 * @file player_manager_mock.c
 *
 * @date 19/12/2024
 * @author matthieuEv
 */

#include "board.h"
#include <assert.h>
#include <stdio.h>

#if defined CONFIG_PLAYER_MANAGER_MOCK

PieceType thisPlayer; // to know who need to play
int i; //variable to browse into test tab

void PlayerManager_init(void)
{
	thisPlayer = CROSS; // Init the first player
	i = 0; //start at 0
}

void PlayerManager_free(void) {}

void PlayerManager_oneTurn(void)
{
  #if defined DEF_CIRCLE_WINS
    printf("Circle Wins test\n");
    //Circle Win test set
    int test_x[6] = {0, 1, 0, 1, 2, 1};//row
    int test_y[6] = {0, 0, 1, 1, 2, 2};
  #endif
  #if defined DEF_CROSS_WINS
    //CROSS Win test set
    printf("Cross Wins test\n");
    int test_x[5] = {0, 1, 0, 1, 0}; //row
    int test_y[5] = {0, 0, 1, 1, 2};
  #endif
  #if defined DEF_DRAW
    //Draw Test set
    printf("Draw test\n");
    int test_x[9] = {0,0,1,2,1,0,2,1,2};//row
    int test_y[9] = {1,0,0,1,1,2,0,2,2};
  #endif
  printf("Player %d\n", thisPlayer);
  printf("x: %d\n", test_x[i]);
  printf("y: %d\n", test_y[i]);
	Board_putPiece(test_x[i], test_y[i], thisPlayer);
  
	switch (thisPlayer) {
		case CROSS:
			thisPlayer = CIRCLE;
			break;
		case CIRCLE:
			thisPlayer = CROSS;
			break;
		default:
			printf("Error during change of player.");
	}
	i++;
}

#endif//  #if defined CONFIG_PLAYER_MANAGER_MOCK