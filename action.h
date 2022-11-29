#ifndef ACTION_H
#define ACTION_H

unsigned char move(unsigned char verb);
unsigned char inventory(void);

/* Action functions; takes the object as parameter */
unsigned char take(unsigned char object);
unsigned char lift(unsigned char object);
unsigned char read(unsigned char object);
unsigned char talkto(unsigned char object);
unsigned char unlock(unsigned char object);
unsigned char turn(unsigned char object);
unsigned char hit(unsigned char object);
unsigned char kill(void);
unsigned char pour(unsigned char object);
unsigned char drink(unsigned char object);
unsigned char throw(unsigned char object);
unsigned char look(unsigned char object);
unsigned char drop(unsigned char object);
unsigned char help(void);

#ifndef SMALL
unsigned char license(void);
#endif
unsigned char quit(void);

#endif