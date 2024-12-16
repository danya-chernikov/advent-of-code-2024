#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF		256
# define MAX_ERR_BUF_SIZE		256
# define MAX_NUM_STR_LEN		16
# define MAX_LINE_LEN			1024
# define MAX_LINE_NUM			512
# define MAX_MAP_WIDTH			64
# define MAX_MAP_HEIGHT			64
# define MAX_MOVES_LINE_LEN		32758
# define SLEEP_TIME             1
# define USLEEP_TIME            5*10E4

# define clear() printf("\033[H\033[J")

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		line_cnt;

	/* The length of the last
	 * line read from a file */
	int		line_len; 
	int		ch;
	int		i;
	FILE	*fptr;

	/* A map representing
	 * page ordering rules */
	char	map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	char	*moves;
	int		width; /* Actual width of the map */
	int		height; /* Actual height of the map */
	int		moves_line_cnt;
    int     sum_boxes_gps;
	int		moves_num; /* The total number of moves found */
	int		r_x; /* x position of the robot */
	int		r_y; /* y position of the robot */
    int     t_x; /* temp x */
    int     t_y;


	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		printf("%s\n", ebuf);
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

	moves = (char *)malloc(MAX_MOVES_LINE_LEN * sizeof (char));
	if (!moves)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	/* Let's read the map */
	i = 0;
	ch = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				map[line_cnt][i] = '\0';
				line_len = i;
				i = 0;
				if (line_cnt == 0)
					width = line_len;

				line_cnt++;

				if (line_len == 0) /* The map section ended */
					break;

				continue;
			}
			else
			{
				line_len = i;
				map[line_cnt][i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
		{
			if (ch == '@')
			{
				r_x = i;
				r_y = line_cnt;
			}
			map[line_cnt][i] = ch;
		}
		else
		{
			line_len = i;
			map[line_cnt][i] = '\0';
			break;
		}
		i++;
	}
	/* We substracting one because
	 * there is an empty line dividing
	 * the map section and moves */
	height = line_cnt - 1; 

	moves[0] = '\0';
	
	/* Let's read the moves */
	i = 0;
	ch = 0;
	moves_line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				line_len = i;
				i = 0;
				ft_strlcat(moves, line, MAX_MOVES_LINE_LEN);
				moves_line_cnt++;
				continue;
			}
			else
			{
				line_len = i;
				line[i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			line[i] = ch;
		else
		{
			line_len = i;
			line[i] = '\0';
			break;
		}
		i++;
	}

	moves_num = ft_strlen(moves);

	for (int mi = 0; mi < moves_num; mi++)
	{
		/* Moving up */
		if (moves[mi] == '^')
		{
			/* We are within the map boundaries, and the
			 * tile we are stepping onto is not a wall */
			if (map[r_y - 1][r_x] != '#' && r_y - 1 > 0)
			{
				/* We have a box on top of us */
				if (map[r_y - 1][r_x] == 'O')
				{
                    t_y = r_y - 1;
                    while (map[t_y][r_x] == 'O' && t_y > 0)
                        t_y--;
                    /* So the 'O' sequence starts at r_y - 1
                     * and ends at t_y */
                    /* Now we need to shift all the 'O' found */
                    int f = 0;
                    if (t_y > 0)
                    {
                        for (int i = r_y - 1; i > t_y; i--)
                        {
                            if (map[i - 1][r_x] == '#')
                            {
                                f = 1;
                                break;
                            }
                            map[i - 1][r_x] = map[i][r_x];
                        }
                        if (!f)
                        {
                            map[r_y][r_x] = '.';
                            map[r_y - 1][r_x] = '@';
                            r_y--;
                        }
                    }
                    /* If there was not any shift we
                     * don't need to move the robot */
				}
				else /* It's just an empty tile (a dot symbol) */
				{
					map[r_y][r_x] = '.';
					map[r_y - 1][r_x] = '@';
					r_y--;
				}
			} // if (map[r_y - 1][r_x] != '#' && r_y - 1 > 0)

		} // if (moves[mi] == '^')

		else if (moves[mi] == '>')
		{
			/* We are within the map boundaries, and the
			 * tile we are stepping onto is not a wall */
			if (map[r_y][r_x + 1] != '#' && r_x + 1 < width - 1)
			{
				/* We have a box to our right */
				if (map[r_y][r_x + 1] == 'O')
				{
                    t_x = r_x + 1;
                    while (map[r_y][t_x] == 'O' && t_x < width - 1)
                        t_x++;
                    /* So the 'O' sequence starts at r_x + 1 and
                     * ends at t_x */
                    /* Now we need to shift all the 'O' found */
                    int f = 0;
                    if (t_x < width - 1)
                    {
                        for (int i = r_x + 1; i < t_x; i++)
                        {
                            if (map[r_y][i + 1] == '#')
                            {
                                f = 1;
                                break;
                            }
                            map[r_y][i + 1] = map[r_y][i];
                        }

                        if (!f)
                        {
                            map[r_y][r_x] = '.';
                            map[r_y][r_x + 1] = '@';
                            r_x++;
                        }
                    }
                    /* If there was not any shift we
                     * don't need to move the robot */
				}
				else /* It's just an empty tile (a dot symbol) */
				{
					map[r_y][r_x] = '.';
					map[r_y][r_x + 1] = '@';
					r_x++;
				}
			} // if (map[r_y][r_x + 1] != '#' && r_x + 1 < width - 1)

		} // else if (moves[mi] == '>')

		else if (moves[mi] == 'v')
		{
			if (map[r_y + 1][r_x] != '#' && r_y + 1 < height - 1)
			{
				if (map[r_y + 1][r_x] == 'O')
				{
                    t_y = r_y + 1;
                    while (map[t_y][r_x] == 'O' && t_y < height - 1)
                        t_y++;

                    int f = 0;
                    if (t_y < height - 1)
                    {
                        for (int i = r_y + 1; i < t_y; i++)
                        {
                            if (map[i + 1][r_x] == '#')
                            {
                                f = 1;
                                break;
                            }
                            map[i + 1][r_x] = map[i][r_x];
                        }

                        if (!f)
                        {
                            map[r_y][r_x] = '.';
                            map[r_y + 1][r_x] = '@';
                            r_y++;
                        }
                    }
				}
				else
				{
					map[r_y][r_x] = '.';
					map[r_y + 1][r_x] = '@';
					r_y++;
				}
			} // if (map[r_y + 1][r_x] != '#' && r_y + 1 < height - 1)

		} //else if (moves[mi] == 'v')

		else if (moves[mi] == '<')
		{
			if (map[r_y][r_x - 1] != '#' && r_x - 1 > 0)
			{
				if (map[r_y][r_x - 1] == 'O')
				{
                    t_x = r_x - 1;
                    while (map[r_y][t_x] == 'O' && t_x > 0)
                        t_x--;

                    int f = 0;
                    if (t_x > 0)
                    {
                        for (int i = r_x - 1; i > t_x; i--)
                        {
                            if (map[r_y][i - 1] == '#')
                            {
                                f = 1;
                                break;
                            }
                            map[r_y][i - 1] = map[r_y][i];
                        }

                        if (!f)
                        {
                            map[r_y][r_x] = '.';
                            map[r_y][r_x - 1] = '@';
                            r_x--;
                        }
                    }
				}
				else
				{
					map[r_y][r_x] = '.';
					map[r_y][r_x - 1] = '@';
					r_x--;
				}
			} // if (map[r_y][r_x - 1] != '#' && r_x - 1 > 0)

		} // else if (moves[mi] == '<')

	} // for (int mi = 0; mi < moves_num; mi++)

    sum_boxes_gps = 0;
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            if (map[yi][xi] == 'O')
                sum_boxes_gps += (yi * 100) + xi;
        }
    }

    printf("sum of all boxes' GPS coordinates is %d\n", sum_boxes_gps);

	free(moves);
	fclose(fptr);
	exit (EXIT_SUCCESS);
}
