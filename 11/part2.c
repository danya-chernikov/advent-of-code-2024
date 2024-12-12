/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:03:15 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/11 23:01:37 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <sys/time.h>

/* MAX_NUMS_STR    - the maximum number
 *                   of numbers in a string;
 * MAX_NUM_STR_LEN - the maximum string length
 *                   of a number. */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	32
# define MAX_LINE_LEN		1024
# define MAX_LINE_NUM		128
# define MAX_NUMS_STR		32
# define MAX_PEBBLES_NUM    134217728
# define NS_IN_S            1E9

typedef long long  t_ll;

static inline int   count_digits(t_ll num);
static inline t_ll  ft_atoll(const char *s);
static inline void  divide_num(t_ll num, t_ll *pleft, t_ll *pright);
static int          get_nums(char *line, t_ll *nums, int arr_size);

t_ll    pebbles[MAX_PEBBLES_NUM];

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];

	int		ch;
	int		i;
	FILE	*fptr;

    struct timespec start;
    struct timespec stop; 
    struct timespec duration;

    t_ll    pebble_cnt;
    t_ll    pleft;
    t_ll    pright;
    int     nums_found;
    int     blinks_num;


	if (argc != 3)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME BLINKS_NUMBER", argv[0]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}
	strncpy(filename, argv[1], MAX_FILE_NAME_BUF);

    blinks_num = atoi(argv[2]);

	fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[1]);
		printf("%s", ebuf);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (i < MAX_LINE_LEN)
			line[i] = ch;
		else
			break;
		i++;
	}
    /* We need to remove the '\n'
     * for the get_nums() not to
     * count it */
    line[i - 1] = '\0';

    nums_found = get_nums(line, pebbles, MAX_PEBBLES_NUM);
 
    pebble_cnt = nums_found;

    double  time_res = 0;

    for (int blink_ind = 0; blink_ind < blinks_num; blink_ind++)
    {
        printf("%d blink\n", blink_ind + 1);
        printf("pebble_cnt = %lld\n", pebble_cnt);
        printf("working time: %lf seconds\n\n", time_res);

        /* Start time measuring */
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        for (t_ll pi = 0; pi < pebble_cnt; pi++)
        {
            /* First rule */
            if (pebbles[pi] == 0)
            {
                pebbles[pi] = 1;
            }
            else if (pebbles[pi] != 0)
            {
                /* Second rule */
                if (count_digits(pebbles[pi]) % 2 == 0)
                {
                    /* Shift all subsequent numbers to the right by 1 */
                    for (t_ll q = pebble_cnt; q > pi; q--)
                    {
                        pebbles[q] = pebbles[q - 1];
                    }
                    pebble_cnt++;
                    divide_num(pebbles[pi], &pleft, &pright);
                    pebbles[pi] = pleft;
                    pebbles[pi + 1] = pright;
                    pi++;
                }
                /* Third rule */
                else
                {
                    pebbles[pi] *= 2024;
                }
            }
        }
        /* Stop time measuring */
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

        /* Count the time difference */
        if ((stop.tv_nsec - start.tv_nsec) < 0)
        {
            duration.tv_sec = stop.tv_sec - start.tv_sec - 1;
            duration.tv_nsec = NS_IN_S + stop.tv_nsec - start.tv_nsec;
        }
        else
        {
            duration.tv_sec = stop.tv_sec - start.tv_sec;
            duration.tv_nsec = stop.tv_nsec - start.tv_nsec;
        }
        /* Get the time difference in seconds */
        time_res = duration.tv_sec + duration.tv_nsec / NS_IN_S;
    }

    printf("After blinking %d times we got %lld stones\n", blinks_num, pebble_cnt);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}

static inline int   count_digits(t_ll num)
{
    int dnum;

    dnum = 0;
    while (num != 0)
    {
        num /= 10;
        dnum++;
    }
    return (dnum);
}

static inline void  divide_num(t_ll num, t_ll *pleft, t_ll *pright)
{
    char    right[MAX_NUM_STR_LEN / 2];
    int     half_digits_num;
    int     ld_cnt; /* Left number digit counter */
    int     j;

    ld_cnt = 0;
    *pright = 0;
    half_digits_num = count_digits(num) / 2;
    for (int i = 0; i < half_digits_num; i++)
    {
        right[ld_cnt] = (char)(num % 10) + '0';
        ld_cnt++;
        num /= 10;
    }

    /* Skip extra leading zeros */
    j = ld_cnt - 1;
    while (right[j] == '0' && j >= 0)
        j--;

    /* If all digits in the
     * right part are zeros */
    if (j == -1)
        *pright = 0;
    /* In another case, just assign
     * the first non-zero digit to
     * the pright */
    else
    {
        *pright = (t_ll)(right[j] - '0');
        j--;
    }

    if (pright != 0)
    {
        /* Create the right part number */
        while (j >= 0)
        {
            *pright *= 10LL;
            *pright += (t_ll)(right[j] - '0');
            j--;
        }
    }

    /* Create the left part number */
    *pleft = num;
}

static inline t_ll  ft_atoll(const char *s)
{
	int		neg_flag;
	int		i;
	t_ll	res;

	res = 0;
	neg_flag = 0;
	i = 0;
	while ((s[i] > 8 && s[i] < 14) || s[i] == 32)
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg_flag = 1;
		i++;
	}
	while (s[i] > 47 && s[i] < 58)
	{
		res = (res * 10) + (s[i] - '0');
		i++;
	}
	if (neg_flag)
		res *= -1;
	return (res);
}

/* It finds all the numbers in the line, divided by
 * any number of any symbols, then puts these numbers into the
 * nums array. It returns the number of found numbers.
 * If the number of numbers exceeds the size of the nums
 * array, defined as arr_size, the function returns -1. */
static int  get_nums(char *line, t_ll *nums, int arr_size)
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
		if (isdigit(line[i]))
		{
			num[j] = line[i];
			j++;
		}
		if (!isdigit(line[i]))
		{
			num[j] = '\0';
			nums[nums_cnt] = ft_atoll(num);
			nums_cnt++;
			j = 0;
			if (line[i] == '\0')
				break;
			while (!isdigit(line[i]) && line[i] != '\0')
				i++;
			continue;
		}
		if (nums_cnt > arr_size)
			return (-1);
		i++;
	}
	return (nums_cnt);
}
