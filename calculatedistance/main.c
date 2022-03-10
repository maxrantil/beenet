#include <stdio.h>
#include <strings.h>

typedef enum
{
    N = 0,
    NE = 1,
    E = 2,
    SE = 3,
    S = 4,
    SW = 5,
    W = 6,
    NW = 7
} dir_t;

typedef struct s_cords
{
	int	row;
	int	col;
}	t_cords;

t_cords flowers[100];
t_cords	bee;
t_cords	hive;

void	movethebee(t_cords *movingbee, int dir)
{
	if (dir == N)
		movingbee->row--;
	if (dir == S)
		movingbee->row++;
	if (dir == E)
		movingbee->col++;
	if (dir == W)
		movingbee->col--;
	if (dir == NW)
	{
		movingbee->row--;
		movingbee->col--;
	}
	if (dir == SW)
	{
		movingbee->row++;
		movingbee->col--;
	}
	if (dir == SE)
	{
		movingbee->row++;
		movingbee->col++;
	}
	if (dir == NE)
	{
		movingbee->row--;
		movingbee->col++;
	}
}

int	memory[100];

int	calculate_distance()
{
	int	step = 0;
	int	i = 0;
	t_cords	movingbee = bee;

	while (movingbee.row != flowers[i].row || movingbee.col != flowers[i].col)
	{
		if (movingbee.row > flowers[i].row && movingbee.col > flowers[i].col)
			memory[step] = NW;
		else if (movingbee.row < flowers[i].row && movingbee.col > flowers[i].col)
			memory[step] = SW;
		else if (movingbee.row > flowers[i].row && movingbee.col < flowers[i].col)//cols < is east
			memory[step] = NE;
		else if (movingbee.row < flowers[i].row && movingbee.col < flowers[i].col)
			memory[step] = SE;
		else if (movingbee.row == flowers[i].row && movingbee.col < flowers[i].col)
			memory[step] = E;
		else if (movingbee.row == flowers[i].row && movingbee.col > flowers[i].col)
			memory[step] = W;
		else if (movingbee.row > flowers[i].row && movingbee.col == flowers[i].col)
			memory[step] = N;
		else if (movingbee.row < flowers[i].row && movingbee.col == flowers[i].col)
			memory[step] = S;
		movethebee(&movingbee, memory[step]);
		step++;
	}
	return (step);
}

int main()
{
	int	i = 0;
	int	steps = 0;
	bee.col = 4;
	bee.row = 4;
	hive.col = 2;
	hive.row = 6;
	flowers[0].col = 2;
	flowers[0].row = 3;
	flowers[1].col = 7;
	flowers[1].row = 2;
	flowers[2].col = 3;
	flowers[2].row = 1;

	bzero(memory, 100);
	steps = calculate_distance();
	while (i != steps - 1)
	{
		printf("%d\n", memory[i]);
		i++;
	}

}

