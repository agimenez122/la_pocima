#include <stdio.h>

#include "action.h"
#include "object.h"
#include "perform.h"
#include "input.h"
#include "game.h"
#include "output.h"

void cannotdothat(void)
{
	PUTS("No puedes hacer eso!");
}

void nothing(void)
{
	PUTS("Nada.");
}

unsigned char move(unsigned char verb)
{
	unsigned char bitmap;
	signed char offset;

	/* Seen scientist? */
	if (gamedata->seenscientist)
	{
		PUTS("El cientifico te captura.");
		return GAME_LOST;
	}

	/* FIXME: Crossing autobahn? */

	/* Check which movement is requested */
	switch (verb)
	{
		case V_N: bitmap = EXIT_NORTH; offset = -6; break;
		case V_S: bitmap = EXIT_SOUTH; offset =  6; break;
		case V_E: bitmap = EXIT_EAST;  offset =  1; break;
		case V_W: bitmap = EXIT_WEST;  offset = -1; break;
	}

	/* Check if movement is allowed in that direction */
	if (gamedata->map[gamedata->room - 1] & bitmap)
	{
		gamedata->room += offset;
	}
	else
	{
		PUTS("No puedes ir por ese camino.");
		return GAME_CONTINUES;
	}

	/* Check if we are carrying both bottles */
#ifdef SMALL
	__asm__("lda %w", (unsigned int) gamedata->inventory);
	__asm__("ldx %w", (unsigned int) &gamedata->inventory[1]);
	__asm__("cmp #%b", (unsigned char) O_ELIXIR);
	__asm__("bne checktwo");
	__asm__("cpx #%b", (unsigned char) O_COKE);
	__asm__("beq haveboth");
	__asm__("checktwo: cmp #%b", (unsigned char) O_COKE);
	__asm__("bne notboth");
	__asm__("cpx #%b", (unsigned char) O_ELIXIR);
	__asm__("bne notboth");
	__asm__("haveboth:");
#else
	if ((O_ELIXIR == gamedata->inventory[0] && O_COKE == gamedata->inventory[1])
	    ||
	    (O_ELIXIR == gamedata->inventory[1] && O_COKE == gamedata->inventory[0]))
#endif
	{
		switch (++ gamedata->bothbottles)
		{
		case 1:
#ifdef SMALL
			PUTS("La pocion se esta calentando.");
#else
			putstring("La pocion esta empezando a calentarse.");
#endif
			break;

		case 5:
#ifdef SMALL
			PUTS("The potion is reacting with the Coke.");
#else
			putstring("Algun tipo de reaccion parece estar ocurriendo"
					  "entre la pocion y lo que queda de la Coca-Cola.");
#endif
			break;

		case 10:
#ifdef SMALL
			PUTS("Las botellas estan a punto de explotar!");
#else
			putstring("Sera mejor que te deshagas de una de las botellas"
					  "antes de que explote la pocion!");
#endif
			break;

		case 12:
#ifdef SMALL
			PUTS("La pocion te explota en la cara!");
#else
			putstring("La pocion te explota en la cara!");
#endif
			return GAME_LOST;
		}
	}
#ifdef SMALL
	__asm__("jmp skip");
	__asm__("notboth:");
#else
	else
#endif
	{
		/* Reset counter */
		gamedata->bothbottles = 0;
	}
#ifdef SMALL
	__asm__("skip:");
#endif

	return GAME_CONTINUES;
}

unsigned char inventory(void)
{
	NR BOOL hasanything;
#ifndef SMALL
	NR unsigned char i;
	NR signed char *inventory;
#endif

	hasanything = FALSE;

#ifdef SMALL
	PUTS("Inventario:");

	if (gamedata->inventory[0] != -1)
	{
		printobject(gamedata->inventory[0]);
		hasanything = TRUE;
	}
	if (gamedata->inventory[1] != -1)
	{
		printobject(gamedata->inventory[1]);
		hasanything = TRUE;
	}
#else
	inventory = gamedata->inventory;

	PUTS("Tu llevas:");
	for (i = INVENTORY; i; -- i, ++ inventory)
		if (*inventory != -1)
		{
			printobject(*inventory);
			hasanything = TRUE;
		}
#endif

	if (!hasanything)
		nothing();

	return GAME_CONTINUES;
}

unsigned char take(unsigned char object)
{
	switch (object)
	{
		case O_PILL:
			/* Something special happens when we take the pill */
			gamedata->pilleaten = TRUE;
			gamedata->objects[O_PILL] = -1;
			PUTS("Tu entiendes.");
			break;

		case O_STONE_PILL:
			/* Finds pill, if it's not taken already */
			if (!gamedata->pilleaten && -1 == gamedata->objects[O_PILL])
			{
#ifdef SMALL
				PUTS("Encuentras una pastilla.");
#else
				PUTS("Encontraste una pequena pastilla debajo.");
#endif
				gamedata->objects[O_PILL] = gamedata->room;
				break;
			}
			/* Else fall through */

		default:
			if (movable(object))
			{
				if (addinventory(object))
					PUTS("Tomado.");
			}
			else
			{
				cannotdothat();
			}
			break;
	}
	return GAME_CONTINUES;
}

unsigned char lift(unsigned char object)
{
	switch (object)
	{
		case O_STONE_PILL:
			if (!gamedata->pilleaten)
			{
				take(object);
				break;
			}
			/* else fall through */

		default:
			nothing();
			break;
	}

	return GAME_CONTINUES;
}

unsigned char read(unsigned char object)
{
	switch (object)
	{
		case O_STONE_MISSION:
#ifdef SMALL
			PUTS("\"Salvanos del malvado cientifico. \n"
			     " Mezcla su droga maligna con agua salada.\"");
#else
			putstring("Dice:\n"
					  "\"Salvanos del malvado cientifico. \n"
				      "Mezcla su droga maligna con agua salada."
					  "Esa es tu mision.\"");
#endif
			break;

		default:
			nothing();
	}

	return GAME_CONTINUES;
}

unsigned char talkto(unsigned char object)
{
	switch (object)
	{
		case O_SCIENTIST:
#ifdef SMALL
			PUTS("\"¡Moriras con los demas!\"");
#else
			putstring("The scientist says:\n"
			          "\"¡Moriras con los demas!\" "
			          "y se rie freneticamente.\n");
#endif
			break;

		case O_SQUIRREL:
			if (gamedata->pilleaten)
			{
#ifdef SMALL
				PUTS("\"Ya puedes salir del bosque\"");
#else
				putstring("La ardilla dice:\n"
						  "\"Ya puedes salir del bosque\"");
#endif
				gamedata->map[3 - 1] |= EXIT_EAST;
				break;
			}
			/* else fall through */

		default:			
			cannotdothat();
	}

	return GAME_CONTINUES;
}

unsigned char unlock(unsigned char object)
{
	/* Check if we have the key */
	NR BOOL havekey;
#ifndef SMALL
	NR unsigned char i;
#endif

	havekey = FALSE;

#ifdef SMALL
	__asm__("lda %w", (unsigned int) gamedata->inventory);
	__asm__("cmp #%b", (unsigned char) O_KEY);
	__asm__("bne checktwo");
	++ havekey;
	__asm__("checktwo: lda %w", (unsigned int) &gamedata->inventory[1]);
	__asm__("cmp #%b", (unsigned char) O_KEY);
	__asm__("bne out");
	++ havekey;
	__asm__("out:");
#else
	for (i = 0; i < INVENTORY; ++ i)
		if (O_KEY == gamedata->inventory[i])
			havekey = TRUE;
#endif

	if (havekey)
	{
		switch (object)
		{
			case O_COTTAGE:
#ifdef SMALL
				PUTS("Encuentras un interruptor.");
#else
				PUTS("Encuentras un interruptor dentro.");
#endif
				gamedata->objects[O_SWITCH] = gamedata->room;
				return GAME_CONTINUES;
			
			case O_DOOR:
				PUTS("Se abre revelando un laboratorio.");
				gamedata->map[20 - 1] |= EXIT_WEST;
				return GAME_CONTINUES;
		}
	}

	cannotdothat();
	return GAME_CONTINUES;
}

unsigned char turn(unsigned char object)
{
	switch (object)
	{
		case O_SWITCH:
			PUTS("Ruido sordo.");
			gamedata->map[22 - 1] |= EXIT_WEST;
			break;

		default:
			cannotdothat();
			break;
	}

	return GAME_CONTINUES;
}

unsigned char hit(unsigned char object)
{
	switch (object)
	{
		case O_SQUIRREL:
			/* If we hit the squirrel, we kill it. */
			PUTS("Matas a la ardilla.");
			return GAME_LOST;

		case O_SCIENTIST:
			/* Clear the seen scientist flag */
			gamedata->seenscientist = FALSE;
			/* Remove scientist from game */
			gamedata->objects[O_SCIENTIST] = -1;

			PUTS("No te volvera a molestar.");
			break;

		default:
			cannotdothat();
			break;
	}

	return GAME_CONTINUES;
}

unsigned char kill(void)
{
#ifdef SMALL
	PUTS("Este es un juego familiar.");
#else
	putstring("Violencia no, por favor. Este es un juego familiar.");
#endif
	return GAME_CONTINUES;
}

unsigned char pour(unsigned char object)
{
	switch (object)
	{
		case O_ELIXIR:
			/* The game object is to mix the potion with salt
			 * water. This can only be done by pouring it into
			 * the sea.
			 */
			 switch (gamedata->room)
			 {
			 	case 6:
#ifdef SMALL
					PUTS("Lo neutralizaste.");
#else
					PUTS("La pocion ha sido neutralizada.");
#endif
					return GAME_WON;

				default:
#ifdef SMALL
					PUTS("Envenena la tierra.");
#else
					PUTS("La pocion envenena la tierra.");
#endif
					return GAME_LOST;
			}
			break;

		case O_COKE:
#ifdef SMALL
			PUTS("Esta vacio");
#else
			PUTS("La botella esta vacia");
#endif
			break;

		default:
			cannotdothat();
			break;
	}

	return GAME_CONTINUES;
}

unsigned char drink(unsigned char object)
{
	switch (object)
	{
		case O_ELIXIR:
			/* Can only drink the elixir, and we lose if we do it */
			PUTS("Mueres al instante.");
			return GAME_LOST;

		case O_COKE:
			/* It's empty */
			return pour(O_COKE);
			break;

		default:
			cannotdothat();
			return GAME_CONTINUES;
	}
}

unsigned char throw(unsigned char object)
{
	if ((object <= O_LAST_STONE || O_COKE == object) &&
	    9 == gamedata->room)
	{
		return hit(O_SQUIRREL);
	}
	else
	{
		return drop(object);
	}
}

unsigned char look(unsigned char object)
{
	switch (object)
	{
		case O_STONE_MISSION:
			return read(object);

		case O_STONE_PILL:
			if (!gamedata->pilleaten && -1 == gamedata->objects[O_PILL])
				return take(object);
			/* Else fall through */

		default:
			nothing();
			return GAME_CONTINUES;
	}
}

unsigned char drop(unsigned char object)
{
	switch (object)
	{
		case O_COKE:
			/* Can only recycle the Cola bottle once we've taken it */
			switch (gamedata->room)
			{
			case 12:
				if (dropinventory(O_COKE))
				{
#ifdef SMALL
					PUTS("Gracias por reciclar!");
#else
					putstring("Reciclas la botella. Gracias por ser"
							  "amable con la naturaleza!");
#endif
					gamedata->objects[O_COKE] = -1; /* Remove from game */
					break;
				}
				/* else fall through */

			case 28: case 29: /* Dump */
#ifndef SMALL
				putstring("Tirar una botella perfectamente reciclable?"
						  "De ninguna manera!");
				break;
#endif
				/* Fall through for small build */

			case 1:  case 2:  case 3:
			case 7:  case 8:  case 9:
			case 13: case 14: case 15:
			case 30: /* Forest */
#ifndef SMALL
				PUTS("Alguien podria salir lastimado!");
				break;
#endif
				/* Fall through for small build */

			default:
#ifdef SMALL
				PUTS("Porfavor recicla");
#else
				PUTS("Porfavor recicla la botella.");
#endif
				break;
			}
			break;

		case O_ELIXIR:
			if (gamedata->bothbottles)
			{
#ifdef SMALL
				PUTS("No te atreves.");
#else
				putstring("No te atrevas a dejar caer la pocion en su actual"
						  "estado!");
#endif
				break;
			}
			/* else fall through */

		default:
			if (dropinventory(object))
				PUTS("Caido");
	}

	return GAME_CONTINUES;
}

#ifndef SMALL
unsigned char license(void)
{
	putstring("Este programa es software libre; puedes redistribuirlo"
              "y/o modificarlo bajo los terminos de GNU General Public"
              "Licencia publicada por la Free Software Foundation,"
              "version 2.\n\n"
              "Este programa se distribuye con la esperanza de que le sea"
              "util, pero SIN NINGUNA GARANTiA; sin siquiera lo implicito"
              "garantia de COMERCIABILIDAD o IDONEIDAD PARA UN PARTICULAR"
              "PROPoSITO. Consulte la Licencia publica general de GNU para obtener mas informacion"
              "detalles."
#ifndef __C64__
              "\n\n"
              "Deberia haber recibido una copia del GNU General Public"
              "Licencia junto con este programa; si no, escribe al Libre"
              "Software Foundation, Inc., 59 Temple Place, Suite 330,"
              "Boston, MA 02111-1307 EE. UU."
#endif
              );

	return GAME_CONTINUES;
}
#endif

unsigned char help(void)
{
	putstring("Tu puedes hacer:\n"	
		"n s e o i tomar recoger levantar leer hablar desbloquear abrir girar "
		"golpear punetazo patear matar verter vaciar beber lanzar mirar examinar soltar licencia salir\n"
    );

	return GAME_CONTINUES;
}

unsigned char quit(void)
{
#ifdef SMALL
	PUTS("Escuchas al cientifico malvado riendose\n"
		 "sin piedad.");
#else
	putstring("Te vuelves a tu pais. En la distancia,"
			  "Puedes escuchar al malvado cientifico riendose sin piedad mientras el"
			  "desata su horrible pocion magica sobre la tierra.");
#endif
	return GAME_LOST;
}
