#ifndef POTION_H
#define POTION_H

/* Global data types */
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

/* Constants */
#define OBJECTS 21
#define INVENTORY 2
#define MAPSIZE 30

#define EXIT_NORTH 0x01
#define EXIT_SOUTH 0x02
#define EXIT_WEST  0x04
#define EXIT_EAST  0x08

/* Structure describing the game data */
struct gamedata_s
{
	unsigned char room; /* 1 based */
	signed char inventory[INVENTORY];
	signed char objects[OBJECTS];
	BOOL pilleaten;
	BOOL seenscientist;
	unsigned char map[MAPSIZE];
	unsigned char roadvisit;
	unsigned char bothbottles;
};

/* Global game data variable */
#ifdef __C64__
# define gamedata ((struct gamedata_s *) 0xC000)
#else
extern struct gamedata_s *gamedata;
#endif

/* String input */
#ifdef __C64__
# define input ((char *) (0xC000 + sizeof (struct gamedata_s)))
#else
extern char *input;
#endif

/* Pointer to the part of the string that describes the object */
#ifdef __C64__
# define object_string (*((char **) 0xCFFE))
#else
extern const char *object_string;
#endif

/* Speedup variables in non-reentrant functions */
#ifdef __C64__
# define NR static
#else
# define NR
#endif

/* Width of the screen to use when displaying text */
#ifdef __C64__
# define SCREENWIDTH 40
#else
# define SCREENWIDTH 80
#endif

#endif
