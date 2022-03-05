#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "agent.h"

typedef struct s_brains {
	char	dir;
	bool	hasflower;
}	t_brains;

t_brains	brains[5];

void	init_brains()
{
	int	i = 0;
	char	dir;

	dir = E;
	while (i != 5)
	{
		brains[i].dir = dir;
		brains[i].hasflower = false;
		i++;
	}
}

void	goback(agent_info_t *info)
{
	if (brains[info->bee].dir == E)
		brains[info->bee].dir = W;
	else
		brains[info->bee].dir = E;
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

command_t think(agent_info_t info)
{
    cell_t bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];

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
        if (flower_dir >= 0 && brains[info.bee].hasflower == false)
        {
		goback(&info);
		brains[info.bee].hasflower = true;
            return (command_t) {
                .action = FORAGE,
                .direction = flower_dir
            };
        }

    }
    int outside_dir = find_neighbour(info, OUTSIDE);
    if (outside_dir >= 0)
	goback(&info);
    return (command_t) {
	.action = MOVE,
	.direction = brains[info.bee].dir
    };
}

int main(int argc, char **argv)
{
    init_brains();
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "example_agent";

    agent_main(host, port, team_name, think);
}

