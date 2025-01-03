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

typedef long long   t_ll;

typedef struct s_group  t_group;

typedef struct s_plot
{
    int     X;
    int     Y;

    /* Pointer to the group to
     * which this plot belongs */
	t_group	*group; 

    /* Type of plant
     * this plot grow */
	char	plant;

    /* The number of turns
     * that have been performed
     * in this plot */
	int		turns_num; 
	int		edges_num;

}	t_plot;

typedef struct s_group
{
    /* Pointers to the found plots */
	t_plot	*plots[MAX_PLOTS_NUM];

    int     plots_num;
	int		group_id;
	char	plant;

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

/* It checks whether a plot with `x` and `y` coordinates has already
 * been found or not */
int plot_was_found(t_plot *plots_found, int *plot_cnt, int x, int y);

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
					t_plot *plots_found, /* Plots that have already been found */
					int *plot_cnt);

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	int		line_cnt;

    /* The length of the last
     * line read from a file */
	int		line_len;

	int		ch;
	int		i;
	FILE	*fptr;


	char	m[MAX_LINE_NUM][MAX_LINE_LEN];
	int		width;
	int		height;

	t_group	*groups;
	t_plot	*plots_found;

	int		group_cnt;
	int		plot_cnt;

    t_ll    total_price;

	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		printf("%s", ebuf);
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
        {
			m[line_cnt][i] = ch;
        }
		else
        {
			m[line_cnt][i] = '\0';
			break;
		}
		i++;
	}

	width = line_len;
	height = line_cnt;

    if (width * height > MAX_GROUPS_NUM || width * height > MAX_PLOTS_NUM)
    {
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "The size of matrix is too large");
		printf("%s", ebuf);
		exit(EXIT_FAILURE);
    }

    groups = (t_group *)malloc((width * height) * sizeof (t_group));
	if (!groups)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

    plots_found = (t_plot *)malloc((width * height) * sizeof (t_plot));
	if (!plots_found)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
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
		groups[i].group_id = 0;
		groups[i].plots_num = 0;
	}


    /* Main algorithm */
	group_cnt = 0; /* Global group counter */
	plot_cnt = 0; /* Global plot counter */
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			/* If a plot hasn't been found yet */
			if (!plot_was_found(plots_found, &plot_cnt, xi, yi))
			{
                groups[group_cnt].group_id = group_cnt;
                groups[group_cnt].plant = m[yi][xi];
                group_cnt++;

                call_cnt++;
                find_all_groups(m,
								width,
								height,
								xi,
								yi,
								m[yi][xi], /* plant */
								groups,
								&group_cnt,
								plots_found,
								&plot_cnt);

                /* Let's clean up the array of
                 * coordinates of visited points */
                for (int yi = 0; yi < height; yi++)
                    for (int xi = 0; xi < width; xi++)
                        vp[yi][xi] = 0;

			} // if (!plot_was_found(plots_found, &plot_cnt, xi, yi))
		} // for (int xi = 0; xi < width; xi++)
	} // for (int yi = 0; yi < height; yi++)

    /* Let's count the total price of fencing all regions on owr map */
    printf("total_price = ");
    total_price = 0;
    for (int gi = 0; gi < group_cnt; gi++)
    {
        int perimeter;
        int area;
        perimeter = 0;
        for (int pi = 0; pi < groups[gi].plots_num; pi++)
            perimeter += groups[gi].plots[pi]->edges_num;

        area = groups[gi].plots_num;

        total_price += (t_ll)perimeter * (t_ll)area;

        if (gi < group_cnt - 1)
            printf("(%d * %d) + ", perimeter, area);
        else
            printf("(%d * %d) = ", perimeter, area);
    }
    printf("%lld\n", total_price);

    printf("total price of fencing all regions is %lld BTC\n", total_price);

    free(plots_found);
    free(groups);
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

int plot_was_found(t_plot *plots_found, int *plot_cnt, int x, int y)
{
    int found;

    found = 0;
    for (int i = 0; i < *plot_cnt; i++)
    {
        if (plots_found[i].X == x && plots_found[i].Y == y)
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
					t_plot *plots_found,
					int *plot_cnt)
{
    /* If this plot does not
     * grow the same plant type
     * as the plot found previously */
	if (m[y][x] != plant)
    {
        /* We just go to the next
         * map cell (to check the
         * next plot) */
        return (1);
    }

    /* If this plot has already been found, that is,
     * if it is part of another plot group */
    if(plot_was_found(plots_found, plot_cnt, x, y))
    {
        /* We just go to the next
         * map cell (to check the
         * next plot) */
        return (1);
    }

    /* Mark that we have
     * visited the point
     * corresponding to
     * the current  plot */
    vp[y][x] = 1; 

    /* Creating a new plot */
    /* ############################################################ */

    plots_found[*plot_cnt].X = x;
    plots_found[*plot_cnt].Y = y;
    plots_found[*plot_cnt].plant = plant;
    plots_found[*plot_cnt].group = (void *)&groups[(*group_cnt) - 1];

    /* ############################################################ */


    /* Assign newly found plot to the current plot group */
    /* ############################################################ */

    groups[(*group_cnt) - 1].plots[ groups[(*group_cnt) - 1].plots_num ] = (void *)&plots_found[*plot_cnt];
    groups[(*group_cnt) - 1].plots_num++;

    /* ############################################################ */


    /* And only now when all asignments was finished */
    (*plot_cnt)++; /* Increment global plot counter */

    /* Now we continue our search by determining which
     * direction to go. We check in four directions: up,
     * right, down, and left. If we find a plot with the
     * same plant type as the current one in any of these
     * directions, we call the find_all_groups() function
     * for that plot */ 

    /* Looking up */
    /* We found a plot with a `plant` on the top */
    if (look_up(m, x, y, plant))
    {
        call_cnt++;
        find_all_groups(m,
                        w,
                        h,
                        x,
                        y - 1,
                        plant,
                        groups,
                        group_cnt,
                        plots_found,
                        plot_cnt);
    }
    /* We did not found a plot with a `plant` on the top */
    else
    {
        /* This indicates that we have reached a border,
         * so we need to increment the number of edges
         * for this plant */
        plots_found[(*plot_cnt) - 1].edges_num++;
    }

    /* Looking right */
    /* We found a plot with a `plant` on the right */
    if (look_right(m, w, x, y, plant))
    {
        call_cnt++;
        find_all_groups(m,
                        w,
                        h,
                        x + 1,
                        y,
                        plant,
                        groups,
                        group_cnt,
                        plots_found,
                        plot_cnt);
    }
    /* We did not found a plot with a `plant` on the right */
    else
    {
        /* This indicates that we have reached a border,
         * so we need to increment the number of edges
         * for this plant */
        plots_found[(*plot_cnt) - 1].edges_num++;
    }

    /* Looking down */
    /* We found a plot with a `plant` in the bottom cell */
    if (look_down(m, h, x, y, plant))
    {
        call_cnt++;
        find_all_groups(m,
                        w,
                        h,
                        x,
                        y + 1,
                        plant,
                        groups,
                        group_cnt,
                        plots_found,
                        plot_cnt);
    }
    /* We did not found a plot with a `plant` in the bottom cell */
    else
    {
        /* This indicates that we have reached a border,
         * so we need to increment the number of edges
         * for this plant */
        plots_found[(*plot_cnt) - 1].edges_num++;
    }

    /* Looking left */
    /* We found a plot with a `plant` on the left */
    if (look_left(m, x, y, plant))
    {
        call_cnt++;
        find_all_groups(m,
                        w,
                        h,
                        x - 1,
                        y,
                        plant,
                        groups,
                        group_cnt,
                        plots_found,
                        plot_cnt);
    }
    /* We did not found a plot with a `plant` on the left */
    else
    {
        /* This indicates that we have reached a border,
         * so we need to increment the number of edges
         * for this plant */
        plots_found[(*plot_cnt) - 1].edges_num++;
    }

    /* If we have looked in all directions
     * and have not found a plot with a plant
     * `plant` in any of them, we must return */
	return 1;
}
