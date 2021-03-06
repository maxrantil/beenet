#ifndef BEENET_H
#define BEENET_H

#define ROWS 25
#define COLUMNS 30

#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/agent.h"

typedef struct
{
	char		**map;
	int			confirmed_score;
	coords_t	hivecords;
}	t_game;

t_game 		game;

typedef struct s_brains {
	char	dir;
	bool	hasflower;
}	t_brains;


static const int flag_dir[8] = {
	0, 	//N
	6, 	//NE 	-> 	NW
	4, 	//E		-> 	W
	2, 	//SE	-> 	SW
	0, 	//S
	-2,	//SW	->	SE
	-4,	//W		->	E
	-6	//NW	->	NE
};

#endif

