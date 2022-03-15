#ifndef BEENET_H
# define BEENET_H

# define ROWS 25
# define COLUMNS 30

# include <time.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "../src/agent.h"
# include "../src/arena_common.h"
# include <strings.h>

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

//t_brains	brains[5];

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

coords_t 	bee_coords[15] = {
	{ .col = 2, .row = 3 },		//0
	{ .col = 26, .row = 3 },		//0
	{ .col = 28, .row = 10 },	//0
	{ .col = 14, .row = 5 },	//1
	{ .col = 18, .row = 7 },	//1
	{ .col = 26, .row = 11 },	//1
	{ .col = 14, .row = 12 },	//2
	{ .col = 26, .row = 12 },	//2
	{ .col = 26, .row = 12 },	//2
	{ .col = 14, .row = 19 },	//3
	{ .col = 18, .row = 17 },	//3
	{ .col = 26, .row = 13 },	//3
	{ .col = 2, .row = 21 },	//4
	{ .col = 26, .row = 21 },	//4
	{ .col = 28, .row = 14 }	//4
};

coords_t 	bee_coords_p1[15] = {
	{ .col = 27, .row = 3 },		//0
	{ .col = 3, .row = 3 },		//0
	{ .col = 2, .row = 10 },	//0
	{ .col = 16, .row = 5 },	//1
	{ .col = 12, .row = 7 },	//1
	{ .col = 3, .row = 11 },	//1
	{ .col = 16, .row = 12 },	//2
	{ .col = 3, .row = 12 },	//2
	{ .col = 3, .row = 12 },	//2
	{ .col = 16, .row = 19 },	//3
	{ .col = 12, .row = 17 },	//3
	{ .col = 3, .row = 13 },	//3
	{ .col = 27, .row = 21 },	//4
	{ .col = 3, .row = 21 },	//4
	{ .col = 2, .row = 14 }	//4
};

#endif
