#include "libft.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

/* VERT_EDGES_NUM - the maximum number of edges
 *					coming from one vertex */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_LEN		8
# define MAX_LINE_NUM		4096
# define MAP_WIDTH			71
# define MAP_HEIGHT			71
# define QUEUE_CAPACITY		4096
# define VERT_EDGES_NUM		4096
# define KILOBYTE			1024
# define SLEEP_TIME         2
# define USLEEP_TIME        5*1e5
# define INF				1e9
# define NONE				-1

# define clear() printf("\033[H\033[J")

typedef struct  s_queue
{
    int items[QUEUE_CAPACITY];
    int front;
    int rear;

}   t_queue;

typedef struct  s_pair
{
    int v_num;
    int dist;

}   t_pair;

typedef struct	s_point
{
	int	x;
	int	y;
	
}	t_point;

typedef struct  s_vertex
{
    int		number;
	int		degree;
	t_point	coord;
    t_pair	v_up;
    t_pair	v_right;
    t_pair	v_down;
    t_pair	v_left;

}	t_vertex;

int		get_nums(char *line, int *nums, int arr_size);

int		read_bytes(char *filename,
				   char (*map)[MAP_HEIGHT]);


/* map         - A map representing the maze;
 * width       - Actual width of the map;
 * height      - Actual height of the map;
 * tiles       - All tiles found on the map;
 * tile_num    - Number of tiles on the map;
 * start       - Coordinates of the tile where
 *			     the Reindeer starts its route;
 * start_v_num - Start vertex number;
 *
 * end         - Coordinates of the tile where
 *				 the Reindeer ends its route;
 * end_v_num   - End vertex number;
 * dist		   - Array of distances from the
 *				 start vertex to all the other
 *				 vertices of the graph `v`. */
int	main(int argc, char *argv[])
{
	char        map[MAP_HEIGHT][MAP_WIDTH];
	char        filename[MAX_FILE_NAME_BUF + 1];
	char        ebuf[MAX_ERR_BUF_SIZE + 1];
	int			vert_num;
	t_point		start;
	t_point		end;
	t_vertex	*graph;

	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		printf("%s\n", ebuf);
		exit(EXIT_FAILURE);
	}
	else
	{
		ft_strlcpy(filename, argv[1], MAX_FILE_NAME_BUF);
	}

	start.x = 0;
	start.y = 0;
	end.x = 70;
	end.y = 70;

	if (!read_bytes(filename, map))
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[1]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	for (int yi = 0; yi < MAP_HEIGHT; yi++)
	{
		for (int xi = 0; xi < MAP_WIDTH; xi++)
			printf("%c", map[yi][xi]);
		printf("\n");
	}

	vert_num = 0;
	for (int yi = 0; yi < MAP_HEIGHT; yi++)
	{
		for (int xi = 0; xi < MAP_WIDTH; xi++)
		{
			if (map[yi][xi] == '.')
			{
				graph[vert_num].number = vert_num;
				graph[vert_num].coord.x = xi;
				graph[vert_num].coord.y = yi;
				vert_num++;
			}
		}
	}

	graph = (t_vertex *)malloc(vert_num * sizeof (t_vertex));
	if (!tiles)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

typedef struct  s_vertex
{
    int		number;
	int		degree;
	t_point	coord;
    t_pair	v_up;
    t_pair	v_right;
    t_pair	v_down;
    t_pair	v_left;

}	t_vertex;

	int	vert_cnt;
	int	cur_x;
	int	cur_y;

	vert_num = 0;
    for (int vi = 0; vi < vert_num; vi++)
    {
		cur_x = graph[vi].coord.x;
		cur_y = graph[vi].coord.y;

		/* Looking up */
		if (cur_y - 1 > 0 && map[cur_y - 1][cur_x] == '.')
			graph[vi].v_up = find_vert(cur_x, cur_y - 1);
		/* Looking right */
		if (xi + 1 < width - 1 && map[yi][xi + 1] == '.')
			t[tile_num].right = true;
		/* Looking down */
		if (yi + 1 < height - 1 && map[yi + 1][xi] == '.')
			t[tile_num].down = true;
		/* Looking left */
		if (xi - 1 > 0 && map[yi][xi - 1] == '.')
			t[tile_num].left = true;

		tile_num++;
    }
	return (tile_num);


	bool	vert_was_found;
	int		vert_num;

    vert_num = 0;
    for (int ti = 0; ti < tile_num; ti++)
    {
        /* Supposing the current
         * tile is not a vertex */
        vert_was_found = false;

        /* First we are trying to detect the turn points (forks) */

        /* Checking if this is two edges fork */
		if ((t[ti].up && (t[ti].left || t[ti].right)) ||
            (t[ti].down && (t[ti].left || t[ti].right)))
		{
            vert_was_found = true;
			t[ti].is_fork = true; /* This tile (point) becomes a vertex */
			t[ti].turns_num = 2; /* The degree of this vertex increases by two */
		}

        /* Checking if this is three edges fork */
        if ((t[ti].up && t[ti].left && t[ti].right) ||
            (t[ti].down && t[ti].left && t[ti].right))
        {
            vert_was_found = true;
			t[ti].is_fork = true;
			t[ti].turns_num = 3;
        }

        /* Checking if this is four edges fork */
        if (t[ti].up && t[ti].left && t[ti].down && t[ti].right)
        {
            vert_was_found = true;
			t[ti].is_fork = true;
			t[ti].turns_num = 4;
        }

        /* Checking if this vertex has only one edge (dead end) */

        /* Now we try to detect if this point is a dead end */
        if ((t[ti].up && !t[ti].down && !t[ti].left && !t[ti].right) ||
            (t[ti].down && !t[ti].up && !t[ti].left && !t[ti].right) || 
            (t[ti].left && !t[ti].up && !t[ti].down && !t[ti].right) ||
            (t[ti].right && !t[ti].up && !t[ti].down && !t[ti].left))
        {
            vert_was_found = true;
			t[ti].is_fork = true;
			t[ti].turns_num = 1;
        }

		/* And finally it's also a good practice
		 * to detect isolated vertices */
        if (!t[ti].up && !t[ti].down && !t[ti].left && !t[ti].right)
        {
            vert_was_found = true;
			t[ti].is_fork = true;
			t[ti].turns_num = 0;
        }

        if (vert_was_found)
            vert_num++;
	}
	return (vert_num);

	free(graph);
	exit (EXIT_SUCCESS);
}


/* It finds all the numbers in the line, divided by
 * any number of any symbols, then puts these numbers into the
 * nums array. It returns the number of found numbers.
 * If the number of numbers exceeds the size of the nums
 * array, defined as arr_size, the function returns -1. */
int		get_nums(char *line, int *nums, int arr_size)
{
	int		i;
	int		j;
	int		nums_cnt;
	char	num[MAX_NUM_STR_LEN + 1];

	i = 0;
	j = 0;
	nums_cnt = 0;
	while (1)
	{
		if (ft_isdigit(line[i]) || line[i] == '-')
		{
			num[j] = line[i];
			j++;
		}
		if (!ft_isdigit(line[i]) && (line[i] != '-'))
		{
			num[j] = '\0';
			nums[nums_cnt] = ft_atoi(num);
			nums_cnt++;
			j = 0;
			if (line[i] == '\0')
				break;
			while (!ft_isdigit(line[i]) && (line[i] != '-') && (line[i] != '\0'))
				i++;
			continue;
		}
		if (nums_cnt > arr_size)
			return (-1);
		i++;
	}
	return (nums_cnt);
}

/* It reads falling bytes from the file with
 * the name `filename` and creates a map
 * indicating on it all corrupted cells.
 *     line_len - The length of the last line read
 *     from a file;
 *     coords   - coordinates of the falling byte. */
int		read_bytes(char *filename, char (*map)[MAP_HEIGHT])
{
	char	line[MAX_LINE_LEN];
	int		coords[2];
	int		nums_found;
	int		line_cnt;
	int		line_len; 
	FILE	*fptr;
	int		ch;
	int		i;

	for (int yi = 0; yi < MAP_HEIGHT; yi++)
		for (int xi = 0; xi < MAP_WIDTH; xi++)
			map[yi][xi] = '.';

	fptr = fopen(filename, "r");
	if (fptr == NULL)
		return (0);

	i = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (line_cnt == KILOBYTE)
			break;
		if (ch == '\n')
		{
			if ((line_cnt < MAX_LINE_NUM))
			{
				line[i] = '\0';
				line_len = i;
				i = 0;

				nums_found = get_nums(line, coords, 2);
				if (nums_found != -1)
					map[ coords[1] ][ coords[0] ] = '#';

				line_cnt++;
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
		{
			line[i] = ch;
		}
		else
		{
			line_len = i;
			line[i] = '\0';
			break;
		}
		i++;
	}
	fclose(fptr);
	return (1);
}
