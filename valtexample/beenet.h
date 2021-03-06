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

typedef struct s_brains {
	char	dir;
	bool	hasflower;
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

