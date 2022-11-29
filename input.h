#ifndef INPUT_H
#define INPUT_H

/* Verb constants as returned from getinput() */
#define V_N 1
#define V_S 2
#define V_E 3
#define V_W 4
#define V_I 5
#define V_TAKE 6
#define V_LIFT 7
#define V_READ 8
#define V_TALKTO 9
#define V_UNLOCK 10
#define V_TURN 11
#define V_HIT 12
#define V_KILL 13
#define V_POUR 14
#define V_DRINK 15
#define V_THROW 16
#define V_LOOK 17
#define V_DROP 18
#define V_LICENSE 19
#define V_QUIT 20
#define V_HELP 21

/* Retrieve input from user and return the verb */
unsigned char getinput(void);

#endif
