/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:59:14 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/18 02:17:40 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef enum
{
    NORTH, EAST, SOUTH, WEST
}   t_direction;

typedef struct  s_tile
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

    /* If turns_num is 0 we found an isolated vertex
     * (there is no any edge connected to it) */

    /* If turns_num is 2 and is_fork is unset
     * it means this tile is a part of an edge */

    /* If turns_num is 2 and is_fork is set it
     * means this tile is a vertex with a degree
     * equal 2 (two edges are connected to it) */

}	t_tile;

typedef struct  s_vertex
{
    int number;

	/* The coordinates of the tile in the map
	 * matrix corresponding to that vertex */
	int	x;
	int	y;

    /* In what directions from
     * this vertex we can go */
    int up;
    int right;
    int down;
    int left;

    /* The number of vertex
     * that is located on top */
    int up_v_num;
    int right_v_num;
    int down_v_num;
    int left_v_num;

	/* The degree of this vertex is
     * a number between 2 and 4 */
	int	degree;

}	t_vertex;

typedef struct  s_pair
{
    int v_num;
    int dist;

}   t_pair;

int	main(int argc, char *argv[])
{
	char        filename[MAX_FILE_NAME_BUF + 1];
	char        ebuf[MAX_ERR_BUF_SIZE + 1];
	int	        line_cnt;

	/* The length of the last
	 * line read from a file */
	int	        line_len; 
	int	        ch;
	int	        i;
	FILE        *fptr;

	/* A map representing the maze */
	char        map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	int	        width;		/* Actual width of the map */
	int	        height;		/* Actual height of the map */

	t_tile      *t;			/* All tiles found on the map */
	int	        tile_num;	/* Number of tiles on the map */

    /* x coordinate of the
     * tile where the Reindeer
     * starts its route */
	int	        start_x;
	int	        start_y;

    /* x coordinate of the
     * tile where the Reindeer
     * ends its route */
	int	        end_x;
	int	        end_y;      

    /* The initial direction
     * of the Reindeer */
    //t_direction direct;

    t_vertex    *v;	
	int         vert_num;
    int         vert_was_found;


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
			if (ch == 'E')
			{
				end_x = i;
				end_y = line_cnt;
				ch = '.'; /* to simplify the following code */
			}
			if (ch == 'S')
			{
				start_x = i;
				start_y = line_cnt;
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

	printf("start point is on (%d, %d)\n", start_x, start_y);
	printf("finish point is on (%d, %d)\n", end_x, end_y);

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

    /* Count the number of directions in
     * which we can go from each vertex.
     * Also we count the number of tiles */
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
		{
			if (map[yi][xi] == '.')
			{
				t[tile_num].x = xi;
				t[tile_num].y = yi;

                /* Looking up */
				if (yi - 1 > 0 && map[yi - 1][xi] == '.')
					t[tile_num].up++;
                /* Looking right */
				if (xi + 1 < width - 1 && map[yi][xi + 1] == '.')
					t[tile_num].right++;
                /* Looking down */
				if (yi + 1 < height - 1 && map[yi + 1][xi] == '.')
					t[tile_num].down++;
                /* Looking left */
				if (xi - 1 > 0 && map[yi][xi - 1] == '.')
					t[tile_num].left++;

				tile_num++;
			}
		}
    }

    vert_num = 0;
    for (int ti = 0; ti < tile_num; ti++)
    {
        /* Supposing the current
         * tile is not a vertex */
        vert_was_found = 0;

        /* First we are trying to detect the turn points (forks) */

        /* Checking if this is two edges fork */
		if ((t[ti].up && (t[ti].left || t[ti].right)) ||
            (t[ti].down && (t[ti].left || t[ti].right)))
		{
            vert_was_found = 1;
			t[ti].is_fork = 1; /* This tile (point) becomes a vertex */
			t[ti].turns_num = 2; /* The degree of this vertex increases by two */
		}

        /* Checking if this is three edges fork */
        if ((t[ti].up && t[ti].left && t[ti].right) ||
            (t[ti].down && t[ti].left && t[ti].right))
        {
            vert_was_found = 1;
			t[ti].is_fork = 1;
			t[ti].turns_num = 3;
        }

        /* Checking if this is four edges fork */
        if (t[ti].up && t[ti].left && t[ti].down && t[ti].right)
        {
            vert_was_found = 1;
			t[ti].is_fork = 1;
			t[ti].turns_num = 4;
        }

        /* Checking if this vertex has only one edge (dead end) */

        /* Now we try to detect if this point is a dead end */
        if ((t[ti].up && !t[ti].down && !t[ti].left && !t[ti].right) ||
            (t[ti].down && !t[ti].up && !t[ti].left && !t[ti].right) || 
            (t[ti].left && !t[ti].up && !t[ti].down && !t[ti].right) ||
            (t[ti].right && !t[ti].up && !t[ti].down && !t[ti].left))
        {
            vert_was_found = 1;
			t[ti].is_fork = 1;
			t[ti].turns_num = 1;;
        }

        if (vert_was_found)
            vert_num++;
	}

    printf("We got %d vertices\n", vert_num);

    v = (t_vertex *)malloc(vert_num * sizeof (t_vertex));
	if (!v)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
        free(t);
		exit(EXIT_FAILURE);
	}

	/* Let's copy all the vertices from tiles'
	 * array into the new vertices array */
	for (int ti = 0, vi = 0; ti < tile_num; ti++)
	{
        if (t[ti].is_fork)
        {
            v[vi].number = vi;
            v[vi].degree = t[ti].turns_num;

            v[vi].x = t[ti].x;
            v[vi].y = t[ti].y;

            v[vi].up = t[ti].up;
            v[vi].right = t[ti].right;
            v[vi].down = t[ti].down;
            v[vi].left = t[ti].left;

            v[vi].up_v_num = -1;
            v[vi].right_v_num = -1;
            v[vi].down_v_num = -1;
            v[vi].left_v_num = -1;

            vi++;
        }
	}

    /* he distances between v[vi]
     * and all the other vertices
     * that may be connected with it */
    t_pair  dists[1000];

    /* A counter we use to add
     * distances to dists[] */
    int dist_cnt;

    for (int vi = 0; vi < vert_num; vi++)
    {
        /* It's possible
         * to go up */ 
        if (v[vi].up)
        {
            /* We should find the nearest
             * vertex to v[vi] by walking
             * upward along the y axis */
            dist_cnt = 0;
            for (int vi2 = 0; vi2 < vert_num; vi2++)
            {
                /* If this vertex is on the same x-coordinate as v[vi]
                 * and is located above v[vi] along the y-axis */
                if ((vi != vi2) && (v[vi].x == v[vi2].x) && (v[vi].y > v[vi2].y))
                {
                    /* Let's measure the distance from v[vi] to v[vi2], but only
                     * if these two vertices are not separated by a wall '#' */
                    int yi = v[vi].y;
                    int wall_found = 0;
                    while (yi > v[vi2].y)
                    {
                        /* A wall was found */
                        if (map[yi][ v[vi].x ] == '#')
                        {
                            wall_found = 1;
                            break;
                        }
                        yi--;
                    }
                    /* This vertex can not
                     * be connected to v[vi]
                     * by a common edge */
                    if (wall_found)
                    {
                        /* Let's proceed to check
                         * the next vertex */
                        continue;
                    }

                    /* No wall was found separating
                     * v[vi] and v[vi2] vertices */

                    /* So let's measure the distance
                     * between v[vi] and v[vi2] and
                     * put this distance in the
                     * distances array */
                    dists[dist_cnt].v_num = v[vi2].number;
                    dists[dist_cnt].dist = v[vi].y - v[vi2].y;
                    dist_cnt++;


                } // if ((vi != vi2) && (v[vi].x == v[vi2].x) && (v[vi].y > v[vi2].y))

            } // for (int vi2 = 0; vi2 < vert_num; vi2++)

            /* If at least one
             * vertex was found */
            if (dist_cnt > 0)
            {
                /* Let's find the minimal
                 * distance in the dists[] */
                t_pair  *min_dist;

                min_dist = (t_pair *)&dists[0];
                for (int di = 1; di < dist_cnt; di++)
                {
                    if (dists[di].dist < min_dist->dist)
                        min_dist = (t_pair *)&dists[di];
                }
                v[vi].up_v_num = min_dist->v_num;
            }

        } // if (v[vi].up)

        if (v[vi].right)
        {
            dist_cnt = 0;
            for (int vi2 = 0; vi2 < vert_num; vi2++)
            {
                if ((vi != vi2) && (v[vi].y == v[vi2].y) && (v[vi].x < v[vi2].x))
                {
                    int xi = v[vi].x;
                    int wall_found = 0;
                    while (xi < v[vi2].x)
                    {
                        if (map[ v[vi].y ][xi] == '#')
                        {
                            wall_found = 1;
                            break;
                        }
                        xi++;
                    }
                    if (wall_found)
                        continue;

                    dists[dist_cnt].v_num = v[vi2].number;
                    dists[dist_cnt].dist = v[vi2].y - v[vi].y;
                    dist_cnt++;
                }
            }

            if (dist_cnt > 0)
            {
                t_pair  *min_dist;

                min_dist = (t_pair *)&dists[0];
                for (int di = 1; di < dist_cnt; di++)
                {
                    if (dists[di].dist < min_dist->dist)
                        min_dist = (t_pair *)&dists[di];
                }
                v[vi].right_v_num = min_dist->v_num;
            }

        } // if (v[vi].right)

        if (v[vi].down)
        {
            dist_cnt = 0;
            for (int vi2 = 0; vi2 < vert_num; vi2++)
            {
                if ((vi != vi2) && (v[vi].x == v[vi2].x) && (v[vi].y < v[vi2].y))
                {
                    int yi = v[vi].y;
                    int wall_found = 0;
                    while (yi < v[vi2].y)
                    {
                        if (map[yi][ v[vi].x ] == '#')
                        {
                            wall_found = 1;
                            break;
                        }
                        yi++;
                    }
                    if (wall_found)
                        continue;

                    dists[dist_cnt].v_num = v[vi2].number;
                    dists[dist_cnt].dist = v[vi2].y - v[vi].y;
                    dist_cnt++;

                }

            } // for (int vi2 = 0; vi2 < vert_num; vi2++)

            if (dist_cnt > 0)
            {
                t_pair  *min_dist;

                min_dist = (t_pair *)&dists[0];
                for (int di = 1; di < dist_cnt; di++)
                {
                    if (dists[di].dist < min_dist->dist)
                        min_dist = (t_pair *)&dists[di];
                }
                v[vi].down_v_num = min_dist->v_num;
            }

        } // if (v[vi].down)

        if (v[vi].left)
        {
            dist_cnt = 0;
            for (int vi2 = 0; vi2 < vert_num; vi2++)
            {
                if ((vi != vi2) && (v[vi].y == v[vi2].y) && (v[vi].x > v[vi2].x))
                {
                    int xi = v[vi].x;
                    int wall_found = 0;
                    while (xi > v[vi2].x)
                    {
                        if (map[ v[vi].y ][xi] == '#')
                        {
                            wall_found = 1;
                            break;
                        }
                        xi--;
                    }
                    if (wall_found)
                        continue;

                    dists[dist_cnt].v_num = v[vi2].number;
                    dists[dist_cnt].dist = v[vi].y - v[vi2].y;
                    dist_cnt++;
                }
            }

            if (dist_cnt > 0)
            {
                t_pair  *min_dist;

                min_dist = (t_pair *)&dists[0];
                for (int di = 1; di < dist_cnt; di++)
                {
                    if (dists[di].dist < min_dist->dist)
                        min_dist = (t_pair *)&dists[di];
                }
                v[vi].left_v_num = min_dist->v_num;
            }

        } // if (v[vi].left)

    } // for (int vi = 0; vi < vert_num; vi++)


	/* Let's print out all found vertices */
	for (int vi = 0; vi < vert_num; vi++)
    {
        printf("\nvertex %d\n", vi);
        printf("\tdegree: %d\n", v[vi].degree);
        printf("\tx: %d\n", v[vi].x);
        printf("\ty: %d\n", v[vi].y);

        if (v[vi].up_v_num != -1)
            printf("\tup: %d\n", v[vi].up_v_num);
        if (v[vi].right_v_num != -1)
            printf("\tright: %d\n", v[vi].right_v_num);
        if (v[vi].down_v_num != -1)
            printf("\tdown: %d\n", v[vi].down_v_num);
        if (v[vi].left_v_num != -1)
            printf("\tleft: %d\n", v[vi].left_v_num);
    }


    /* Set up the initial direction
     * of the Reindeer */
    //direct = EAST;




    free(v);
	free(t);
	fclose(fptr);
	exit (EXIT_SUCCESS);
}
