#ifndef BEENET_H
#define BEENET_H

#define ROWS 27
#define COLUMNS 32

typedef struct
{
	char	**map;
	int		confirmed_score;
}	t_game;

typedef struct s_brains {
	char	dir;
	bool	hasflower;
	coords_t	hivelocation;
	char	path[1000]; //remember the task
}	t_brains;

static const int flag_dir[8] = {
	0,
	6,
	4,
	2,
	0,
	-2,
	-4,
	-6
};

#endif

