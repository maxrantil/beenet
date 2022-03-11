#include "beenet.h"

coords_t	flower_cords[50];

int	check_for_duplicate(agent_info_t info, int count, int col, int row)
{
	for (int i = count; i > 0; i--)
	{
		if (flower_cords[i].row == -1 && flower_cords[i].col == -1)
			return (0);
		if (flower_cords[i].row == row && flower_cords[i].col == col)
			return (1);
	}
	return (0);
}

void get_flowerpos(agent_info_t info)
{
	int count = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (info.cells[i][j] == FLOWER)
			{
				if (check_for_duplicate(info, count, info.col - i, info.col - j))
					break;
				flower_cords[count].col = info.col - 3 + i; //bee col = 2 // flower col = 0
				flower_cords[count].row = info.row - 3 + j; //bee row = 3 // flower row = 0
				count++;
			}
		}
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
	int a = 0, b = 2, c = 4, d = 6, e = 8;
	if (is_obstacle(*info))
		return (NE);
	if (info->bee == 0)
	{
		if (info->row == bee_coords[a].row && info->col == bee_coords[a].col)
		{
			bee_coords[a].row = -1;
			bee_coords[a].col = -1;
			a++;
		}
		if (info->row > bee_coords[a].row && info->col == bee_coords[a].col)
			return (N);
		if (info->row > bee_coords[a].row && info->col < bee_coords[a].col)
			return (NE);
	}
	if (info->bee == 1)
	{
		if (info->row == bee_coords[b].row && info->col == bee_coords[b].col)
		{
			bee_coords[b].row = -1;
			bee_coords[b].col = -1;
			b++;
		}
		if (info->row > bee_coords[b].row && info->col < bee_coords[b].col)
			return (NE);
		if (info->row == bee_coords[b].row && info->col < bee_coords[b].col)
			return (E);
	}
	if (info->bee == 2)
	{
		if (info->row == bee_coords[c].row && info->col == bee_coords[c].col)
		{
			bee_coords[c].row = -1;
			bee_coords[c].col = -1;
			c++;
		}
		if (info->row == bee_coords[c].row && info->col < bee_coords[c].col)
			return (E);
	}
	if (info->bee == 3)
	{
		if (info->row == bee_coords[d].row && info->col == bee_coords[d].col)
		{
			bee_coords[d].row = -1;
			bee_coords[d].col = -1;
			d++;
		}
		if (info->row < bee_coords[d].row && info->col < bee_coords[d].col)
			return (SE);
		if (info->row == bee_coords[b].row && info->col < bee_coords[b].col)
			return (E);
	}
	if (info->bee == 4)
	{
		if (info->row == bee_coords[e].row && info->col == bee_coords[e].col)
		{
			bee_coords[e].row = -1;
			bee_coords[e].col = -1;
			e++;
		}
		if (info->row < bee_coords[e].row && info->col == bee_coords[e].col)
			return (S);
		if (info->row < bee_coords[a].row && info->col < bee_coords[a].col)
			return (SE);
	}
	return (E);
}

dir_t	get_player_dir(agent_info_t *info, int hasflower)
{
	dir_t dir;
	int go_way = rand() % 8;
	if (go_way == W && info->player == 0)
		go_way = E;
	if (go_way == E && info->player == 1)
		go_way = W;
	int bee_pos_is_midmap = (info->col > 1 && info->col < 28);
	if (hasflower == 0)
		dir = get_mission(info) + (flag_dir[dir] * (info->player == 1));
	else
	{
		if (is_obstacle(*info))
			return (go_way + (flag_dir[dir] * (info->player == 1)));
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
	static int a;
	for (int i = 0; i < 50; i++)
	{
		flower_cords[i].col = -1;
		flower_cords[i].row = -1;
	}
	
	
	/* for (int i = 0; i < 50; i++)
	{
		if (flower_cords[i].col != -1 && flower_cords[i].row != -1)
			printf("%d ,%d\n", flower_cords[i].col, flower_cords[i].row);
	} */
    if (is_bee_with_flower(bee))
    {
        int hive_dir = find_neighbour(info, hive_cell(info.player));
        if (hive_dir >= 0)
        {
            return (command_t) {
                .action = FORAGE,
                .direction = hive_dir
            };
        }
    }
    else
    {
        int flower_dir = find_neighbour(info, FLOWER);
        if (flower_dir >= 0)
        {
            return (command_t) {
                .action = FORAGE,
                .direction = flower_dir
            };
        }
    }

	if (is_bee_with_flower(bee))
	{
		return (command_t) {
			.action = MOVE,
			.direction = get_player_dir(&info, 1)
		};
	}
	if (info.player == 0 && a == 0)
	{
		get_flowerpos(info);
		printf("%d ,%d\n", flower_cords[0].col, flower_cords[0].row);
		a++;
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