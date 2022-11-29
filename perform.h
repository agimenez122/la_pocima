#ifndef PERFORM_H
#define PERFORM_H

#define GAME_CONTINUES 0
#define GAME_LOST 1
#define GAME_WON 2

/* Perform action based on a verb.
 * Returns one of the constants above.
 */
unsigned char perform(unsigned char verb);

#endif
