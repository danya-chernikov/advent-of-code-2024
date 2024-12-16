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
# define MAX_MAP_WIDTH			256
# define MAX_MAP_HEIGHT			256
# define SLEEP_TIME             1
# define USLEEP_TIME            5*10E4

# define clear() printf("\033[H\033[J")

typedef struct s_tile
{
	int	x;
	int	y;

	int	up;		/* If 1, it's possible to go up from this tile */
	int	right;	/* If 1, it's possible to go right */
	int	down;	/* If 1, it's possible to go down */
	int	left;	/* If 1, it's possible to go left */

	/* Is this tile (point) a fork?
	 * If it is, this tile is a vertex */
	int	is_fork; 

	/* If fact, it's the degree
	 * of this vertex assuming
	 * the `is_fork` flag is set */
	int	turns_num;

}	t_tile;

typedef struct staruct	s_vertex
{
	/* The coordinates of the tile in the map
	 * matrix corresponding to that vertex */
	int	x;
	int	y;

	/* The degree of this vertex is a number between 2 and 4 */
	int	degree;

}	t_vertex;

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

	/* A map representing the maze */
	char	map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	int		width;		/* Actual width of the map */
	int		height;		/* Actual height of the map */

	t_tile	*t;			/* All tiles found on the map */
	int		tile_num;	/* Number of tiles on the map */
	int		s_x;		/* x coordinate of the start */
	int		s_y;
	int		f_x;
	int		f_y;



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
				line_cnt++;
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
			if (ch == 'F')
			{
				f_x = i;
				f_y = line_cnt;
				ch = '.'; /* to simplify the following code */
			}
			if (ch == 'S')
			{
				s_x = i;
				s_y = line_cnt;
				ch = '.'; /* to simplify the following code */
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
	width = line_len;
	height = line_cnt;

	printf("width = %d\nheight = %d\n", width, height);

	printf("\n");
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
			printf("%c", map[yi][xi]);
		printf("\n");
    }
	printf("\n");

	t = (t_tile *)malloc((width * height) * sizeof (t_tile));
	if (!t)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	for (int ti = 0; ti < width * height; ti++)
	{
		t[ti].is_fork = 0;
		t[ti].up = 0;
		t[ti].right = 0;
		t[ti].down = 0;
		t[ti].left = 0;
	}

    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
		{
			if (map[yi][xi] == '.')
			{
				t[tile_num].x = xi;
				t[tile_num].y = yi;

				if (yi - 1 > 0 && map[yi - 1][xi] == '.')
					t[tile_num].up++;
				if (xi + 1 < width - 1 && map[yi][xi + 1] == '.')
					t[tile_num].right++;
				if (yi + 1 < height - 1 && map[yi + 1][xi] == '.')
					t[tile_num].down++;
				if (xi - 1 > 0 && map[yi][xi - 1] == '.')
					t[tile_num].left++;

				tile_num++;
			}
		}
    }

    for (int ti = 0; ti < tile_num; ti++)
    {
		/* We have a turn here! */
		if (map[ti].up && map[ti].left)
		{
			map[ti].is_fork = 1; /* This tile (point) becomes a vertex */
			map[ti].turns_num++; /* The degree of this vertex increases by one */
		}
		if (map[ti].up && map[ti].right)
		{
			map[ti].is_fork = 1;
			map[ti].turns_num++;
		}
		if (map[ti].down && map[ti].left)
		{
			map[ti].is_fork = 1;
			map[ti].turns_num++;
		}
		if (map[ti].down && map[ti].right)
		{
			map[ti].is_fork = 1;
			map[ti].turns_num++;
		}
	}

	
	int	vert_num = 0;

	/* Let's copy all the vertices from tiles'
	 * array into a new vertices array */
	for (int ti = 0; ti < tile_num; ti++)
	{

	}


	/* Let's print out all found vertices */

	free(t);
	fclose(fptr);
	exit (EXIT_SUCCESS);
}
