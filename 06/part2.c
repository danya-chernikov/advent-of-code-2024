#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* MAX_NUMS_STR - the maximum
 * number of numbers in a string */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_LEN		256
# define MAX_LINE_NUM		256
# define MAX_NUMS_STR		32

typedef struct s_coord
{
	int	X;
	int	Y;
}	t_coord;

/*int	is_loop_exist(t_coord *t, int turn_cnt)
{
	int	is_loop;

	is_loop = 0;
	for (int i = 0; i < turn_cnt; i++)
	{
		if (((t[turn_cnt - 1].X == t[turn_cnt - 5].X) && (t[turn_cnt - 1].Y == t[turn_cnt - 5].Y)) &&
		   ((t[turn_cnt - 2].X == t[turn_cnt - 6].X) && (t[turn_cnt - 2].Y == t[turn_cnt - 6].Y)) &&
		   ((t[turn_cnt - 3].X == t[turn_cnt - 7].X) && (t[turn_cnt - 3].Y == t[turn_cnt - 7].Y)) &&
		   ((t[turn_cnt - 4].X == t[turn_cnt - 8].X) && (t[turn_cnt - 4].Y == t[turn_cnt - 8].Y)))
		{

		}
	}
}*/

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];

	/* Number of lines read from a file */
	int		line_cnt;

	/* The length of the last
	 * line read from a file */
	int		line_len; 

	int		ch;
	int		i;
	FILE	*fptr;

	/* Here we store coordinates of the cell before turning right */
	t_coord	t[MAX_LINE_LEN * MAX_LINE_NUM];

	char	m[MAX_LINE_LEN][MAX_LINE_NUM];
	int		width;
	int		height;

	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(filename, argv[1], MAX_FILE_NAME_BUF);

	fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[1]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	/* Let's read the ordering rules */
	i = 0;
	ch = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				m[line_cnt][i] = '\0';
				line_len = i;
				i = 0;

				line_cnt++;

				if (line_len == 0) /* The ordering rule section ended */
					break;

				continue;
			}
			else
			{
				line_len = i;
				m[line_cnt][i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			m[line_cnt][i] = ch;
		else
		{
			line_len = i;
			m[line_cnt][i] = '\0';
			break;
		}
		i++;
	}
	width = line_len;
	height = line_cnt;

	printf("width = %d\n", width);
	printf("height = %d\n", height);

	printf("\n");

/*	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	/* Let's determine the initial position of the guard */
	int	x;
	int	y;
	int	found = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m[i][j] == '^')
			{
				x = j;
				y = i;
				found = 1;
				break;
			}
		}
		if (found)
			break;
	}

	printf("x = %d\n", x);
	printf("y = %d\n", y);

	printf("\n");

	/* Direction of the guard.
	 *     d = 0 - up;
	 *     d = 1 - right;
	 *     d = 2 - down;
	 *     d = 3 - left. */
	int	d;

	int	pos_cnt = 0;
	int	turn_cnt; /* turns counter */
	int	is_loop;
	int	tx; // temporary X
	int	ty; // temporary Y

	for (int cy = 0; cy < height; cy++)
	{
		for (int cx = 0; cx < width; cx++)
		{	
			if (m[cy][cx] != '#' && m[cy][cx] != '^')
			{
				m[cy][cx] = 'O';

				turn_cnt = 0;
				is_loop = 0;
				d = 0;
				tx = x;
				ty = y;
				while ((tx >= 0 && tx < width) && (ty >= 0 && ty < height))
				{
					if (turn_cnt > 7)
					{

						if (turn_cnt > 50000)
						{
							is_loop = 1;
							break;
						}
						/* If a loop was detected */
						if (((t[turn_cnt - 1].X == t[turn_cnt - 5].X) && (t[turn_cnt - 1].Y == t[turn_cnt - 5].Y)) &&
							((t[turn_cnt - 2].X == t[turn_cnt - 6].X) && (t[turn_cnt - 2].Y == t[turn_cnt - 6].Y)) &&
							((t[turn_cnt - 3].X == t[turn_cnt - 7].X) && (t[turn_cnt - 3].Y == t[turn_cnt - 7].Y)) &&
							((t[turn_cnt - 4].X == t[turn_cnt - 8].X) && (t[turn_cnt - 4].Y == t[turn_cnt - 8].Y)))
						{

							is_loop = 1;
							break;
						}

					}

					/* Trying to step up */
					if (d == 0)
					{
						if (m[ty - 1][tx] == '#' || m[ty - 1][tx] == 'O')
						{
							t[turn_cnt].X = tx;
							t[turn_cnt].Y = ty;
							turn_cnt++;
							d = (d + 1) % 4;
						}
						else
						{
							ty--;
						}
						continue;
					}
					/* Trying to step right */
					if (d == 1)
					{
						if (m[ty][tx + 1] == '#' || m[ty][tx + 1] == 'O')
						{
							t[turn_cnt].X = tx;
							t[turn_cnt].Y = ty;
							turn_cnt++;
							d = (d + 1) % 4;
						}
						else
						{
							tx++;
						}
						continue;
					}
					/* Trying to step down */
					if (d == 2)
					{
						if (m[ty + 1][tx] == '#' || m[ty + 1][tx] == 'O')
						{

							t[turn_cnt].X = tx;
							t[turn_cnt].Y = ty;
							turn_cnt++;
							d = (d + 1) % 4;
						}
						else
						{
							ty++;
						}
						continue;
					}
					/* Trying to step left */
					if (d == 3)
					{
						if (m[ty][tx - 1] == '#' || m[ty][tx - 1] == 'O')
						{
							t[turn_cnt].X = tx;
							t[turn_cnt].Y = ty;
							turn_cnt++;
							d = (d + 1) % 4;
						}
						else
						{
							tx--;
						}
						continue;
					}	

				} // while ((x >= 0 && x < width) && (y >= 0 && y < height))
				
				if (is_loop)
				{
					pos_cnt++;
					/*printf("LOOP WAS FOUND!!!\n");
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
							printf("%c", m[i][j]);
						printf("\n");
					}
					printf("\n");*/
				}
				
				m[cy][cx] = '.';
				m[y][x] = '^'; // It's very important to restore the initial guard position because it will be overwritten by '.'

			} // if (m[cy][cx] != '#' && m[cy][cx] != '^')

		} // for (int cx = 0; cx < width; cx++)

	} // for (int cy = 0; cy < height; cy++)




	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	printf("number of distinct positions is %d\n", pos_cnt);


	fclose(fptr);
	exit (EXIT_SUCCESS);
}
