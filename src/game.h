/**
 * @file game.h
 *
 * Whole game's management module
 *
 * @date 19/12/2024
 * @author matthieuEv
 */
#if !defined GAME_H_
#define GAME_H_

/**
 * Game constructor.
 */
extern void Game_init (void);

/**
 * Game destructor.
 */
extern void Game_free (void);

/**
 * Starts game loop.
 *
 * Exits as soon as the game has ended. This function can then
 * be called again.
 */
extern void Game_loop (void);

#endif /* !defined GAME_H_ */
