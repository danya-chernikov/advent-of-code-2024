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
# define MAX_MACHINES_NUM   256

# define BTN_A_COST			3
# define BTN_B_COST			1

typedef long long t_ll;

typedef struct	s_machine
{
	int	btn_A_X;
	int	btn_A_Y;

	int	btn_B_X;
	int	btn_B_Y;

	int prize_X;
	int	prize_Y;

}	t_machine;

t_ll	ft_atoll(const char *s)
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


int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];

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

    t_machine   m[256];

	int	coords[5];

    int nums_found;
	int	machine_cnt;

	/* The program quitting
	 * flag. We set it 1 if
	 * the input data was
	 * not correct */

    int comp_cnt; /* Counter of each component of the machine */

	/* Let's read the ordering rules */
	i = 0;
	ch = 0;
    comp_cnt = 0;
	line_cnt = 0;
	machine_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				line_len = i;
				i = 0;
                
                if (line_len == 0)
                {
                    comp_cnt = 0;
                }
                else
                {
                    nums_found = get_nums(line, coords, MAX_NUMS_STR);
                    //printf("line %d: nums_found = %d\n", line_cnt, nums_found);
                    if (nums_found > 0)
                    {
                        if (comp_cnt == 0 && line_cnt > 0)
                        {
                            machine_cnt++;
                        }
                        /* A button info */
                        if (comp_cnt == 0)
                        {
                            m[machine_cnt].btn_A_X = coords[1];
                            m[machine_cnt].btn_A_Y = coords[2];
                        }
                        /* B button info */
                        else if (comp_cnt == 1)
                        {
                            m[machine_cnt].btn_B_X = coords[1];
                            m[machine_cnt].btn_B_Y = coords[2];
                        }
                        /* Prize location info */
                        else if (comp_cnt == 2)
                        {
                            m[machine_cnt].prize_X = coords[1];
                            m[machine_cnt].prize_Y = coords[2];
                        }
                    }
                    comp_cnt++;
                }
                
				line_cnt++;
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
    machine_cnt++;

    for (int mi = 0; mi < machine_cnt; mi++)
    {
        printf("Button A: X+%d, Y+%d\n", m[mi].btn_A_X, m[mi].btn_A_Y);
        printf("Button B: X+%d, Y+%d\n", m[mi].btn_B_X, m[mi].btn_B_Y);
        printf("Prize: X=%d, Y=%d\n\n", m[mi].prize_X, m[mi].prize_Y);
    }


    for (int mi = 0; mi < machine_cnt; mi++)
    {

    }



	fclose(fptr);
	exit (EXIT_SUCCESS);
}
