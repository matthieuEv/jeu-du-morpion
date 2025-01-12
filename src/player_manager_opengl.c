/**
 * @file player_manager_opengl.c
 *
 * @date 1 jan 2025
 * @author Guitter Julien
 */

#include "board.h"
#include "board_view.h"
#include <assert.h>
#include <stdio.h>

#include "window.h"

#include <stdbool.h>

#if defined CONFIG_PLAYER_MANAGER_OPENGL

static int cell_x, cell_y = 0;
static bool isClicked = false;

extern PieceType currentPlayer;

void mouse_clicked(double x, double y){
    cell_x = (float)x-((int)x % 200);
    cell_y = (float)y-((int)y%200);
	isClicked = true;
}

void PlayerManager_init (void)
{
	set_mouse_callback(mouse_clicked);
}

void PlayerManager_free (void)
{
}

static bool tryMove (int x, int y)
{
	if(Board_putPiece(x/200, y/200, currentPlayer) == SQUARE_IS_NOT_EMPTY){
		return false;
	}
	BoardView_displaySquare(x/200, y/200, currentPlayer);
	return true;
}

void PlayerManager_oneTurn (void)
{
	if(isClicked){
		if(tryMove(cell_x, cell_y)){
			isClicked = false;
			switch (currentPlayer){
				case CROSS:
					currentPlayer = CIRCLE;
					break;
				case CIRCLE:
					currentPlayer = CROSS;
					break;
				default:
					assert(0);
			}
		}
	}
}

#endif // defined CONFIG_PLAYER_MANAGER_OPENGL
