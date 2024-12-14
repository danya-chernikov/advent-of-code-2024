/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 19:52:46 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/14 22:20:19 by dchernik         ###   ########.fr       */
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
# define MAX_LINE_NUM		2048
# define MAX_NUMS_STR		32
# define MAX_ROBOTS_NUM     512

typedef long long t_ll;

typedef struct	s_robot
{
	int p_x;
    int p_y;
    int v_x;
    int v_y;

}	t_robot;

typedef struct  s_tile
{
    /* The number of robots
     * located at this tile */
    int robots_num;

}   t_tile;


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


int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];

	/* Number of lines read from a file */
	int		line_cnt;

	int		ch;
	int		i;
	FILE	*fptr;


    t_robot r[MAX_ROBOTS_NUM];
    t_tile  **m;
    int     width;
    int     height;
	int     rdata[MAX_NUMS_STR];
    int     nums_found;
	int     robot_cnt;
    int     safety_factor; /* Global safety factor */
    int     sf_q1; /* The first quadrant safety factor */
    int     sf_q2; /* The second quadrant safety factor */
    int     sf_q3;
    int     sf_q4;


	if (argc != 4)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME WIDTH HEIGHT", argv[0]);
		printf("%s\n", ebuf);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(filename, argv[1], MAX_FILE_NAME_BUF);
    width = ft_atoi(argv[2]);
    height = ft_atoi(argv[3]);

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
	robot_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				i = 0;

                nums_found = get_nums(line, rdata, MAX_NUMS_STR);
                if (nums_found > 0)
                {
                    r[robot_cnt].p_x = rdata[1];
                    r[robot_cnt].p_y = rdata[2];
                    r[robot_cnt].v_x = rdata[3];
                    r[robot_cnt].v_y = rdata[4];
                    robot_cnt++;
                }
                
				line_cnt++;
				continue;
			}
			else
			{
				line[i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			line[i] = ch;
		else
		{
			line[i] = '\0';
			break;
		}
		i++;
	}

    printf("width = %d\nheight = %d\n", width, height);


    m = (t_tile **)malloc(height * sizeof (t_tile *));
    if (!m)
    {
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
    }
    for (int i = 0; i < height; i++)
    {
        m[i] = (t_tile *)malloc(width * sizeof (t_tile));
        if (!m)
        {
            snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
            perror(ebuf);
            exit(EXIT_FAILURE);
        }
    }

    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            m[yi][xi].robots_num = 0;
        }
    }

    for (int ri = 0; ri < robot_cnt; ri++)
    {
        m[ r[ri].p_y ][ r[ri].p_x ].robots_num++;
    }

    printf("\n");
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            if (m[yi][xi].robots_num == 0)
                printf(".");
            else
                printf("%c", m[yi][xi].robots_num + '0');
        }
        printf("\n");
    }
    printf("\n");

    /*for (int ri = 0; ri < robot_cnt; ri++)
    {
        printf("p=%d,%d v=%d,%d\n",
               r[ri].p_x,
               r[ri].p_y,
               r[ri].v_x,
               r[ri].v_y);
    }*/


    int x, y, v_x, v_y;

    for (int ri = 0; ri < robot_cnt; ri++)
    {
        x = r[ri].p_x;
        y = r[ri].p_y;
        v_x = r[ri].v_x;
        v_y = r[ri].v_y;

        /* The robot is leaving
         * the previous tile */
        if (m[y][x].robots_num > 0)
            m[y][x].robots_num--;

        for (int sec = 0; sec < 100; sec++)
        {
            /* Let's determine the
             * new x coordinate */
            if (x + v_x >= width)
                x = (x + v_x) % width;
            else if ((x + v_x < width) && (x + v_x >= 0))
                x = x + v_x;
            else if (x + v_x < 0)
                x = width + (x + v_x);

            /* Let's determine the
             * new y coordinate */
            if (y + v_y >= height)
                y = (y + v_y) % height;
            else if ((y + v_y < height) && (y + v_y >= 0))
                y = y + v_y;
            else if (y + v_y < 0)
                y = height + (y + v_y);
        }

        /* The robot ended up
         * on the new tile */
        m[y][x].robots_num++;

        r[ri].p_x = x;
        r[ri].p_y = y;
        r[ri].v_x = v_x;
        r[ri].v_y = v_y;
    }

    printf("\n");
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            if (m[yi][xi].robots_num == 0)
                printf(".");
            else
                printf("%c", m[yi][xi].robots_num + '0');
        }
        printf("\n");
    }
    printf("\n");


    sf_q1 = 0;
    /* First quadrant */
    for (int yi = 0; yi < height / 2; yi++)
    {
        for (int xi = 0; xi < width / 2; xi++)
        {
            if (m[yi][xi].robots_num > 0)
                sf_q1 += m[yi][xi].robots_num; 
        }
    }
    printf("sf_q1 = %d\n", sf_q1);

    sf_q2 = 0;
    /* Second quadrant */
    for (int yi = 0; yi < height / 2; yi++)
    {
        for (int xi = (width / 2) + 1; xi < width; xi++)
        {
            if (m[yi][xi].robots_num > 0)
                sf_q2 += m[yi][xi].robots_num; 
        }
    }
    printf("sf_q2 = %d\n", sf_q2);

    sf_q3 = 0;
    /* Third quadrant */
    for (int yi = (height / 2) + 1; yi < height; yi++)
    {
        for (int xi = (width / 2) + 1; xi < width; xi++)
        {
            if (m[yi][xi].robots_num > 0)
                sf_q3 += m[yi][xi].robots_num; 
        }
    }
    printf("sf_q3 = %d\n", sf_q3);

    sf_q4 = 0;
    /* Fourth quadrant */
    for (int yi = (height / 2) + 1; yi < height; yi++)
    {
        for (int xi = 0; xi < width / 2; xi++)
        {
            sf_q4 += m[yi][xi].robots_num;
        }
    }
    printf("sf_q4 = %d\n", sf_q4);

    safety_factor = sf_q1 * sf_q2 * sf_q3 * sf_q4;

    printf("safety factor = %d\n", safety_factor);

    for (int i = 0; i < height; i++)
        free(m[i]);
    free(m);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
