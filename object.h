#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"

/* Print the selected object to screen. */
void printobject(unsigned char);

#define O_STONE_PILL 0
#define O_STONE_1 1
#define O_STONE_2 2
#define O_STONE_3 3
#define O_STONE_MISSION 4
#define O_STONE_5 5
#define O_STONE_6 6
#define O_STONE_7 7
#define O_STONE_8 8
#define O_LAST_STONE O_STONE_8
#define O_PILL 9
#define O_SQUIRREL 10
#define O_COIN 11
#define O_COTTAGE 12
#define O_SWITCH 13
#define O_HUBCAP 14
#define O_KEY 15
#define O_SCIENTIST 16
#define O_ELIXIR 17
#define O_BOOK 18
#define O_DOOR 19
#define O_COKE 20

/* Retrieve object number from a word.
 * Checks the current room and (optionally) inventory.
 * Returns -1 if object was not found.
 */
signed char objectfromword(const char *, BOOL checkinventory);

/* Remove object from game map and add to inventory. */
BOOL addinventory(unsigned char object);

/* Remove object from inventory and add to game map. */
BOOL dropinventory(unsigned char object);

/* Check if an object is movable. */
BOOL movable(unsigned char object);

#endif
