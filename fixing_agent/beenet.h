#ifndef BEENET_H
#define BEENET_H

#define ROWS 25
#define COLUMNS 30


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

int 	find_neighbour(agent_info_t info, cell_t type);
dir_t	get_player_dir(agent_info_t *info, int hasflower);

#endif

