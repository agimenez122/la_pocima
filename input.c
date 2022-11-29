#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "input.h"
#include "game.h"
#include "output.h"

#ifdef SMALL
# define VERBS 27
#else
# define VERBS 30
#endif

static const struct verb_s
{
	const unsigned char len;
	const char *s;
	unsigned char verbnum;
} verbs[VERBS] =
{

	{ 1, "n", V_N },
	{ 1, "s", V_S },
	{ 1, "e", V_E },
	{ 1, "o", V_W },
	{ 1, "i", V_I },
	{ 5, "tomar", V_TAKE },
	{ 7, "recoger", V_TAKE },
	{ 8, "levantar", V_LIFT },
	{ 4, "leer", V_READ },
	{ 6, "hablar", V_TALKTO },
	{ 11, "desbloquear", V_UNLOCK },
	{ 5, "abrir", V_UNLOCK },
	{ 5, "girar", V_TURN },
	{ 7, "golpear", V_HIT },
	{ 8, "punetazo", V_HIT },
	{ 6, "patear", V_HIT },
	{ 5, "matar", V_KILL },
	{ 6, "verter", V_POUR },
	{ 6, "vaciar", V_POUR },
	{ 5, "beber", V_DRINK },
	{ 6, "lanzar", V_THROW },
	{ 7, "mirar a", V_LOOK },
	{ 5, "mirar", V_LOOK },
	{ 8, "examinar", V_LOOK },
	{ 6, "soltar", V_DROP },
	{ 5, "ayuda", V_HELP },
#ifndef SMALL
	{ 8, "licencia", V_LICENSE },
	{ 8, "licencia", V_LICENSE },
#endif
	{ 6, "cerrar", V_QUIT },
	{ 5, "salir", V_QUIT },
};

#ifdef __C64__
void __fastcall__ getline(void)
{
#define CHRIN 0xFFCF

	__asm__("ldy #$FF");
	__asm__("chrinloop: iny");
	__asm__("jsr %w", CHRIN);
	__asm__("sta %w,y", (unsigned int) input);
	__asm__("cmp #%b", '\n');
	__asm__("bne chrinloop");
	__asm__("lda #0");
	__asm__("sta %w,y", (unsigned int) input);
}
#endif

/* text_input
 *
 * Get some text input from the user.  Pass in the max number of characters
 * to be accepted, and a pointer to a string that is at least as big plus one
 * for the null terminator to hold the string.
 *
 * Returns the number of characters entered if everything went ok
 * and the user pressed return, 0 if the user pressed Escape.
 */

//unsigned char __fastcall__ text_input(unsigned char x, unsigned char y, char
//*text, unsigned char max, unsigned char num_only)
unsigned char __fastcall__ text_input(char *text, unsigned char max, unsigned char num_only)
{
  char c;
  unsigned char i;
  unsigned char x;
  unsigned char y;
 
  //--- Initialize ---//
  i = 0;
  cursor(1);
  //cputsxy(x, y, text);                  // show text
  //gotoxy(x, y); 
  x = wherex();
  y = wherey();
  for(i=0;i<max;i++)
  {
	text[i]='\0';
  }
  i = 0;
  while (1) {
    c = cgetc();
 
    switch(c) {
      case 27:                          // user pressed escape
            text[0] = 27;                   // flag that user pressed escape
            cursor(0);                      // since a blank string and escape 
            return(0);                      // both have a return value of zero
            break;
 
      case 155:                         // user pressed return
            text[strlen(text)] = '\0';
            cursor(0);
            return(i);
            break;
 
      case 126:                         // user pressed backspace 
            if (i != 0) {
              --i;
              if (i == (strlen(text)-1))
                text[i] = '\0';
              else
                text[i] = ' ';
              //cclearxy(x+i, y, 1);
              cputcxy(x+i, y, ' ');
              gotoxy(x+i, y);
            }
            break;
 
      case 30:                          // user pressed left arrow
            if (i != 0) {
              --i;
              gotoxy(x+i, y);
            }
            break;
 
      case 31:                          // user pressed right arrow
            if (i < strlen(text)) {
              ++i;
              gotoxy(x+i, y); 
            }
 
      default:
            if (i == max) continue;                     // maxed out
 
            if (!num_only) {
              if (isprint(c)) {                           // is a printable char
                text[i] = c;
                cputc(c);
                ++i;
                break;
              }
            }
 
            if (isdigit(c)) {                           // is a number
              text[i] = c;
              cputc(c);
              ++i;
              break;
            }
 
            break;
    }
  }
  // end of function
}


unsigned char getinput(void)
{
	while (1)
	{
		NR unsigned char i;
		NR const struct verb_s *verb;

#ifdef __C64__
		PUTS("Tu turno?");
		getline();
#else
		fputs("\nTu turno? ", stdout);
		//fgets(input, 38, stdin);
		text_input(input, 28, 0);
		for(i = 0; i < 29; i++)
		{
			input[i]=tolower(input[i]);
		}

		//printf("%s",input);
#endif
		PUTCHAR('\n');

		verb = verbs;
		for (i = VERBS; i; -- i, ++ verb)
		{
			NR unsigned char n;
			NR char *p;

			n = verb->len; /* Verb length */
			p = input + n; /* First character after verb */
			if (!strncmp(input, verb->s, n) &&
			    (!*p || ' ' == *p))
			{
				if (' ' == *p)
				{
					object_string = p + 1;
				}
				else
				{
					object_string = NULL;
				}
				return verb->verbnum;
			}
		}

#ifdef SMALL
		PUTS("Que?");
#else
		PUTS("No te entiendo");
#endif
	}
}
