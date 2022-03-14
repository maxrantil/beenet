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
			cell_t check = info.cells[i][j];
			if (check == FLOWER)
			{
				if (check_for_duplicate(info, count, info.col - i, info.col - j))
					break;
				flower_cords[count].col = info.col - 3 + j; //bee col = 2 // flower col = 0
				flower_cords[count].row = info.row - 3 + i; //bee row = 3 // flower row = 0
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

coords_t	get_nearby_flower(int beenum)
{
	coords_t cpycords;

	cpycords.row = -1;
	cpycords.col = -1;
	for (int i = 0; i < 50; i++)
	{
		if (beenum == 0)
		{
			if (flower_cords[i].row < 8 && flower_cords[i].col < 29)
			{
				cpycords.row = flower_cords[i].row;
				cpycords.col = flower_cords[i].col;
				flower_cords[i].row = -1;
				flower_cords[i].col = -1;
				break ;
			}
		}
		else if (beenum == 1)
		{
			if (flower_cords[i].row < 10 && flower_cords[i].col >= 10 && flower_cords[i].col < 29)
			{
				cpycords.row = flower_cords[i].row;
				cpycords.col = flower_cords[i].col;
				flower_cords[i].row = -1;
				flower_cords[i].col = -1;
				break ;
			}
		}
		else if (beenum == 2)
		{
			if (flower_cords[i].row >= 8 && flower_cords[i].row <= 16 && flower_cords[i].col < 29)
			{
				cpycords.row = flower_cords[i].row;
				cpycords.col = flower_cords[i].col;
				flower_cords[i].row = -1;
				flower_cords[i].col = -1;
				break ;
			}
		}
		else if (beenum == 3)
		{
			if (flower_cords[i].row > 14 && flower_cords[i].col >= 10 && flower_cords[i].col < 29)
			{
				cpycords.row = flower_cords[i].row;
				cpycords.col = flower_cords[i].col;
				flower_cords[i].row = -1;
				flower_cords[i].col = -1;
				break ;
			}
		}
		else if (beenum == 4)
		{
			if (flower_cords[i].row > 16 && flower_cords[i].col < 29)
			{
				cpycords.row = flower_cords[i].row;
				cpycords.col = flower_cords[i].col;
				flower_cords[i].row = -1;
				flower_cords[i].col = -1;
				break ;
			}
		}
	}
	return (cpycords);
}

dir_t	calculate_distance(coords_t bee, coords_t dest)
{
	//while (bee.row != dest.row || bee.col != dest.col)
	//{
		if (bee.row > dest.row && bee.col > dest.col)
			return (NW);
		else if (bee.row < dest.row && bee.col > dest.col)
			return (SW);
		else if (bee.row > dest.row && bee.col < dest.col)
			return (NE);
		else if (bee.row < dest.row && bee.col < dest.col)
			return (SE);
		else if (bee.row == dest.row && bee.col < dest.col)
			return (E);
		else if (bee.row == dest.row && bee.col > dest.col)
			return (W);
		else if (bee.row > dest.row && bee.col == dest.col)
			return (N);
		else if (bee.row < dest.row && bee.col == dest.col)
			return (S);
	//}
	return (-1);
}

dir_t	get_flower_dir(agent_info_t *info, coords_t flower_dir)
{
	dir_t dir;
	if (info->row < flower_dir.row && info->col < flower_dir.col) // bee is NW of flower
		dir = SE;
	else if (info->row < flower_dir.row && info->col > flower_dir.col) // bee is NE of flower
		dir = SW;
	else if (info->row > flower_dir.row && info->col < flower_dir.col) // bee is SE of flower
		dir = NE;
	else if (info->row > flower_dir.row && info->col > flower_dir.col) // bee is SE of flower
		dir = NW;
	else if (info->row == flower_dir.row && info->col < flower_dir.col) // bee is W of flower
		dir = E;
	else if (info->row == flower_dir.row && info->col > flower_dir.col) // bee is W of flower
		dir = W;
	else if (info->row > flower_dir.row && info->col == flower_dir.col) // bee is SE of flower
		dir = N;
	else if (info->row < flower_dir.row && info->col == flower_dir.col) // bee is SE of flower
		dir = S;
	return (dir);
}

int a = 0, b = 3, c = 6, d = 9, e = 12;

dir_t get_mission(agent_info_t *info)
{
	int flag = 0;
	coords_t bee = { info->row, info->col };
	coords_t checkflower = get_nearby_flower(info->bee);
	if (checkflower.col != -1 && checkflower.row != -1)
		return (get_flower_dir(info, checkflower));
  	if (is_obstacle(*info))
		return (NE);
	if (info->bee == 0)
	{
here1:
	 	flag = 25 * (info->player == 1);
		flag = 16 * (info->player == 1 && a == 1);
		flag = 0 - 25 * (info->player == 1 && a == 2);
		coords_t dest = { bee_coords[a].row, bee_coords[a].col + flag };
		dir_t dir = calculate_distance(bee, dest);
		if (dir == -1)
		{
			a++;
			goto here1;
		}
		if (a > 2)
			return (rand() % 8);
		return (dir);
	}
	if (info->bee == 1)
	{
here2:
		flag = 3 * (info->player == 1 && b != 5);
		flag -= 23 * (info->player == 1 && b == 5);
		coords_t dest = { bee_coords[b].row, bee_coords[b].col + flag };
		dir_t dir = calculate_distance(bee, dest);
		if (dir == -1)
		{
			b++;
			goto here2;
		}
		if (b > 5)
			return (rand() % 8);
		return (dir);
	}
	if (info->bee == 2)
	{
here3:
		coords_t dest = { bee_coords[c].row, bee_coords[c].col + flag };
		dir_t dir = calculate_distance(bee, dest);
		if (dir == -1)
		{
			c++;
			goto here3;
		}
		if (c > 8)
			return (rand() % 8);
		return (dir);
		flag = 3 * (info->player == 1 && c == 6);
		flag -= 23 * (info->player == 1 && (c == 7 || c == 8));
	}
	if (info->bee == 3)
	{
here4:
		coords_t dest = { bee_coords[d].row, bee_coords[d].col + flag };
		dir_t dir = calculate_distance(bee, dest);
		if (dir == -1)
		{
			d++;
			goto here4;
		}
		if (d > 11)
			return (rand() % 8);
		return (dir);
		flag = 3 * (info->player == 1 && d != 11);
		flag -= 23 * (info->player == 1 && d == 11);
	}
	if (info->bee == 4)
	{
here5:
		coords_t dest = { bee_coords[e].row, bee_coords[e].col + flag };
		dir_t dir = calculate_distance(bee, dest);
		if (dir == -1)
		{
			e++;
			goto here5;
		}
		if (e > 14)
			return (rand() % 8);
		return (dir);
		flag = 25 * (info->player == 1);
		flag = 16 * (info->player == 1 && e == 13);
		flag = 0 - 25 * (info->player == 1 && e == 14);
	}
	return (rand() % 8);
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
		dir = get_mission(info);// + (flag_dir[dir] * (info->player == 1));
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
	static int a, b;
	for (int i = 0; i < 50; i++)
	{
		flower_cords[i].col = -1;
		flower_cords[i].row = -1;
	}
	get_flowerpos(info);
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

