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
	0, 	//N	:0
	6, 	//NE:1	-> 	NW	:7
	4, 	//E	:2	-> 	W	:6
	2, 	//SE:3	-> 	SW	:5
	0, 	//S	:4
	-2,	//SW:5	->	SE	:3
	-4,	//W	:6	->	E	:2
	-6	//NW:7	->	NE	:1
};

dir_t bee_dir[5][20] = {
	{ N, N, N, N, N, N, N },
	{ NE, NE, NE, NE, NE, NE, NE, NE },
	{ E, E, E, E, E, E, E, E, E, E, E, E},
	{ SE, SE, SE, SE, SE, SE, SE, SE },
	{ S, S, S, S, S, S, S }
};

coords_t 	bee_coords[10] = {
	{ .col = 2, .row = 3 },
	{ .col = 7, .row = 2 },
	{ .col = 10, .row = 3 },
	{ .col = 14, .row = 7 },
	{ .col = 14, .row = 12 },
	{ .col = 26, .row = 12 },
	{ .col = 10, .row = 21 },
	{ .col = 14, .row = 17 },
	{ .col = 2, .row = 21 },
	{ .col = 7, .row = 22 }
};


#endif

