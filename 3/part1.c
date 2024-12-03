#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/*
 * MAX_NUMS_STR - the maximum number of numbers in a line */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		4096
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_NUMS		10

typedef struct s_finder
{
	int	mul_pos;
}	t_finder;

typedef unsigned long long t_ull;

/* It counts the number of digits in a number */
int	get_num_len(int num)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

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

int	muls_searcher(char *line, t_finder *state)
{
	int	i;

	i = state->mul_pos;
	while (i < (int)ft_strlen(line) - 3)
	{
		if (line[i] == 'm')
		{
			if (line[i + 1] == 'u')
			{
				if (line[i + 2] == 'l')
				{
					if (line[i + 3] == '(')
					{
						state->mul_pos = i + 4;
						return (i);
					}
				}
			}
		}
		i++;
	}
	return (-1);
}

/*
 * It returns the number retreied after "mul("
 * sequence. If no number was found it returns -1.
 *     dpos - position of a digit in the found number. */
int	get_num(char *line, int pos)
{
	int		num;
	int		i;
	
	num = -1;
	i = pos;
	while ((i < pos + 3) && (i < (int)ft_strlen(line)))
	{
		if (ft_isdigit(line[i]))
		{
			if (ft_isdigit(line[i + 1]))
			{
				num = line[i] - '0';
				num *= 10;
				/* Number can not start with zero
				 * if it's a number consiting of two
				 * or more digits */
				if (line[i] == '0')
					return (-1);
				/* The number consist of three digits */
				if (ft_isdigit(line[i + 2]))
				{
					num += line[i + 1] - '0';
					num *= 10;
					num += line[i + 2] - '0';
				}
				else /* The number consist of two digits */
				{
					num += line[i + 1] - '0';
					break;
				}
			}
			else /* The number consist of one digit */
			{
				num = line[i] - '0';
				break;
			}
		}
		i++;
	}
	return (num);
}

t_ull	find_all_mul_instructions(char *line)
{
	t_finder	state;
	int			first_num;
	int			second_num;

	/* Global position in a searched line up
	 * to which the found mul 
	 * corresponds to the template mul(X,Y) */
	int			pos;

	/* The position of the next
	 * "mul(" substring found */
	int			next_mul; 

	/* The local result of the execution of all
	 * mul instructions found in the line */
	t_ull		line_mul_result; 

	line_mul_result = 0;
	state.mul_pos = 0;
	next_mul = 0;
	pos = 0;
	while (next_mul != -1)
	{
		/* The "mul(" substring was found */
		next_mul = muls_searcher(line, &state);
		if (next_mul != -1) 
		{
			pos = next_mul + 4;
			first_num = get_num(line, pos);
			/* The first number was found and
			 * it fits the template mul(X,Y) */
			if (first_num != -1)
			{
				//printf("first_num = %d : ", first_num);
				pos += get_num_len(first_num);
				/* The first number was followed by a comma */
				if (line[pos] == ',')
				{
					second_num = get_num(line, pos);
					/* The first number was found and
					 * it fits the template mul(X,Y) */
					if (second_num != -1)
					{
						//printf("second_num = %d\n", second_num);
						pos += get_num_len(second_num);
						pos++;
						//printf("pos = %d\n", pos);
						/* The insctuction must end
						 * with a closing parenthesis */
						if (line[pos] == ')')
						{
							for (int q = next_mul; q < pos + 1; q++)
								printf("%c", line[q]);
							printf("\n");
							line_mul_result += first_num * second_num;
							printf("local mul result is: %llu\n", line_mul_result);
						}
					}
				}
			} // if (first_num != -1)
		} // if (next_mul != -1)
	} // while (next_mul != -1)
	return (line_mul_result);
}

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		ch;
	int		i;
	FILE	*fptr;

	/*  Global result of execution of all
	 *  mul operations in all lines that
	 *  were found in the file */
	t_ull	global_mul_result;

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
	global_mul_result = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			line[i] = '\0';
			i = 0;
			global_mul_result += find_all_mul_instructions(line);
			printf("multiplication result is: %llu\n", global_mul_result);
			continue;
		}
		if (i < MAX_LINE_LEN)
		{
			line[i] = ch;
		}
		i++;
	}

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
