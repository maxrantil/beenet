#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "agent.h"
#include "beenet.h"

t_brains	brains[5];
t_game 		game;

void	ft_bzero(void *s, size_t n)
{
	int	i;

	i = 0;
	while (n--)
		((unsigned char *)s)[i++] = 0;
}

void	*ft_memalloc(size_t size)
{
	void	*mem;

	mem = malloc(sizeof(mem) * size);
	if (!mem)
		return (NULL);
	ft_bzero(mem, size);
	return (mem);
}

dir_t	get_player_dir(dir_t dir, agent_info_t *info)
{
	dir = dir + (flag_dir[dir] * (info->player == 1));
	return (dir);
}

t_game		init_game()
{
	t_game game;
	int j;

	//game = (t_game *)ft_memalloc(sizeof(t_game) + 1);
	game.map = (char **)malloc(sizeof(char *) * ROWS + 1);
	if (!game.map)
		exit(1);
	game.map[ROWS] = 0;
	for (int i = 0; i < ROWS; i++)
	{
		game.map[i] = (char *)ft_memalloc(sizeof(char) * COLUMNS + 1);
		if (!game.map[i])
			exit(1);
		for (j = 0; j < COLUMNS; j++)
			game.map[i][j] = '#';
		game.map[i][j] = '\0';

	}
	game.confirmed_score = 0;
	return (game);
}

void	init_brains()
{
	int		i = 0;
	char	dir;

	dir = E;
	while (i != 5)
	{
		brains[i].dir = dir;
		brains[i].hasflower = false;
		i++;
	}
}

/* void	goback(agent_info_t *info)
{
	if (brains[info->bee].dir == E)
		brains[info->bee].dir = W;
	else
		brains[info->bee].dir = E;
} */

char	enum_to_str(cell_t type, coords_t cords, agent_info_t info)
{
	if (type == EMPTY)
		return ('.');
	if (type == FLOWER)
		return ('F');
	if (type == WALL)
		return ('W');
	if (type == OUTSIDE)
		return ('0');

	if (info.player == 0)
	{
		if (type == BEE_0)
			return ('B');
		if (type == BEE_1)
			return ('E');
		if (type == BEE_0_WITH_FLOWER)
			return ('b');
		if (type == BEE_1_WITH_FLOWER)
			return ('e');
		if (type == HIVE_0)
		{
			game.hivecords = cords;
			return ('H');
		}
		if (type == HIVE_1)
			return ('h');
	}
	else if (info.player == 1)
	{
		if (type == BEE_1)
			return ('B');
		if (type == BEE_0)
			return ('E');
		if (type == BEE_1_WITH_FLOWER)
			return ('b');
		if (type == BEE_0_WITH_FLOWER)
			return ('e');
		if (type == HIVE_1)
		{
			game.hivecords = cords;
			return ('H');
		}
		if (type == HIVE_0)
			return ('h');
	}
	return ('X');
}

void	update_map(t_game game, agent_info_t info)
{
	coords_t center = {VIEW_DISTANCE, VIEW_DISTANCE};
	coords_t bee = {info.row, info.col};

	game.map[info.row][info.col] = enum_to_str(info.cells[center.row][center.col], bee, info);
	for (int dir = 0; dir < 8; dir++)		//here somewhere is the bug that makes it crash
	{
		coords_t gcoords = direction_to_coords(bee, dir);
		coords_t coords = direction_to_coords(center, dir);
		game.map[gcoords.row][gcoords.col] = enum_to_str(
				info.cells[coords.row][coords.col],
				gcoords,
				info);
	}
}

/* void	print_map(t_game game)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			write(1, &game.map[i][j], 1);
		}
		write(1, "\n", 1);
	}
} */

int find_neighbour(agent_info_t info, cell_t type)
{
    coords_t center = {VIEW_DISTANCE, VIEW_DISTANCE};

    for(int dir = 0 ; dir < 8 ; dir++)
    {
        coords_t coords = direction_to_coords(center, dir);
        cell_t neighbour = info.cells[coords.row][coords.col];
        if (neighbour == type)
        {
            return dir;
        }
    }
    return -1;
}
command_t think(agent_info_t info)
{
    cell_t bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];

	update_map(game, info);
	//print_map(game);
	//printf("row: %d\n col: %d\n", game.hivecords.row, game.hivecords.col);
    if (is_bee_with_flower(bee))
    {
        int hive_dir = find_neighbour(info, hive_cell(info.player));
        if (hive_dir >= 0)
        {
			brains[info.bee].hasflower = false;
            return (command_t) {
                .action = FORAGE,
                .direction = hive_dir
            };
        }
    }
    else
    {
        int flower_dir = find_neighbour(info, FLOWER);
        if (flower_dir >= 0 && brains[info.bee].hasflower == false)
        {
			brains[info.bee].hasflower = true;
            return (command_t) {
                .action = FORAGE,
                .direction = flower_dir
            };
        }
    }
	/* if (brains[info.bee].hasflower == true)
	{
		int obstacle;
			obstacle = find_neighbour(info, FLOWER);
			if (obstacle >= 0)
			{
				return (command_t) {
					.action = MOVE,
					.direction = rand() % 8
				};
			}
		if (info.player == 0)
		{
			if (find_neighbour(info, BEE_1) >= 0 || find_neighbour(info, BEE_1_WITH_FLOWER) >= 0)
			{
				return (command_t) {
					.action = MOVE,
					.direction = rand() % 8
				};
			}
			if (info.row < game.hivecords.row && info.col > 1)
			{
				return (command_t) {
					.action = MOVE,
					.direction = SW
				};
			}
			if (info.col == game.hivecords.col && info.row < 12)
			{
				return (command_t) {
					.action = MOVE,
					.direction = S
				};
			}
			if (info.row > game.hivecords.row && info.col > 1)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NW
				};
			}
			if (info.col == game.hivecords.col && info.row > 12)
			{
				return (command_t) {
					.action = MOVE,
					.direction = N
				};
			}
			if (info.col > game.hivecords.col)
				return (command_t) {
					.action = MOVE,
					.direction = get_player_dir(W, &info)
				};
		}
		else
		{
			if (find_neighbour(info, BEE_0) >= 0 || find_neighbour(info, BEE_0_WITH_FLOWER) >= 0)
			{
				return (command_t) {
					.action = MOVE,
					.direction = rand() % 8
				};
			}
			if (info.row < game.hivecords.row && info.col < 28)
			{
				return (command_t) {
					.action = MOVE,
					.direction = SE
				};
			}
			if (info.col == game.hivecords.col && info.row < 12)
			{
				return (command_t) {
					.action = MOVE,
					.direction = S
				};
			}
			if (info.row < game.hivecords.row && info.col < 28)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NE
				};
			}
			if (info.col == game.hivecords.col && info.row > 12)
			{
				return (command_t) {
					.action = MOVE,
					.direction = N
				};
			}
			if (info.col < game.hivecords.col)
				return (command_t) {
					.action = MOVE,
					.direction = E
				};
		}
	} */
	int go_way = rand() % 8;

	if (go_way == W)
		go_way = E;
	return (command_t) {
		.action = MOVE,
		.direction = get_player_dir(go_way, &info)
	};
}

int main(int argc, char **argv)
{
	game = init_game();

   init_brains();
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "Beenet";

    agent_main(host, port, team_name, think);
}

/*
    int outside_dir = find_neighbour(info, OUTSIDE);
    if (outside_dir >= 0)
		goback(&info);
    return (command_t) {
		.action = MOVE,
		.direction = brains[info.bee].dir
    };

    */

