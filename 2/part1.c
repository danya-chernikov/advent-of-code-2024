#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/*
 * MAX_NUMS_STR - the maximum number of numbers in a line */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		32
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_NUMS		10

/*
 * It finds all the numbers in the line, divided by
 * single spaces, then puts these numbers into the
 * nums array. It returns the number of found numbers.
 * If the number of numbers exceeds the size of the nums
 * array, defined as arr_size, the function returns -1. */
int	get_nums(char *line, int *nums, int arr_size)
{
	int		i;
	int		j;
	int		nums_cnt;
	char	num[MAX_NUM_STR_LEN];

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

/* It checks whether the report complies the first condition
 * that "The levels are either all increasing or all decreasing".
 *     lnum - the number of levels in the report */
int	check_first_rule(int *report, int lnum)
{

	int	order; /* 0 - increasing; 1 - decreasing; */
	int	i;

	if (lnum < 2)
		return (0);
	i = 0;
	/* As long as all the levels are the equal
	 * we keep going through the report */
	while ((report[i] == report[i + 1]) && (i < lnum - 1))
		i++;

	/* This is the first case when
	 * the next level is not equal
	 * to the current level */
	if (i < lnum - 1)
	{
		if (report[i] > report[i + 1])
		{
			order = 1; /* The order is supposed to be decreasing */
		}
		else
		{
			order = 0; /* Increasing */
		}

		/* Now let's see if the order is preserved */
		/* If the order was decreasing */
		if (order == 1)
		{
			while (i < lnum - 1)
			{
				/* And we found a level that is
				 * bigger than the previous one */
				if (report[i] < report[i + 1]) 
				{
					/* This means the order was
					 * broken here at this level */
					return (0); /* Report does not safe	*/
				}
				i++;
			}
		}
		else /* If the order was increasing */
		{
			while (i < lnum - 1)
			{
				/* And we found a level that is
				 * lesser than the previous one */
				if (report[i] > report[i + 1])
				{
					return (0);
				}
				i++;
			}
		}
	}
	/* In other case all the numbers
	 * in the array are the same */
	else
	{
		/* It's not possible
		 * to determine the order */
		return (0); 
	}

	return (1);
}

/* Here we check whether the report complies the second condition
 * that "Any two adjacent levels differ by at least one and at most three".
 *     lnum - the number of levels in the report */
int	check_second_rule(int *report, int lnum)
{
	int	i;
	int	diff;

	i = 0;
	while (i < lnum - 1)
	{
		diff = abs(report[i] - report[i + 1]);
		if (diff < 1 || diff > 3)
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		ch;
	int		i;
	FILE	*fptr;

	int		nums[MAX_LINE_NUMS];
	int		nums_found;
	int		safe_report_cnt;

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
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[0]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	i = 0;
	ch = 0;
	safe_report_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			line[i] = '\0';
			i = 0;
			nums_found = get_nums(line, nums, MAX_LINE_NUMS);
			if (nums_found)
			{
				if (check_first_rule(nums, nums_found))
					if (check_second_rule(nums, nums_found))
						safe_report_cnt++;
			}
			continue;
		}
		if (i < MAX_LINE_LEN)
		{
			line[i] = ch;
		}
		i++;
	}

	printf("safe reports number is: %d\n", safe_report_cnt);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
