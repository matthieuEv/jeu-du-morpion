#include "board_view.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if defined CONFIG_TEXTUI

extern PieceType boardSquares[3][3];

void BoardView_init (void)
{
  clear_console();
  BoardView_displayAll();
}

void BoardView_free (void)
{
}

void BoardView_displayAll (void)
{
  #ifndef CONFIG_PLAYER_MANAGER_MOCK
    clear_console();
  #endif

    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 3; x++) {
        if(boardSquares[y][x] == CROSS){
          color("X ",CROSS_COLOR, 1);
        }else if(boardSquares[y][x] == CIRCLE){
          color("O ",CIRCLE_COLOR, 1);
        }else{
          printf(". ");
        }
      }
      printf("\n");//line break
    }
    printf("\n\n");//line break
}

void BoardView_displaySquare (Coordinate x, Coordinate y, PieceType kindOfPiece)
{
	BoardView_displayAll();
}

void BoardView_displayEndOfGame (GameResult result){
  #ifndef CONFIG_PLAYER_MANAGER_MOCK//If we don't use the mock
	  clear_console();                      // clear the console
  #endif
  
	BoardView_displayAll();//display the board
	bool is_draw = false;
	if (result == CROSS_WINS) {// Circle wins
    color(" CROSS ",CROSS_COLOR, 0);
		printf(" Wins\n");
	} else if (result == CIRCLE_WINS) {// If cross wins
    color(" CIRCLE ",CIRCLE_COLOR, 0);
		printf(" Wins\n");
	} else {
		is_draw = true;//save that is a draw
		printf(" _____  _____       __          __\n"
 "|  __ \\|  __ \\     /\\ \\        / /\n"
 "| |  | | |__) |   /  \\ \\  /\\  / / \n"
 "| |  | |  _  /   / /\\ \\ \\/  \\/ /  \n"
 "| |__| | | \\ \\  / ____ \\  /\\  /   \n"
 "|_____/|_|  \\_\\/_/    \\_\\/  \\/    \n");
	}
	if (!is_draw) {//if it's not a draw

		printf(" __          _______ _   _ \n"
" \\ \\        / /_   _| \\ | |\n"
"  \\ \\  /\\  / /  | | |  \\| |\n"
"   \\ \\/  \\/ /   | | | . ` |\n"
"    \\  /\\  /   _| |_| |\\  |\n"
"     \\/  \\/   |_____|_| \\_|\n");
	}
}

void BoardView_displayPlayersTurn (PieceType thisPlayer){
	switch (thisPlayer) {
		case CROSS: //if it's the turn of cross player
			printf("It's cross turn \n");
			break;
		case CIRCLE://if it's the turn of circle player
			printf("It's circle turn \n");
			break;
    default:
      break;
	}
}

void BoardView_sayCannotPutPiece (void)
{
  printf("Square occupied try again.\n");
}

void color(char *string, Color color, bool foreground){
  if(foreground){
    printf("\033[38;5;%dm%s\033[0m", color, string);
  }else{
    printf("\033[48;5;%dm%s\033[0m", color, string);
  }
}

void clear_console(){
  #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    // Check OS
    system("clear");// send the command clear
  #endif

  #if defined(_WIN32) || defined(_WIN64)
    system("cls");//send the command cls
  #endif
}
#endif // defined CONFIG_TEXTUI
