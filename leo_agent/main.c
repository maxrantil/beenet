#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "agent.h"
#include "beenet.h"

t_brains	brains[5];
t_game 		game;



t_game		init_game()
{
	t_game *game;

	game = (t_game *)malloc(sizeof(t_game));
	game->map = (char **)malloc(sizeof(char*) * ROWS);
	for (int i = 0; i < ROWS; i++)
	{
		game->map[i] = (char *)malloc(sizeof(char) * COLUMNS);
		for (int j = 0; j < COLUMNS; j++)
			game->map[i][j] = '#';
	}
	game->confirmed_score = 0;
	return (*game);
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

char	enum_to_str(cell_t type, int player, coords_t cords, agent_info_t info)
{
	if (type == EMPTY)
		return ('.');
	else if (type == FLOWER)
		return ('F');
	else if (type == WALL)
		return ('W');
	else if (type == OUTSIDE)
		return ('0');

	if (player == 0)
	{
		if (type == BEE_0)
			return ('B');
		else if (type == BEE_1)
			return ('E');
		else if (type == BEE_0_WITH_FLOWER)
			return ('b');
		else if (type == BEE_1_WITH_FLOWER)
			return ('e');
		else if (type == HIVE_0)
		{
			game.hivecords = cords;
			return ('H');
		}
		else if (type == HIVE_1)
			return ('h');
	}
	else if (player == 1)
	{
		if (type == BEE_1)
			return ('B');
		else if (type == BEE_0)
			return ('E');
		else if (type == BEE_1_WITH_FLOWER)
			return ('b');
		else if (type == BEE_0_WITH_FLOWER)
			return ('e');
		else if (type == HIVE_1)
		{
			game.hivecords = cords;
			return ('H');
		}
		else if (type == HIVE_0)
			return ('h');
	}
	return ('X');
}

void	update_map(t_game game, agent_info_t info)
{
	coords_t center = {VIEW_DISTANCE, VIEW_DISTANCE};
	coords_t bee = {info.row, info.col};

	game.map[info.row][info.col] = enum_to_str(info.cells[center.row][center.col], info.player, bee, info);
	for (int dir = 0; dir < 8; dir++)
	{
		coords_t gcoords = direction_to_coords(bee, dir);
		coords_t coords = direction_to_coords(center, dir);
		game.map[gcoords.row][gcoords.col] = enum_to_str(
				info.cells[coords.row][coords.col],
				info.player,
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

dir_t	get_player_dir(agent_info_t *info, int hasflower)
{
	dir_t dir;
	int bee_pos_is_midmap = (info->col > 1 && info->col < 28);
	int go_way = rand() % 8;
	if (go_way == W)
		go_way = E;
	if (hasflower)
	{
		if (info->row > game.hivecords.row && bee_pos_is_midmap)
			dir = NW + (flag_dir[dir] * (info->player == 1));	
		if (info->row < game.hivecords.row && bee_pos_is_midmap)
			dir = SW + (flag_dir[dir] * (info->player == 1));
		if (info->col == game.hivecords.col && info->row > 12)
			dir = N;
		if (info->col == game.hivecords.col && info->row < 12)
			dir = S;
		if (info->col > game.hivecords.col && info->player == 0)
			dir = W;
		if (info->col < game.hivecords.col && info->player == 1)
			dir = E;
	}
	else
		dir = go_way + (flag_dir[dir] * (info->player == 1));
	return (dir);
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
	if (brains[info.bee].hasflower == true)
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
		return (command_t) {
			.action = MOVE,
			.direction = get_player_dir(&info, 1)
		};
	}
	return (command_t) {
		.action = MOVE,
		.direction = get_player_dir(&info, 0)
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

