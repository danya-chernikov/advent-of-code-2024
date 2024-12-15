/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 21:19:05 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/08 23:31:41 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* MAX_NUMS_STR    - the maximum number of
 *					 numbers in a string;
 * MAX_NUM_STR_LEN - the maximum number of
 *					 digits in a number. */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	32
# define MAX_LINE_LEN		256
# define MAX_LINE_NUM		1024
# define MAX_NUMS_STR		32


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


    t_line  line;
    int     res;

    /* First quadrant test */
    /*res = construct_line(&line,
                        (t_point){.X=9, .Y=10},
                        (t_point){.X=12, .Y=7});*/

    /* Second quadrant test */
    /*res = construct_line(&line,
                        (t_point){.X=9, .Y=10},
                        (t_point){.X=6, .Y=7});*/

    /* Third quadrant test */
    /*res = construct_line(&line,
                        (t_point){.X=9, .Y=10},
                        (t_point){.X=6, .Y=13});*/

    /* Fourth quadrant test */
    res = construct_line(&line,
                        (t_point){.X=12, .Y=13},
                        (t_point){.X=9, .Y=10});

    if (res)
    {
        if (line.fa.X >= 0 && line.fa.X < width &&
            line.sa.X >= 0 && line.sa.X < width &&
            line.fa.Y >= 0 && line.fa.Y < height &&
            line.sa.Y >= 0 && line.sa.Y < height)
        {
            m[line.fa.Y][line.fa.X] = '#';
            m[line.sa.Y][line.sa.X] = '#';
        }
    }

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
