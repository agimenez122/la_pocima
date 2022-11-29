#include <stdio.h>

#include "game.h"
#include "object.h"
#include "output.h"

void printroom(void)
{
	NR unsigned char i;
	NR BOOL seen;
	NR const char *s;
	NR unsigned char mapdata;
	NR signed char *object;

	seen = FALSE;
	mapdata = gamedata->map[gamedata->room - 1];

	/* Print room description */
	switch (gamedata->room)
	{
		case 1:  case 2:  case 3:
		case 7:  case 8:  case 9:
		case 13: case 14: case 15:
		case 30:
#ifdef SMALL
			s = "Estas en un bosque";
#else
			s = "Estas en un bosque. Hay arboles en todas direcciones"
				"tan lejos como el ojo pueda ver.";
#endif
			break;

		case 4: case 10:
		case 16: case 18:
#ifdef SMALL
			s = "Estas en una gran llanura.";
#else
			s = "Estas parado en una gran llanura."
				"Oyes un zumbido distante de motores desde algun lugar.";
#endif
			break;

		case 12:
#ifdef SMALL
			s = "Estas en una estacion de reciclaje.";
#else
			s = "Estas parado en una gran planicie, al lado de una estacion "
				"de reciclaje.";
#endif
			break;

		case 5: case 11: case 17:
		case 23: case 24:
			switch (gamedata->roadvisit)
			{
			case 0:
			case 1:
				++ gamedata->roadvisit;
			case 3:
#ifdef SMALL
				s = "Estas parado en una autopista.";
#else
				s = "Estas parado en una autopista. "
				    "Pasan varios coches";
#endif
				break;

			case 2:
#ifdef SMALL
				s = "Estas parado en una autopista. \n"
				    "Pasa un skater, cogiendo una Coca Cola "
					"de un coche cercano.";
#else
				s = "Estas parado en una autopista. "
				    "De repente ves pasar un skater, cogiendo una Coca Cola "
					"de un coche cercano.";
#endif
				++ gamedata->roadvisit;
				gamedata->objects[O_COKE] = gamedata->room;
				break;
			}
			break;

		case 6:
#ifdef SMALL
			s = "Estas de pie junto al mar.";
#else
			s = "Estas parado en la orilla del mar. El mar se extiende"
				"hasta donde alcanza la vista hacia el norte y hacia el este.";
#endif
			break;

		case 19: case 25:
#ifdef SMALL
			s = "Estas en el laboratorio del cientifico.";
#else
			s = "Estas dentro del laboratorio del cientifico. Te das cuenta de que "
				"No parece poner mucha energia en mantenerlo ordenado.";
#endif
			break;

		case 20: case 21: case 26: case 27:
#ifdef SMALL
			s = "Estas dentro de una fortaleza.";
#else
			s = "Estas parado dentro de una gran fortaleza. Hay algunas "
				"telaranas en los rincones, y una capa de polvo encima"
				"de practicamente todo.";
#endif
			break;

		case 22:
			if (!(unsigned char) (mapdata & EXIT_WEST))
			{
#ifdef SMALL
				s = "Estas parado al lado de un foso. tu\n"
					"Hay que bajar el puente levadizo para pasar.";
#else
				s = "Estas parado al lado de un gran foso. No hay manera de"
					"pasar por encima de el sin bajar el puente levadizo.";
#endif
			}
			else
			{
#ifdef SMALL
				s = "Estas parado al lado de un puente levadizo.";
#else
				s = "Estas parado al lado de un puente levadizo. Que conduce "
				    "a una gran fortaleza.";
#endif
			}
			break;

		case 28: case 29:
			s = "Estas parado en un vertedero. Apesta!";
			break;

		default:
#ifdef SMALL
			s = "?ERROR";
#else
			s = "Algo anda mal!";
#endif
			break;
	}

#ifdef SMALL
	PUTS(s);
#else
	putstring(s);
#endif

	/* Print room contents */
	object = gamedata->objects;
	for (i = 0; i < OBJECTS; ++ i, ++ object)
	{
		if (*object == gamedata->room)
		{
			if (!seen)
			{
				PUTS("Tu ves");
				seen = TRUE;
			}
			printobject(i);

			if (O_SCIENTIST == i && !gamedata->seenscientist)
			{
				PUTS("El dice: \"¿Que haces aqui?\"");
				gamedata->seenscientist = TRUE;
			}
		}
	}

	/* Print exits */
	PUTS("Salidas disponibles:");
	if ((unsigned char) (mapdata & EXIT_NORTH) != 0)
		PUTCHAR('N');
	if ((unsigned char) (mapdata & EXIT_SOUTH) != 0)
		PUTCHAR('S');
	if ((unsigned char) (mapdata & EXIT_EAST) != 0)
		PUTCHAR('E');
	if ((unsigned char) (mapdata & EXIT_WEST) != 0)
		PUTCHAR('O');
	PUTCHAR('\n');
}
