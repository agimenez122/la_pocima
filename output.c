#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "output.h"
#include "game.h"

#ifndef SMALL
void putstring(const char *s)
{
	static char tmpbuf[SCREENWIDTH];
	NR size_t len;

	len = strlen(s);

	while (len >= SCREENWIDTH - 1)
	{
		NR const char *p;
		NR char *dest;
		NR const char *lastspace;
		NR size_t spaceleft;
		NR size_t width;

		p = s;
		dest = tmpbuf;
		spaceleft = SCREENWIDTH - 1;

		/* Loop through one line, locating the first newline or last space */
		while (spaceleft && '\n' != *p)
		{
			switch (*p)
			{
				case ' ': lastspace = p;
			}
			*dest = *p;
			++ dest;
			++ p;
			-- spaceleft;
		}

		if ('\n' != *p)
		{
			/* Terminate at last space */
			p = lastspace;
			width = p - s - 1;
			tmpbuf[width + 1] = 0;
		}
		else
		{
			/* Terminate at found newline */
			*dest = 0;
			width = p - s;
		}

		/* Print the text */
		PUTS(tmpbuf);

		/* Advance pointer */
		s = p + 1;

		/* Decrease counter */
		len -= width;
	}

	PUTS(s);
}
#endif

#ifdef SMALL
void myputs(const unsigned char *s)
{
	NR unsigned char c;

	while (c = *s)
	{
		__asm__("lda %v", c);
		__asm__("jsr %w", CHROUT);
		++ s;
	}
	PUTCHAR('\n');
}
#endif
