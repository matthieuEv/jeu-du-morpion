/**
 * @file board_view_opengl.c
 *
 * @date 1 jan 2025
 * @author Guitter Julien
 */

#include "board_view.h"
#include <assert.h>
#include <stdio.h>

#include "window.h"

#include "tictactoe_errors.h"

#if defined CONFIG_OPENGLUI


#define RESOURCES_PATH      "./src/resources/"
#define WIN_SIZE            600

unsigned int background, cross, circle;
extern PieceType boardSquares[3][3];

// void render(){
//     draw_image(background, 0, 0);
//     // Afficher les pièces sur le plateau
//     for(int y = 0; y < 3; y++) {
//         for(int x = 0; x < 3; x++) {
// 			if(boardSquares[y][x] == CROSS){
// 				draw_image(cross, x*200, y*200);
// 			}else if(boardSquares[y][x] == CIRCLE){
// 				draw_image(circle, x*200, y*200);
// 			}
//         }
//     }
// }

void BoardView_init (void)
{
	init_window(WIN_SIZE, WIN_SIZE, false, "Board");

	background = add_image(RESOURCES_PATH "background.png", WIN_SIZE, WIN_SIZE);
	cross = add_image(RESOURCES_PATH "cross.png", WIN_SIZE/3, WIN_SIZE/3);
	circle = add_image(RESOURCES_PATH "cricle.png", WIN_SIZE/3, WIN_SIZE/3);

	draw_image(background, 0, 0);
}

void BoardView_free (void)
{
    free_window();
}

void BoardView_displayAll (void)
{
	render_window();
}

void BoardView_displaySquare (Coordinate x, Coordinate y, PieceType kindOfPiece)
{
	if(kindOfPiece == CROSS){
		draw_image(cross, x*200, y*200);
	}else if(kindOfPiece == CIRCLE){
		draw_image(circle, x*200, y*200);
	}
}

void BoardView_displayEndOfGame (GameResult result)
{
	if(result == CROSS_WINS){
		printf("CROSS WINS\n");
	}else if(result == CIRCLE_WINS){
		printf("CIRCLE WINS\n");
	}else{
		printf("DRAW\n");
	}
}

void BoardView_displayPlayersTurn (PieceType thisPlayer)
{
}

#endif // defined CONFIG_OPENGLUI
