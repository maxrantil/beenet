#include "beenet.h"

t_brains	brains[5];
int			bee_cords;

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

int	is_obstacle(agent_info_t info)
{
	cell_t obstacle = 0;

	for (int i = 1; i < 6; i++)
	{
		if (find_neighbour(info, obstacle + i) >= 0)
			return (1);
	}
	if (find_neighbour(info, OUTSIDE) >= 0)
		return (1);
	return (0);
}

dir_t get_mission(agent_info_t *info)
{
	if (info->row == 12 && is_obstacle(*info))
		return (NE);
	if (info->bee == 0)
	{
		if (info->row == bee_coords[0].row && info->col == bee_coords[0].col)
		{
			bee_coords[0].row = -1;
			bee_coords[0].col = -1;
		}
		if (bee_coords[0].row == -1 || bee_coords[0].col == -1)
			return (E);
		if (info->row > bee_coords[0].row && info->col == bee_coords[0].col)
			return (N);
	}
	if (info->bee == 1)
	{
		if (info->row == bee_coords[2].row && info->col == bee_coords[2].col)
		{
			bee_coords[2].row = -1;
			bee_coords[2].col = -1;
		}
		if (bee_coords[2].row == -1 || bee_coords[2].col == -1)
			return (E);
		if (info->row > bee_coords[2].row && info->col < bee_coords[2].col)
			return (NE);
	}
	if (info->bee == 2)
	{
		if (info->row == bee_coords[4].row && info->col == bee_coords[4].col)
		{
			bee_coords[4].row = -1;
			bee_coords[4].col = -1;
		}
		if (bee_coords[4].row == -1 || bee_coords[4].col == -1)
			return (E);
		if (info->row == bee_coords[4].row && info->col < bee_coords[4].col)
			return (E);
	}
	if (info->bee == 3)
	{
		if (info->row == bee_coords[6].row && info->col == bee_coords[6].col)
		{
			bee_coords[6].row = -1;
			bee_coords[6].col = -1;
		}
		if (bee_coords[6].row == -1 || bee_coords[6].col == -1)
			return (E);
		if (info->row < bee_coords[6].row && info->col < bee_coords[6].col)
			return (SE);
	}
	if (info->bee == 4)
	{
		if (info->row == bee_coords[8].row && info->col == bee_coords[8].col)
		{
			bee_coords[8].row = -1;
			bee_coords[8].col = -1;
		}
		if (bee_coords[8].row == -1 || bee_coords[8].col == -1)
			return (E);
		if (info->row < bee_coords[8].row && info->col == bee_coords[8].col)
			return (S);
	}
	return (E);
}

dir_t	get_player_dir(agent_info_t *info, int hasflower)
{
	dir_t dir;
	int bee_pos_is_midmap = (info->col > 1 && info->col < 28);
	if (hasflower == 0)
		dir = get_mission(info) + (flag_dir[dir] * (info->player == 1));
	else
	{
		if (info->row > game.hivecords.row && bee_pos_is_midmap)
			dir = NW + flag_dir[NW] * (info->player == 1);
		else if (info->row < game.hivecords.row && bee_pos_is_midmap)
			dir = SW + flag_dir[SW] * (info->player == 1);
		else if (info->col == game.hivecords.col && info->row > 12)
			dir = N;
		else if (info->col == game.hivecords.col && info->row < 12)
			dir = S;
		else if (info->col > game.hivecords.col && info->player == 0)
			dir = W;
		else if (info->col < game.hivecords.col && info->player == 1)
			dir = E;
	}
	return (dir);
}

command_t think(agent_info_t info)
{
    cell_t bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];
	game.hivecords.row = 12;
	game.hivecords.col = 1 + 27 * (info.player == 1);
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
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "Leo Beenet";

    agent_main(host, port, team_name, think);
}
