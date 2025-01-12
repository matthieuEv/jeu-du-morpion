/**
 * @file main.c
 *
 * @date 19/12/2024
 * @author matthieuEv
 */

#include "game.h"

int main (void)
{
	Game_init ();

	Game_loop ();

	Game_free ();

	return 0;
}
