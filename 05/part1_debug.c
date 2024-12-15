/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:30:22 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/06 02:07:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* MAX_NUMS_STR - the maximum
 * number of numbers in a string */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_LEN		512
# define MAX_LINE_NUM		2048
# define MAX_NUMS_STR		32

typedef struct s_map
{
	int X;
	int Y;
}	t_map;

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
		if (ft_isdigit(line[i]))
		{
			num[j] = line[i];
			j++;
		}
		if (!ft_isdigit(line[i]))
		{
			num[j] = '\0';
			nums[nums_cnt] = ft_atoi(num);
			nums_cnt++;
			j = 0;
			if (line[i] == '\0')
				break;
			while (!ft_isdigit(line[i]) && line[i] != '\0')
				i++;
			continue;
		}
		if (nums_cnt > arr_size)
			return (-1);
		i++;
	}
	return (nums_cnt);
}

void	print_update(int (*u)[MAX_NUMS_STR], int up_ind)
{
	for (int i = 0; u[up_ind][i] != -1; i++)
		printf("%d ", u[up_ind][i]);
	printf("\n");
}

int		get_update_len(int (*u)[MAX_NUMS_STR], int up_ind)
{
	int	len;

	for (len = 0; u[up_ind][len] != -1; len++);
	return (len);
}

int		is_rule_exist(t_map *r, int rnum, int x, int y)
{
	for (int i = 0; i < rnum; i++)
	{
		if (r[i].X == x && r[i].Y == y)
		{
			return (1);
		}
	}
	return (0);
}

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
	t_map	*r;
	int		rules_num;

	int		u[MAX_LINE_NUM][MAX_NUMS_STR]; /* Updates */
	int		updates_num;

	/* Number of number read from
	 * a line representing an update */
	int		up_nums_read; 

	/* An index used to copy
	 * found update numbers
	 * from a temporary array */
	int		up_num_ind; 

	/* A temporary array serving for retreiving
	 * X and Y values from each line of the
	 * ordering rules section and also for store
	 * the numbers found in an update line */
	int		nums_found[MAX_NUMS_STR + 1];


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

	r = (t_map *)malloc(MAX_LINE_NUM * sizeof (t_map));
	if (!r)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

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
				line[i] = '\0';
				line_len = i;
				i = 0;

				if (2 == get_nums(line, nums_found, MAX_NUMS_STR))
				{
					r[line_cnt].X = nums_found[0];
					r[line_cnt].Y = nums_found[1];
				}
				line_cnt++;

				if (line_len == 0) /* The ordering rule section ended */
					break;

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
	/* we substracting one because
	 * there is an empty line dividing
	 * the section of rules and updates
	 * and the algorhtim counted it */
	rules_num = line_cnt - 1; 

	/*printf("number of rules: %d\n", rules_num);

	for (int i = 0; i < rules_num; i++)
	{
		printf("%d|%d\n", r[i].X, r[i].Y);
	}
	printf("\n");*/

	/* Let's read the updates */
	i = 0;
	ch = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				line_len = i;
				i = 0;

				up_nums_read = get_nums(line, nums_found, MAX_NUMS_STR);
				if (up_nums_read != -1)
				{
					up_num_ind = 0;
					while (up_num_ind < up_nums_read)
					{
						u[line_cnt][up_num_ind] = nums_found[up_num_ind];
						up_num_ind++;
					}
					u[line_cnt][up_num_ind] = -1; /* Marking the end of the number sequence */
				}
				line_cnt++;

				if (line_len == 0) /* The ordering rule section ended */
					break;

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
	updates_num = line_cnt;

	/*printf("number of updates: %d\n", updates_num);

	int num_ind;
	for (int up_ind = 0; up_ind < updates_num; up_ind++)
	{
		num_ind = 0;
		while (u[up_ind][num_ind] != -1)
		{
			printf("%d ", u[up_ind][num_ind]);
			num_ind++;
		}
		printf("\n");
	}	
	printf("\n");*/

	/* Checking if any update contains an even number of numbers */
	/*for (int i = 0; i < updates_num; i++)
	{
		int len = get_update_len(u, i);
		if (len % 2 == 0)
		{
			printf("ALERT!!! The update with"
				   "the index %d contain %d numbers\n",
				   i, len);
		}
	}*/

	int	total_mid_page_num = 0;
	
	/* Algorithm itself */
	int	up_len;
	int	is_up_good;
	int	good_up_cnt = 0;
	for (int upi = 0; upi < updates_num; upi++)
	{
		/* Let's suppose the update is good, i.e. is in correct order */
		is_up_good = 1;
		up_len = get_update_len(u, upi);
		for (int yi = 0; yi < up_len - 1; yi++)
		{
			for (int xi = yi + 1; xi < up_len; xi++)
			{
				/* check if u[xi] must stand before up[yi] */
				if (is_rule_exist(r, rules_num, u[upi][xi], u[upi][yi]))
				{
					is_up_good = 0;
					break;
				}
			}
			if (!is_up_good)
			{
				break; /* Go to check the next udpate */
			}
		}
		if (is_up_good)
		{
			good_up_cnt++;
			total_mid_page_num += u[upi][up_len / 2];
			print_update(u, upi);
		}
	}

	printf("\ngood_up_cnt = %d\n", good_up_cnt);

	printf("\ntotal middle page number = %d\n", total_mid_page_num);

	free(r);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
