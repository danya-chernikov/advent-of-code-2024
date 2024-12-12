#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		256 /* It's map width in fact */
# define MAX_LINE_NUM		256 /* It's map height */
# define MAX_PLOTS_NUM		256*256 /* Map width multiplied by height */
# define MAX_GROUPS_NUM		256*256 /* In fact this number will be less */
# define SLEEP_TIME         0

typedef struct s_point
{
	int	X;
	int	Y;

}	t_point;

typedef struct s_plot
{
	t_point	coord; /* Coordinates of this plot */
	t_group	*group; /* Pointer to the group to which this plot belongs */
	char	plant;
	int		turns_num; /* The number of turns that have been performed in this plot */
	int		edges_num;

}	t_plot;

typedef struct s_group
{
	int		group_id;
	char	plant;
	t_plot	*plots[MAX_PLOTS_NUM]; /* Pointers to the found plots */

}	t_group;


/* FOR DEBUGGING PURPOSES */
/* Points of the map containing plots
 * that have already been visited */
char    vp[MAX_LINE_NUM][MAX_LINE_LEN]; /* vp - visited points (plots) */

/* How many times
 * the recursive
 * function was called */
int     call_cnt; 
/* FOR DEBUGGING PURPOSES */

/* If m[y-1][x] contains plot that has specific plant type, returns 1 */
int	look_up(char (*m)[MAX_LINE_NUM], int x, int y, char plant);
int	look_right(char (*m)[MAX_LINE_NUM], int w, int x, int y, char plant);
int	look_down(char (*m)[MAX_LINE_NUM], int h, int x, int y, char plant);
int	look_left(char (*m)[MAX_LINE_NUM], int x, int y, char plant);

/* It checks whether a plot has already been found or not */
int plot_was_found(t_point *plots_found, int *plot_cnt, t_plot plot);

/* Recursively searches for all the groups
 * that have a specific plant type */
int	find_all_groups(char (*m)[MAX_LINE_NUM],
					int w, /* Width of the map */
					int h, /* Height of the map */
					int x, /* Current point (plot) x coordinate */
					int y, /* Current point (plot) y coordinate */
					char plant, /* Plant type of the searched group */
					t_group *groups,
					int *group_cnt,
					t_point *plots_found, /* Plots that have already been found */
					int *plot_cnt);

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	int		line_cnt;
	int		line_len; /* The length of the last line read from a file */
	int		ch;
	int		i;
	FILE	*fptr;


	char	m[MAX_LINE_NUM][MAX_LINE_LEN];
	int		width;
	int		height;

	t_group	groups[MAX_GROUPS_NUM];
	t_plot	plots_found[MAX_PLOTS_NUM];
	t_plot	cur_plot;

	int		group_cnt;
	int		plot_cnt;

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
				line_cnt++;
				line_len = i;
				i = 0;
				
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
			m[line_cnt][i] = '\0';
			break;
		}
		i++;
	}

	width = line_len;
	height = line_cnt;


	printf("width: %d\n", width);
	printf("height: %d\n", height);
	
	printf("\n");
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
			printf("%c", m[yi][xi]);
		printf("\n");
	}

	/* Clean up the debugging array */
    for (int yi = 0; yi < height; yi++)
        for (int xi = 0; xi < width; xi++)
            vp[yi][xi] = 0;
    call_cnt = 0;
	

	/* At the begining each plot has 0 turns,
	 * we assume that a plot is located somewhere
	 * inside its group */
	for (int i = 0; i < width * height; i++)
	{
		plots_found[i].turns_num = 0;
		plots_found[i].edges_num = 0;
	}



	group_cnt = 0;
	plot_cnt = 0;
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			cur_plot.coord.X = xi;
			cur_plot.coord.Y = yi;
			cur_plot.plant = m[yi][xi];
			/* If a plot hasn't been found yet */
			if (!plot_was_found(plots_found, &plot_cnt, cur_plot))
			{
                vp[yi][xi] = 1; /* Mark that we have visited the point corresponding to that plot */
                printf("m[%d][%d] = %c\n", yi, xi, cur_plot.plant);
                printf("f(m, %d, %d, %d, %d, %c, ...)\n", width, height, xi, yi, cur_plot.plant);
                sleep(SLEEP_TIME);
                call_cnt++;
                score = find_all_groups(m,
										width,
										height,
										xi,
										yi,
										cur_plot.plant,
										groups,
										&group_cnt,
										plots_found,
										&plot_cnt);
                printf("returned from the %d f()\n", call_cnt);
                printf("\nplots_found:\n");
                for (int i = 0; i < plot_cnt; i++)
                {
                    printf("%d. (%d, %d)\n", i, plots_found[i].coord.X, plots_found[i].coord.Y);
                }
                printf("\n");

                /* Let's clean up the array of coordinates of visited points */
                for (int yi = 0; yi < height; yi++)
                    for (int xi = 0; xi < width; xi++)
                        vp[yi][xi] = 0;
			}
		}
	}
	

	fclose(fptr);
	exit (EXIT_SUCCESS);
}

int	look_up(char (*m)[MAX_LINE_NUM], int x, int y, char plant)
{
	int	res;

	res = 0;
	if (y - 1 >= 0)
		if (m[y - 1][x] == plant)
			res = 1;
	return (res);
}

int	look_right(char (*m)[MAX_LINE_NUM], int w, int x, int y, char plant)
{
	int	res;

	res = 0;
	if (x + 1 < w)
		if (m[y][x + 1] == plant)
			res = 1;
	return (res);
}

int	look_down(char (*m)[MAX_LINE_NUM], int h, int x, int y, char plant)
{
	int	res;

	res = 0;
	if (y + 1 < h)
		if (m[y + 1][x] == plant)
			res = 1;
	return (res);
}

int	look_left(char (*m)[MAX_LINE_NUM], int x, int y, char plant)
{
	int	res;

	res = 0;
	if (x - 1 >= 0)
		if (m[y][x - 1] == plant)
			res = 1;
	return (res);
}

int plot_was_found(t_point *plots_found, int *plot_cnt, t_plot plot)
{
    int found;

    found = 0;
    for (int i = 0; i < *plot_cnt; i++)
    {
        if (plots_found[i].coord.X == plot.coord.X &&
			plots_found[i].coord.Y == plot.coord.Y)
            found = 1;
    }
    return (found);
}

int	find_all_groups(char (*m)[MAX_LINE_NUM],
					int w,
					int h,
					int x,
					int y,
					char plant,
					t_group *groups,
					int *group_cnt,
					t_point *plots_found,
					int *plot_cnt)
{
    printf("find_all_groups() says hello!\n");

	int     score;
	
	score = 0;

    vp[y][x] = 1;

	if (m[y][x].plant == plant)
	{
        plots_found[*plot_cnt].X = x;
        plots_found[*plot_cnt].Y = y;
        printf("nines_found[%d].X = %d\n", *nine_cnt, x);
        printf("nines_found[%d].Y = %d\n", *nine_cnt, y);
        printf("nine_cnt++\n");
        (*nine_cnt)++;
        printf("nine_cnt = %d\n", *nine_cnt);
		score++;
        printf("score++\n");
        printf("score = %d\n", score);

        for (int yi = 0; yi < h; yi++)
        {
            for (int xi = 0; xi < w; xi++)
            {
                if (vp[yi][xi])
                    printf("\033[32m%c\033[37m", m[yi][xi]);
                else
                    printf("%c", m[yi][xi]);
            }
            printf("\n");
        }
        printf("\n");

        return (score);
	}

    for (int hi = 0; hi < 9; hi++) /* Height index */
    {
        if (ch == hi + '0')
        {
            /* Looking up */
            if (look_up(m, x, y, (hi + 1) + '0'))
            {
                printf("loop_up(m, %d, %d, '%c')\n", x, y, (hi + 1) + '0');
                printf("f(m, %d, %d, %d, %d, '%c', ...)\n", w, h, x, y - 1, (hi + 1) + '0');
                sleep(SLEEP_TIME);

                call_cnt++;
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x,
                                        y - 1,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
                printf("returned from the %d f() call\nscore = %d\n\n", call_cnt, score);
            }
            /* Looking right */
            if (look_right(m, w, x, y, (hi + 1) + '0'))
            {
                printf("loop_right(m, %d, %d, %d, '%c')\n", w, x, y, (hi + 1) + '0');
                printf("f(m, %d, %d, %d, %d, '%c', ...)\n", w, h, x + 1, y, (hi + 1) + '0');
                sleep(SLEEP_TIME);

                call_cnt++;
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x + 1,
                                        y,
                                        (hi + 1) + '0',
                                        nines_found, nine_cnt);
                printf("returned from the %d f() call\nscore = %d\n\n", call_cnt, score);
            }
            /* Looking down */
            if (look_down(m, h, x, y, (hi + 1) + '0'))
            {
                printf("loop_down(m, %d, %d, %d, '%c')\n", h, x, y, (hi + 1) + '0');
                printf("f(m, %d, %d, %d, %d, '%c', ...)\n", w, h, x, y + 1, (hi + 1) + '0');
                sleep(SLEEP_TIME);

                call_cnt++;
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x,
                                        y + 1,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
                printf("returned from the %d f() call\nscore = %d\n\n", call_cnt, score);

            }
            /* Looking left */
            if (look_left(m, x, y, (hi + 1) + '0'))
            {
                printf("loop_left(m, %d, %d, '%c')\n", x, y, (hi + 1) + '0');
                printf("f(m, %d, %d, %d, %d, '%c', ...)\n", w, h, x - 1, y, (hi + 1) + '0');
                sleep(SLEEP_TIME);

                call_cnt++;
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x - 1,
                                        y,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
                printf("returned from the %d f() call\nscore = %d\n\n", call_cnt, score);
            }
        }
    }

	return (score);
}
