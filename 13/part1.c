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

# define BTN_A_COST			3
# define BTN_B_COST			1

typedef long long t_ll;

/*typedef struct	s_machine
{
	int	btn_A_X;
	int	btn_A_Y;

	int	btn_B_X;
	int	btn_B_Y;

	int prize_X;
	int	prize_Y;

}	t_machine;*/

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
int		get_nums(char *line, t_ll *nums, int arr_size)
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
			nums[nums_cnt] = ft_atoll(num);
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

	char	ops[MAX_NUMS_STR + 1];	/* Operators */
	int		nums_found;				/* Number of numbers found in an equation */

	/* Number of positions (spaces)
	 * between numbers in an equation */
	int		pos_num;

	int		add_flag;
	int		perms_num;				/* Permutations number */
	t_ll	op_res;					/* Result of an operation */

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

	int	coords[2];

	int	machine_cnt;

	int	btn_A_X;
	int	btn_A_Y;

	int	btn_B_X;
	int	btn_B_Y;

	int prize_X;
	int	prize_Y;

	/* The program quitting
	 * flag. We set it 1 if
	 * the input data was
	 * not correct */
	int	quit_flag;

	/* Let's read the ordering rules */
	i = 0;
	ch = 0;
	quit_flag = 0;
	line_cnt = 0;
	machines_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				line_len = i;
				i = 0;

				nums_found = get_nums(line, coords, MAX_NUMS_STR);
				if (nums_found != -1)
				{
					if (nums_found == 2)					
					{
						/* A button info */
						if (line_cnt % 4 == 1)
						{
							btn_A_X = coords[0];
							btn_A_Y = coords[1];
						}
						/* B button info */
						else if (line_cnt % 4 == 2)
						{
							btn_B_X = coords[0];
							btn_B_Y = coords[1];
						}
						/* Prize location info */
						else if (line_cnt % 4 == 3)
						{
							prize_X = coords[0];
							prize_Y = coords[1];
						}
					}
					/* If more than 2
					 * numbers were
					 * found in a line */
					else
					{
						/* It means something
						 * is wrong with input
						 * data! */
						quit_flag = 1;
						break;
					}
				}
				/* It's just an empty
				 * line dividing machines */
				else
				{
					printf("Button A: X+%d");
					machine_cnt++;
				}


				if (nums_found != 1)
				{
					pos_num = nums_found - 2; /* Minus test value */
					perms_num = (int)pow((double)2, (double)pos_num);

					add_flag = 1;
					/* Number of bits in permutation is equal the number of positions */
					for (int perm = 0; perm < perms_num; perm++)
					{
						for (int bit_ind = 0; bit_ind < pos_num; bit_ind++)
						{
							/* Checking if the bit number
							 * `bit_ind` in `perm` number
							 * is set or not  */
							if (perm & (1 << bit_ind)) /* If it's set */
								ops[bit_ind] = '+';
							else
								ops[bit_ind] = '*';
						}

						/* Calculating the operation result */
						op_res = eq[1];
						for (int j = 2; j < nums_found; j++)
						{
							if (ops[j - 2] == '+')
								op_res += eq[j];
							else
								op_res *= eq[j];
						}
						if (op_res == eq[0])
						{
							if (add_flag)
								total_calib_res += op_res;
							add_flag = 0;
						}

					} // for (int perm = 0; perm < perms_num; perm++)
				} // if (nums_found != 1)

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
	if (quit_flag)
	{
		printf("Something is wrong with your input data, buddy.");
		printf(" I am quitting\n");		
		fclose(fptr);
		exit (EXIT_FAILURE);
	}



	fclose(fptr);
	exit (EXIT_SUCCESS);
}
