/**
 * @file player_manager_scanf.c
 *
 * @date 7 oct. 2016
 * @author jilias
 */

#include "board.h"
#include "board_view.h"
#include <assert.h>
#include <stdio.h>

#if defined CONFIG_PLAYER_MANAGER_SCANF

extern PieceType currentPlayer;

void PlayerManager_init (void)
{
}

void PlayerManager_free (void)
{
}

void PlayerManager_oneTurn (void)
{
  int x, y;
  while (1) {
    switch (currentPlayer)
    {
    case CROSS:
      color(" CROSS ",CROSS_COLOR, 0);
      break;
    case CIRCLE:
      color(" CIRCLE ",CIRCLE_COLOR, 0);
      break;
    default:
      break;
    }
    printf(" Entrez les coordonnées de la case (X,Y) : \n> ");

    if (scanf("%d,%d", &x, &y) != 2) {
      printf("Format invalide. Veuillez entrer sous la forme X,Y.\n\n");
      // Vider le tampon d'entrée
      int c; while ((c = getchar()) != '\n' && c != EOF);
      continue;
    }

    if (x < 0 || x > 2 || y < 0 || y > 2) {
      printf("Les coordonnées doivent être entre 0 et 2.\n\n");
      continue;
    }

    if (Board_putPiece(x, y, currentPlayer) == SQUARE_IS_NOT_EMPTY) {
      printf("La case est déjà occupée.\n\n");
      continue;
    }

    switch (currentPlayer) {
      case CROSS:
        currentPlayer = CIRCLE;
        break;
      case CIRCLE:
        currentPlayer = CROSS;
        break;
      default:
        assert(0);
    }

    break;
  }
}

#endif // defined CONFIG_PLAYER_MANAGER_SCANF
