#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_

/** @defgroup game_view game_view
 * @{
 *
 * Functions related to the game view
 */

/**
 * @brief Draws the game (in the current state)
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_game();


/**
 * @brief Updates the time on the screen
 */
void update_time_on_screen();

/**@}*/

#endif /* _GAME_VIEW_H_ */
