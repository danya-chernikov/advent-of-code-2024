/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:59:14 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/21 00:38:49 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define MAX_LINE_LEN		1024
# define MAX_LINE_NUM		512
# define MAX_MAP_WIDTH		256
# define MAX_MAP_HEIGHT		256
# define QUEUE_CAPACITY		1024
# define VERT_EDGES_NUM		4
# define SLEEP_TIME         1
# define USLEEP_TIME        5*1e5
# define INF				1e9
# define NONE				-1

# define clear() printf("\033[H\033[J")

typedef enum
{
    NORTH, EAST, SOUTH, WEST
}   t_direction;

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

/* If `turns_num` is 0 we found an isolated vertex
 * (there is no any edge connected to it).
 * If `turns_num` is 2 and is_fork is unset
 * it means this tile is a part of an edge.
 * If `turns_num` is 2 and is_fork is set it
 * means this tile is a vertex with a degree
 * equal 2 (two edges are connected to it) */
typedef struct  s_tile
{
	t_point	coord;

	bool	up;		/* If 1, it's possible to go up from this tile */
	bool	right;	/* If 1, it's possible to go right */
	bool	down;	/* If 1, it's possible to go down */
	bool	left;	/* If 1, it's possible to go left */

	/* Is this tile (point) a fork?
	 * If it is, this tile is a vertex */
	bool	is_fork; 

	/* If fact, it's the degree
	 * of this vertex assuming
	 * the `is_fork` flag is set */
	int		turns_num;

}	t_tile;

typedef struct  s_vertex
{
    int		number;

	/* The coordinates of the tile in the map
	 * matrix corresponding to that vertex */
	t_point	coord;

    /* In what directions from
     * this vertex we can go */
    bool	up;
    bool	right;
    bool	down;
    bool	left;

	/* The number of the nearest
	 * vertex located above and
	 * the distance to it */
    t_pair	v_up;
    t_pair	v_right;
    t_pair	v_down;
    t_pair	v_left;

	/* The degree of this vertex */
	int		degree;

}	t_vertex;

void    queue_init(t_queue *q);
bool    is_queue_empty(t_queue *q);
bool    is_queue_full(t_queue *q);
void    enqueue(t_queue *q, int value);
void    dequeue(t_queue *q);
int     queue_get_front(t_queue *q);
void    print_queue(t_queue *q);

int		read_map(char *filename,
				 char (*map)[MAX_MAP_HEIGHT],
				 int *width,
				 int *height,
				 t_point *start,
				 t_point *end);

void	print_map(char (*map)[MAX_MAP_HEIGHT],
				  int width,
				  int hegiht,
				  t_point *start,
				  t_point *end);

int		find_tiles(t_tile *t,
				   char (*map)[MAX_MAP_HEIGHT],
				   int width,
				   int height);

int		find_vertices(t_tile *t, int tile_num);
void	init_vertices(t_vertex *v, t_tile *t, int tile_num);
void	build_graph(char (*map)[MAX_MAP_HEIGHT], t_vertex *v, int vert_num);
void	print_graph(t_vertex *v, int vert_num);
int     *bfs(t_vertex *graph, int *from, int vert_num, int start);
void    reverse_array(int *arr, int size);
int     *get_path(int *from, int vert_num, int finish);

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
 *				 vertices of the graph `v`;
 * */
int	main(int argc, char *argv[])
{
	char        map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	char        filename[MAX_FILE_NAME_BUF + 1];
	char        ebuf[MAX_ERR_BUF_SIZE + 1];
	int	        width;
	int	        height;
	t_tile      *tiles;
	int	        tile_num;
	t_point		start;
	t_point		end;
    int         start_v_num;
    int         end_v_num;
    t_vertex    *v;	
	int         vert_num;
	int			*dist;
    int         *from;
    int         *path;

    /* The initial direction
     * of the Reindeer */
    //t_direction direct;

	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		printf("%s\n", ebuf);
		exit(EXIT_FAILURE);
	}
	else
	{
		strncpy(filename, argv[1], MAX_FILE_NAME_BUF);
	}

	if (!read_map(filename, map, &width, &height, &start, &end))
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[1]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	print_map(map, width, height, &start, &end);

	tiles = (t_tile *)malloc((width * height) * sizeof (t_tile));
	if (!tiles)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	tile_num = find_tiles(tiles, map, width, height);

	vert_num = find_vertices(tiles, tile_num);

    printf("We got %d vertices\n", vert_num);

    v = (t_vertex *)malloc(vert_num * sizeof (t_vertex));
	if (!v)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
        free(tiles);
		exit(EXIT_FAILURE);
	}

	init_vertices(v, tiles, tile_num);
	
	/* We no longer need
	 * the array of tiles */
	free(tiles);

	build_graph(map, v, vert_num);

	print_graph(v, vert_num);

    /* Let's determine the vertives numbers corresponding
     * to the start and end tiles */
    for (int vi = 0; vi < vert_num; vi++)
    {
        if (v[vi].coord.x == start.x && v[vi].coord.y == start.y)
            start_v_num = vi;
        if (v[vi].coord.x == end.x && v[vi].coord.y == end.y)
            end_v_num = vi;
    }

    printf("Start vertex number is: %d\n", start_v_num);
    printf("End vertex number is: %d\n", end_v_num);

    if ( !(from = (int *)malloc( vert_num * sizeof (int) )) )
    {
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
        free(v);
		exit(EXIT_FAILURE);
    }

	/* Finally, let's find the shortest path in the
     * undirected, unweighted graph represented by `v` */
    dist = bfs(v, from, vert_num, start_v_num);
    if (!dist)
    {
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
        free(v);
		exit(EXIT_FAILURE);
    }

    path = get_path(from, vert_num, end_v_num);
    if (!path)
    {
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
        free(v);
		exit(EXIT_FAILURE);
    }

    /* Set up the initial direction
     * of the Reindeer */

    //direct = EAST;

    printf("\n");
    for (int di = 0; di < vert_num; di++)
        printf("%d : %d\n", di, dist[di]);
    printf("\n");

    printf("\n");
    for (int vi = 0; path[vi] != -1; vi++)
        printf("%d ", path[vi]);
    printf("\n");

    free(v);
    free(path);
    free(dist);
    free(from);
	exit (EXIT_SUCCESS);
}

void    queue_init(t_queue *q)
{
    q->front = -1;
    q->rear = 0;
}

bool    is_queue_empty(t_queue *q)
{
    return (q->front == q->rear - 1);
}

bool    is_queue_full(t_queue *q)
{
    return (q->rear == QUEUE_CAPACITY);
}

void    queue_push(t_queue *q, int value)
{
    if (is_queue_full(q))
        return;
    q->items[q->rear] = value;
    q->rear++;
}

void    queue_pop(t_queue *q)
{
    if (is_queue_empty(q))
        return;
    q->front++;
}

int     queue_get_front(t_queue *q)
{
    if (is_queue_empty(q))
        return (-1);
    return (q->items[q->front + 1]);
}

void    print_queue(t_queue *q)
{
    if (is_queue_empty(q))
        return;
    for (int i = q->front + 1; i < q->rear; i++)
        printf("%d ", q->items[i]);
    printf("\n");
}

/* It reads a map from the file with
 * the name `filename`.
 *     line_len - The length of the last
 *				  line read from a file.
 *
 * In case of finding S or E tile we
 * assign '.' to its point to simplify
 * the following code */
int		read_map(char *filename,
				 char (*map)[MAX_MAP_HEIGHT],
				 int *width,
				 int *height,
				 t_point *start,
				 t_point *end)
{
	int	        line_cnt;
	int	        line_len; 
	FILE        *fptr;
	int	        ch;
	int	        i;

	fptr = fopen(filename, "r");
	if (fptr == NULL)
		return (0);

	i = 0;
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
				end->x = i;
				end->y = line_cnt;
				ch = '.';
			}
			if (ch == 'S')
			{
				start->x = i;
				start->y = line_cnt;
				ch = '.';
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
	*width = line_len;
	*height = line_cnt;
	fclose(fptr);
	return (1);
}

void	print_map(char (*map)[MAX_MAP_HEIGHT],
				  int width,
				  int height,
				  t_point *start,
				  t_point *end)
{
	printf("start point is on (%d, %d)\n", start->x, start->y);
	printf("end point is on (%d, %d)\n", end->x, end->y);

	printf("width = %d\nheight = %d\n", width, height);

	printf("\n");
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
			printf("%c", map[yi][xi]);
		printf("\n");
	}
	printf("\n");
}

/* Count the number of directions in
 * which we can go from each vertex.
 * Also we count the number of tiles
 * the map contains */
int		find_tiles(t_tile *t,
				   char (*map)[MAX_MAP_HEIGHT],
				   int width,
				   int height)
{
	int	tile_num;

	for (int ti = 0; ti < width * height; ti++)
	{
		t[ti].is_fork = false;
		t[ti].up      = false;
		t[ti].right   = false;
		t[ti].down    = false;
		t[ti].left    = false;
	}

	tile_num = 0;
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
		{
			if (map[yi][xi] == '.')
			{
				t[tile_num].coord.x = xi;
				t[tile_num].coord.y = yi;

                /* Looking up */
				if (yi - 1 > 0 && map[yi - 1][xi] == '.')
					t[tile_num].up = true;
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
		}
    }
	return (tile_num);
}

/* Let's find all the vertices among the tiles
 * and determine each vertex degree (turns_num) */
int		find_vertices(t_tile *t, int tile_num)
{
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
}

/* Let's copy all the vertices from tiles' array into the new vertices array */
void	init_vertices(t_vertex *v, t_tile *t, int tile_num)
{
	for (int ti = 0, vi = 0; ti < tile_num; ti++)
	{
		if (t[ti].is_fork)
		{
			v[vi].number  = vi;
			v[vi].degree  = t[ti].turns_num;
			v[vi].coord.x = t[ti].coord.x;
			v[vi].coord.y = t[ti].coord.y;
			v[vi].up      = t[ti].up;
			v[vi].right   = t[ti].right;
			v[vi].down    = t[ti].down;
			v[vi].left    = t[ti].left;

			v[vi].v_up.v_num    = NONE;
			v[vi].v_up.dist     = NONE;
			v[vi].v_right.v_num = NONE;
			v[vi].v_right.dist  = NONE;
			v[vi].v_down.v_num  = NONE;
			v[vi].v_down.dist   = NONE;
			v[vi].v_left.v_num  = NONE;
			v[vi].v_left.dist   = NONE;
			vi++;
		}
	}
}

/* Let's find all incident vertices for each
 * vertex in the graph represented by `v`.
 *     dists    - The distances between v[vi]
 *		          and all the other vertices
 *		          that may be connected with it;
 *     dist_cnt - A counter we use to add
 *				  distances to dists[]. */
void	build_graph(char (*map)[MAX_MAP_HEIGHT], t_vertex *v, int vert_num)
{
	t_pair  dists[VERT_EDGES_NUM];
	t_pair	*min_dist;
	bool	wall_found;
	int		dist_cnt;
	int		xi;
	int		yi;

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
				if ((vi != vi2) &&
					(v[vi].coord.x == v[vi2].coord.x) &&
					(v[vi].coord.y > v[vi2].coord.y))
				{
					/* Let's measure the distance from v[vi] to v[vi2], but only
					 * if these two vertices are not separated by a wall '#' */
					yi = v[vi].coord.y;
					wall_found = false;
					while (yi > v[vi2].coord.y)
					{
						/* A wall was found */
						if (map[yi][ v[vi].coord.x ] == '#')
						{
							wall_found = true;
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
					dists[dist_cnt].dist = v[vi].coord.y - v[vi2].coord.y;
					dist_cnt++;

				} // if ((vi != vi2) && (v[vi].x == v[vi2].x) && (v[vi].y > v[vi2].y))

			} // for (int vi2 = 0; vi2 < vert_num; vi2++)

			/* If at least one
			 * vertex was found */
			if (dist_cnt > 0)
			{
				/* Let's find the minimal
				 * distance in the dists[] */
				min_dist = (t_pair *)&dists[0];
				for (int di = 1; di < dist_cnt; di++)
				{
					if (dists[di].dist < min_dist->dist)
						min_dist = (t_pair *)&dists[di];
				}
				v[vi].v_up.v_num = min_dist->v_num;
				v[vi].v_up.dist = min_dist->dist;
			}

		} // if (v[vi].up)

		if (v[vi].right)
		{
			dist_cnt = 0;
			for (int vi2 = 0; vi2 < vert_num; vi2++)
			{
				if ((vi != vi2) &&
					(v[vi].coord.y == v[vi2].coord.y) &&
					(v[vi].coord.x < v[vi2].coord.x))
				{
					xi = v[vi].coord.x;
					wall_found = false;
					while (xi < v[vi2].coord.x)
					{
						if (map[ v[vi].coord.y ][xi] == '#')
						{
							wall_found = true;
							break;
						}
						xi++;
					}
					if (wall_found)
						continue;

					dists[dist_cnt].v_num = v[vi2].number;
					dists[dist_cnt].dist = v[vi2].coord.x - v[vi].coord.x;
					dist_cnt++;
				}
			}

			if (dist_cnt > 0)
			{
				min_dist = (t_pair *)&dists[0];
				for (int di = 1; di < dist_cnt; di++)
				{
					if (dists[di].dist < min_dist->dist)
						min_dist = (t_pair *)&dists[di];
				}
				v[vi].v_right.v_num = min_dist->v_num;
				v[vi].v_right.dist = min_dist->dist;
			}

		} // if (v[vi].right)

		if (v[vi].down)
		{
			dist_cnt = 0;
			for (int vi2 = 0; vi2 < vert_num; vi2++)
			{
				if ((vi != vi2) &&
					(v[vi].coord.x == v[vi2].coord.x) &&
					(v[vi].coord.y < v[vi2].coord.y))
				{
					yi = v[vi].coord.y;
					wall_found = 0;
					while (yi < v[vi2].coord.y)
					{
						if (map[yi][ v[vi].coord.x ] == '#')
						{
							wall_found = true;
							break;
						}
						yi++;
					}
					if (wall_found)
						continue;

					dists[dist_cnt].v_num = v[vi2].number;
					dists[dist_cnt].dist = v[vi2].coord.y - v[vi].coord.y;
					dist_cnt++;
				}

			} // for (int vi2 = 0; vi2 < vert_num; vi2++)

			if (dist_cnt > 0)
			{
				min_dist = (t_pair *)&dists[0];
				for (int di = 1; di < dist_cnt; di++)
				{
					if (dists[di].dist < min_dist->dist)
						min_dist = (t_pair *)&dists[di];
				}
				v[vi].v_down.v_num = min_dist->v_num;
				v[vi].v_down.dist = min_dist->dist;
			}

		} // if (v[vi].down)

		if (v[vi].left)
		{
			dist_cnt = 0;
			for (int vi2 = 0; vi2 < vert_num; vi2++)
			{
				if ((vi != vi2) &&
					(v[vi].coord.y == v[vi2].coord.y) &&
					(v[vi].coord.x > v[vi2].coord.x))
				{
					xi = v[vi].coord.x;
					wall_found = false;
					while (xi > v[vi2].coord.x)
					{
						if (map[ v[vi].coord.y ][xi] == '#')
						{
							wall_found = true;
							break;
						}
						xi--;
					}
					if (wall_found)
						continue;

					dists[dist_cnt].v_num = v[vi2].number;
					dists[dist_cnt].dist = v[vi].coord.x - v[vi2].coord.x;
					dist_cnt++;
				}
			}

			if (dist_cnt > 0)
			{
				min_dist = (t_pair *)&dists[0];
				for (int di = 1; di < dist_cnt; di++)
				{
					if (dists[di].dist < min_dist->dist)
						min_dist = (t_pair *)&dists[di];
				}
				v[vi].v_left.v_num = min_dist->v_num;
				v[vi].v_left.dist = min_dist->dist;
			}

		} // if (v[vi].left)

	} // for (int vi = 0; vi < vert_num; vi++)
}

void	print_graph(t_vertex *v, int vert_num)
{
	/* Let's print out all found vertices */
	for (int vi = 0; vi < vert_num; vi++)
	{
		printf("\nvertex %d\n", vi);
		printf("\tdegree: %d\n", v[vi].degree);
		printf("\tx: %d\n", v[vi].coord.x);
		printf("\ty: %d\n", v[vi].coord.y);

		if (v[vi].v_up.v_num != NONE)
			printf("\tup: %d\n", v[vi].v_up.v_num);
		if (v[vi].v_right.v_num != NONE)
			printf("\tright: %d\n", v[vi].v_right.v_num);
		if (v[vi].v_down.v_num != NONE)
			printf("\tdown: %d\n", v[vi].v_down.v_num);
		if (v[vi].v_left.v_num != NONE)
			printf("\tleft: %d\n", v[vi].v_left.v_num);
	}
	printf("\n");

	for (int vi = 0; vi < vert_num; vi++)
	{
		printf("%d: ", vi);
		if (v[vi].v_up.v_num != NONE)
		{
			printf("{%d, ", v[vi].v_up.v_num);
			printf("%d} ", v[vi].v_up.dist);
		}
		if (v[vi].v_right.v_num != NONE)
		{
			printf("{%d, ", v[vi].v_right.v_num);
			printf("%d} ", v[vi].v_right.dist);
		}
		if (v[vi].v_down.v_num != NONE)
		{
			printf("{%d, ", v[vi].v_down.v_num);
			printf("%d} ", v[vi].v_down.dist);
		}
		if (v[vi].v_left.v_num != NONE)
		{
			printf("{%d, ", v[vi].v_left.v_num);
			printf("%d} ", v[vi].v_left.dist);
		}
		printf("\n");
	}
	printf("\n");
}

/* This function finds the shortest path between start
 * vertex and all the other vertices of the `v` graph.
 *     dist - Array of distances from the start
 *			  vertex to all the other vertices
 *			  of the graph `v`;
 *     v    - Vertex's number got from queue; */
int     *bfs(t_vertex *graph, int *from, int vert_num, int start)
{
    int			*dist;
    int			to;
    int			v;
    t_queue     q;

    if ( !(dist = (int *)malloc( vert_num * sizeof (int) )) )
        return (NULL);

    for (int vi = 0; vi < vert_num; vi++)
    {
        dist[vi] = INF;
        from[vi] = -1;
    }
    dist[start] = 0;
    queue_init(&q);
    queue_push(&q, start);

    /* While queue is not empty */
    while (!is_queue_empty(&q))
    {
        v = queue_get_front(&q);
        queue_pop(&q);

        /* Let's look at all of this vertex neighbors */
        /* If the neighbor on the right exists */
        if (graph[v].v_up.v_num != NONE)
        {
            to = graph[v].v_up.v_num;
            if (dist[to] == INF)
            {
                dist[to] = dist[v] + 1;
                from[to] = v;
                queue_push(&q, to);
            }
        }
        if (graph[v].v_right.v_num != NONE)
        {
            to = graph[v].v_right.v_num;
            if (dist[to] == INF)
            {
                dist[to] = dist[v] + 1;
                from[to] = v;
                queue_push(&q, to);
            }
        }
        if (graph[v].v_down.v_num != NONE)
        {
            to = graph[v].v_down.v_num;
            if (dist[to] == INF)
            {
                dist[to] = dist[v] + 1;
                from[to] = v;
                queue_push(&q, to);
            }
        }
        if (graph[v].v_left.v_num != NONE)
        {
            to = graph[v].v_left.v_num;
            if (dist[to] == INF)
            {
                dist[to] = dist[v] + 1;
                from[to] = v;
                queue_push(&q, to);
            }
        }
    }
    return (dist);
}

void    reverse_array(int *arr, int size)
{
    int tmp;

    for (int i = 0; i < (size >> 1); i++)
    {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

int     *get_path(int *from, int vert_num, int finish)
{
    int *path;
    int path_cnt;

    path = (int *)malloc(vert_num * sizeof (int));
    if (!path)
        return (NULL);
    for (int i = 0; i < vert_num; i++)
        path[i] = -1;
    path_cnt = 0;
    for (int v = finish; v != -1; v = from[v])
    {
        path[path_cnt] = v;
        path_cnt++;
    }
    reverse_array(path, path_cnt);
    return (path);
}
