#ifndef BEENET_H
#define BEENET_H

#define ROWS 27
#define COLUMNS 32

typedef struct
{
	char		**map;
	int			confirmed_score;
	coords_t	hivecords;
}	t_game;

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

