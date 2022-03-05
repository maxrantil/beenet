#ifndef BEENET_H
#define BEENET_H

#define ROWS 25
#define COLUMNS 30

typedef struct
{
	char	**map;
	int		confirmed_score;
}	t_game;

typedef struct s_brains {
	char	dir;
	bool	hasflower;
}	t_brains;

#endif

