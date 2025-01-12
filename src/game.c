/**
 * @file main.c
 *
 * @date 19/12/2024
 * @author matthieuEv
 */

#include "player_manager.h"
#include "board_view.h"
#include "board.h"
#include "game.h"
#include <stdio.h>

bool TemoinFinPartie;
PieceType currentPlayer;
PieceType boardSquares[3][3];

void Game_SquareChangeCallback(Coordinate x, Coordinate y, PieceType kindOfPiece)
{
	BoardView_displaySquare(x, y, kindOfPiece);
}

void Game_EndOfGameCallback(GameResult result)
{
	TemoinFinPartie = true;
	BoardView_displayEndOfGame(result);
}

void Game_init(void)
{
	TemoinFinPartie = false;
	currentPlayer = CROSS;
	Board_init(Game_SquareChangeCallback, Game_EndOfGameCallback);
	BoardView_init();
	PlayerManager_init();
}

void Game_free(void)
{
	BoardView_free();
	PlayerManager_free();
	Board_free();
}

void Game_loop (void)
{
  #if defined CONFIG_PLAYER_MANAGER_OPENGL
	while(!windowShouldClose() && !TemoinFinPartie){
		BoardView_displayAll();
		PlayerManager_oneTurn();
	}
  #else
	while(!TemoinFinPartie){
		PlayerManager_oneTurn();
	}
  #endif
}