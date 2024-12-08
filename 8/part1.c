/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 21:19:05 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/09 00:52:16 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		64
# define MAX_LINE_NUM		64

typedef struct s_point
{
    int X;
    int Y;

}   t_point;

typedef struct s_line
{
    t_point fa; /* First antinode coordinates */
    t_point sa; /* Second antinode coordinates */

}   t_line;

typedef struct s_map
{
    char    freq;

    /* Number of antennas that
     * have `freq` frequency */
    int     ant_num;

    /* Coordinates of all antennas that have `freq` frequency */
    t_point ants[MAX_LINE_NUM * MAX_LINE_LEN]; /* Antennas */

}   t_map;

/* Returns the index of the frequency `freq` */
int     was_found(t_map *f, int *freq_cnt, char freq)
{
    for (int fi = 0; fi < *freq_cnt; fi++)
    {
        if (f[fi].freq == freq)
            return (fi);
    }
    return (-1); /* Frequency was not found */
}

/* Adds the new frequency `freq` that has coordinates `x` and `y`
 * in the map (dictionary) `f` */
void    add(t_map *f, int *freq_cnt, char freq, int x, int y)
{
    int find = was_found(f, freq_cnt, freq);

    /* An antenna with `freq` frequency was
     * already found, i.e. the key `freq` are
     * already present in the map `f` */
    if (find != -1)
    {
        f[find].ants[ f[find].ant_num ].X = x;
        f[find].ants[ f[find].ant_num ].Y = y;
        f[find].ant_num++;
    }
    else /* An antenna with `freq` was not found yet */
    {
        /* Let's add a new frequency in the map */
        f[*freq_cnt].freq = freq;
        f[*freq_cnt].ants[0].X = x;
        f[*freq_cnt].ants[0].Y = y;
        f[*freq_cnt].ant_num++;
        (*freq_cnt)++;
    }
}

/* It constructs a line from two points.
 * Returns two points representing antinodes
 * of this line. If the construction of the
 * line is not possible returns 0 */
int construct_line(t_line *res, t_point p1, t_point p2)
{
    int     dx;
    int     dy;

    if (p1.X == p2.X || p1.Y == p2.Y)
    {
        return (-1);
    }

    dx = abs(p1.X - p2.X);
    dy = abs(p1.Y - p2.Y);

    /* The first point is in the frist
     * quadrant relative to the second */
    if (p1.X > p2.X && p1.Y < p2.Y) /* First quadrant */
    {
        res->fa.X = p1.X + dx;
        res->fa.Y = p1.Y - dy;

        /* At the same time it means that
         * the second point is in the third
         * quadrant relative to the first */
        res->sa.X = p2.X - dx;
        res->sa.Y = p2.Y + dy;
    }

    /* The first point is in the second
     * quadrant relative to the second */
    if (p1.X < p2.X && p1.Y < p2.Y) /* Second quadrant */
    {
        res->fa.X = p1.X - dx;
        res->fa.Y = p1.Y - dy;

        /* At the same time it means that
         * the second point is in the fourth
         * quadrant relative to the first */
        res->sa.X = p2.X + dx;
        res->sa.Y = p2.Y + dy;
    }

    if (p1.X < p2.X && p1.Y > p2.Y) /* Third quadrant */
    {
        res->fa.X = p1.X - dx;
        res->fa.Y = p1.Y + dy;

        res->sa.X = p2.X + dx;
        res->sa.Y = p2.Y - dy;
    }

    if (p1.X > p2.X && p1.Y > p2.Y) /* Forth quadrant */
    {
        res->fa.X = p1.X + dx;
        res->fa.Y = p1.Y + dy;

        res->sa.X = p2.X - dx;
        res->sa.Y = p2.Y - dy;
    }

    return (1);
}


int	main(int argc, char *argv[])
{
    printf("kek\n");
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

    char    m[MAX_LINE_NUM][MAX_LINE_LEN];
    int     width;
    int     height;

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

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c", m[i][j]);
        }
        printf("\n");
    }

    /* Counter of the unique
     * frequencies found */
    int     freq_cnt; 

    /* A dictionary that stores all unique
     * frequencies found */
    t_map   f[MAX_LINE_NUM * MAX_LINE_LEN];

    for(int i = 0; i < MAX_LINE_NUM * MAX_LINE_LEN; i++)
        f[i].ant_num = 0;
    
    freq_cnt = 0;
    /* Let's group all the antennas that exist
     * in the matrix by their frequencies
     * using a dictionary `f` */
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            if (m[yi][xi] != '.')
            {
                add(f, &freq_cnt, m[yi][xi], xi, yi);
            }
        }
    }

    int     anum;
    /* Let's print the generated dictionary */
    for (int ki = 0; ki < freq_cnt; ki++)
    {
        printf("'%c' => { ", f[ki].freq);
        anum = f[ki].ant_num;
        for (int ai = 0; ai < anum; ai++)
        {
            if (ai < anum - 1)
                printf("(%d, %d), ", f[ki].ants[ai].X, f[ki].ants[ai].Y);
            else
                printf("(%d, %d) ", f[ki].ants[ai].X, f[ki].ants[ai].Y);
        }
        printf("};\n");
    }



 /*   if (res)
    {
        if (line.fa.X >= 0 && line.fa.X < width &&
            line.sa.X >= 0 && line.sa.X < width &&
            line.fa.Y >= 0 && line.fa.Y < height &&
            line.sa.Y >= 0 && line.sa.Y < height)
        {
            m[line.fa.Y][line.fa.X] = '#';
            m[line.sa.Y][line.sa.X] = '#';
        }
    }*/

    printf("\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c", m[i][j]);
        }
        printf("\n");
    }

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
