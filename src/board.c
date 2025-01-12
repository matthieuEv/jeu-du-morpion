#include "board.h"
#include <assert.h>

PieceType boardSquares[3][3]; //declare the board
SquareChangeCallback boardOnSquareChange; //declare OnSquareChange callback
EndOfGameCallback boardOnEndOfGame;//declare EndOfGame Callback
GameResult boardGameResult; //declare the gameresult variable

/**
 * Check if the game has to be ended. Only alignment from the last
 * modified square are checked.
 *
 * @param [in] boardSquares the board to check
 * @param [in] lastChangeX the colum where the last piece has been put
 * @param [in] lastChangeY the row where the last piece has been put
 * @param [out] gameResult the winning status if the game has ended (value is not set if
 * the game is not finished)
 *
 * @pre lastChangeX must be in [0..2]
 * @pre lastChangeY must be in [0..2]
 * @pre gameResult must point on an allocated GameResult variable
 *
 * @return a boolean that tells if the game is finished
 */
static bool isGameFinished (const PieceType boardSquares[3][3], Coordinate lastChangeX, Coordinate lastChangeY, GameResult *gameResult)
{
  assert (lastChangeX >= 0 && lastChangeX <= 2);
  assert (lastChangeY >= 0 && lastChangeY <= 2);

  // Vérification de la ligne
  if (boardSquares[lastChangeY][0] == boardSquares[lastChangeY][1] && boardSquares[lastChangeY][1] == boardSquares[lastChangeY][2] && boardSquares[lastChangeY][0] != NONE)
  {
    *gameResult = (boardSquares[lastChangeY][0] == CROSS) ? CROSS_WINS : CIRCLE_WINS;
    return true;
  }

  // Vérification de la colonne
  if (boardSquares[0][lastChangeX] == boardSquares[1][lastChangeX] && boardSquares[1][lastChangeX] == boardSquares[2][lastChangeX] && boardSquares[0][lastChangeX] != NONE)
  {
    *gameResult = (boardSquares[0][lastChangeX] == CROSS) ? CROSS_WINS : CIRCLE_WINS;
    return true;
  }

  // Vérification de la première diagonale
  if (lastChangeX == lastChangeY)
  {
    if (boardSquares[0][0] == boardSquares[1][1] && boardSquares[1][1] == boardSquares[2][2] && boardSquares[0][0] != NONE)
    {
      *gameResult = (boardSquares[0][0] == CROSS) ? CROSS_WINS : CIRCLE_WINS;
      return true;
    }
  }

  // Vérification de la seconde diagonale
  if (lastChangeX + lastChangeY == 2)
  {
    if (boardSquares[0][2] == boardSquares[1][1] && boardSquares[1][1] == boardSquares[2][0] && boardSquares[1][1] != NONE)
    {
      *gameResult = (boardSquares[1][1] == CROSS) ? CROSS_WINS : CIRCLE_WINS;
      return true;
    }
  }

  // Vérification du match nul
  bool isDraw = true;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (boardSquares[i][j] == NONE) {
        isDraw = false;
        break;
      }
    }
    if (!isDraw) break;
  }

  if (isDraw) {
    *gameResult = DRAW;
    return true;
  }

  return false;
}

void Board_init (SquareChangeCallback onSquareChange, EndOfGameCallback onEndOfGame){
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
      boardSquares[i][j] = NONE; 
    }
	}
	boardOnSquareChange = onSquareChange;
	boardOnEndOfGame = onEndOfGame;
}

void Board_free (){
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) { 
      boardSquares[i][j] = NONE; 
    }
  }
}

PutPieceResult Board_putPiece (Coordinate x, Coordinate y, PieceType kindOfPiece){
	PutPieceResult result;

	if (Board_getSquareContent(x, y) == NONE) { //check if the square is empty
		boardSquares[y][x] = kindOfPiece; // Put the piece
		boardOnSquareChange(x, y, kindOfPiece);//Call OnSquareChange callback
		//Check if the game is win
		if (isGameFinished(boardSquares, x, y, &boardGameResult))
			boardOnEndOfGame(boardGameResult);//Call EndOfGame callback
		result = PIECE_IN_PLACE; //save that the piece is in place
	} else {
		result = SQUARE_IS_NOT_EMPTY;
	}
	return result;
}

PieceType Board_getSquareContent (Coordinate x, Coordinate y)
{
	return boardSquares[y][x];
}