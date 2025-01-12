/**
 * @file board_view.h
 *
 * Tic tac toe's board visualisation module
 *
 * @date 19/12/2024
 * @author matthieuEv
 */

#if !defined BOARD_VIEW_H_
#define BOARD_VIEW_H_

#include "board.h"

typedef enum
{
    CROSS_COLOR = 40,
    CIRCLE_COLOR = 9
} Color;

/**
 * Constructor of the board view.
 *
 * Must be called before any other function of this module.
 */
extern void BoardView_init (void);

/**
 * Destructor of the board view.
 *
 * Must be called after using any function of this module to free
 * related resources.
 */
extern void BoardView_free (void);

/**
 * Displays the entire board (including background) at current status.
 */
extern void BoardView_displayAll (void);

/**
 * Display the specified content into the specified square.
 *
 * @param [in] x logical column coordinate
 * @param [in] y logical row coordinate
 * @param [in] kindOfPiece the content to display (can be NONE if empty)
 *
 * @pre x must be in [0..2]
 * @pre y must be in [0..2]
 */
extern void BoardView_displaySquare (Coordinate x, Coordinate y, PieceType kindOfPiece);

/**
 * Displays a message to the user at the end of the game.
 *
 * @param [in] result winning status
 */
extern void BoardView_displayEndOfGame (GameResult result);

/**
 * Displays who has to play.
 *
 * @param [in] thisPlayer the player who has to play, designated by its piece type
 *
 * @pre thisPlayer cannot be NONE
 */
extern void BoardView_displayPlayersTurn (PieceType thisPlayer);

/**
 * Displays a message when a move has been refused because the square is not empty
 */
extern void BoardView_sayCannotPutPiece (void);


/**
 * Clears the console.
 */
void clear_console();

/**
 * set the color of the text
 * 
 * @param string the string to print
 * @param color the color to set
 * @param bool foreground if true set the foreground color else set the background color
 */
void color(char *string, Color color, bool foreground);


#endif /* !defined BOARD_VIEW_H_ */
