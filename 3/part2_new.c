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
# define MAX_FILE_SIZE		32768
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_NUMS		10
# define MAX_MAP_SIZE		1024
# define DO_STR				"do()"
# define DONT_STR			"don't()"

typedef struct s_finder
{
	int	mul_pos;
	int	do_pos; /* The position of the last "do()" found */
	int	dont_pos; /* The position of the last "don't()" found */
}	t_finder;

typedef unsigned long long t_ull;

long	get_file_size(FILE *fptr);
int		get_num_len(int num);
int		get_nums(char *line, int *nums, int arr_size);
int		muls_searcher(char *line, t_finder *state);
int		dos_searcher(char *line, t_finder *state);
int		donts_searcher(char *line, t_finder *state);
int		get_num(char *line, int pos);
t_ull	find_all_mul_instructions(char *line, int **map, int msize);

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		ch;
	int		i;
	FILE	*fptr;

	t_finder	state;

	/*  Global result of execution of all
	 *  mul operations in all lines that
	 *  were found in the file */
	t_ull	global_mul_result;

	int		**map;
	int		area_cnt; /* Counter of the save zones */

	char	*file_cnt;

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

	/* Allocating memory to the file content buffer. We use ft_calloc()
	 * here to zero out the buffer to be able to add content to it later
	 * easily via the ft_strlcat() function */
	file_cnt = (char *)ft_calloc(MAX_FILE_SIZE + 1, sizeof (char));
	if (file_cnt == NULL)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	/* Allocating memory for the map */
	map = (int **)malloc((MAX_MAP_SIZE + 1) * sizeof (int *));
	if (map == NULL)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < MAX_MAP_SIZE + 1; i++)
	{
		map[i] = (int *)malloc(2 * sizeof (int));
		if (map[i] == NULL)
		{
			snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
			perror(ebuf);
			exit(EXIT_FAILURE);
		}
	}

	/* Let's read all the file content into the buffer */
	i = 0;
	ch = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			line[i] = '\0';
			i = 0;
			ft_strlcat(file_cnt, line, MAX_FILE_SIZE);
			ft_strlcat(file_cnt, "\n", MAX_FILE_SIZE);
			continue;
		}
		if (i < MAX_LINE_LEN)
		{
			line[i] = ch;
		}
		i++;
	}

	/* Let's fill in the map */
	int	next_do_pos;
	int	next_dont_pos;

	i = 0;
	area_cnt = 0;
	next_do_pos = -1;

	/* Let's find the first "don't" */
	next_dont_pos = donts_searcher(file_cnt, &state);

	/* First "don't" was not found */
	if (next_dont_pos == -1) 
	{
		/* It's permitted to execute mul anywhere */
		map[area_cnt][0] = 0;
		map[area_cnt][1] = ft_strlen(file_cnt) - 1;
	}
	else
	{
		/* First "don't" was successfully found */
		map[area_cnt][0] = 0;
		map[area_cnt][1] = next_dont_pos - 1;
		area_cnt++;

cont:
		/* Now we need to find the first "do" that is
		 * located after the "don't" found previously */
		while (next_do_pos < next_dont_pos)
		{
			next_do_pos = dos_searcher(file_cnt, &state);
			/* The "do" was not found after the previous "don't" */
			if (next_do_pos == -1)
			{
				goto ret; /* The unltimately found "don't" was the last one; */
			}
		}
		/* First "do" was found */
		map[area_cnt][0] = next_do_pos + ft_strlen(DO_STR);


		/* Now searching for "don't". But! This "don't" must be located
		 * after the "do" found previously */
		while (next_dont_pos < next_do_pos)
		{
			next_dont_pos = donts_searcher(file_cnt, &state);

			/* "don't" was not found */
			if (next_dont_pos == -1)
			{	
				map[area_cnt][1] = ft_strlen(file_cnt) - 1;
				area_cnt++;
				goto ret;
			}
		}
		/* "don't was found */	
		map[area_cnt][1] = next_dont_pos - 1;
		area_cnt++;

		goto cont;
	}

ret:
	
	global_mul_result += find_all_mul_instructions(line, map, &state, area_cnt);
	printf("multiplication result is: %llu\n", global_mul_result);

	for (int i = 0; i < MAX_MAP_SIZE + 1; i++)
		free(map[i]);
	free(map);

	free(file_cnt);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}

long	get_file_size(FILE *fptr)
{
	long	size;

	fseek(fptr, 0L, SEEK_END);
	size = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	return (size);
}

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

int	muls_searcher(char *line, t_finder *state)
{
	int	i;

	if ((int)ft_strlen(line) < 4)
		return (-1);
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

int	dos_searcher(char *line, t_finder *state)
{
	int	i;

	if ((int)ft_strlen(line) < 4)
		return (-1);
	i = state->do_pos;
	while (i < (int)ft_strlen(line) - 3)
	{
		if (line[i] == 'd')
		{
			if (line[i + 1] == 'o')
			{
				if (line[i + 2] == '(')
				{
					if (line[i + 3] == ')')
					{
						state->do_pos = i + 4;
						return (i);
					}
				}
			}
		}
		i++;
	}
	return (-1);
}

int	donts_searcher(char *line, t_finder *state)
{
	int	i;

	if ((int)ft_strlen(line) < 7)
		return (-1);
	i = state->dont_pos;
	while (i < (int)ft_strlen(line) - 6)
	{
		if (line[i] == 'd')
		{
			if (line[i + 1] == 'o')
			{
				if (line[i + 2] == 'n')
				{
					if (line[i + 3] == '\'')
					{
						if (line[i + 4] == 't')
						{
							if (line[i + 5] == '(')
							{
								if (line[i + 6] == ')')
								{
									state->dont_pos = i + 7;
									return (i);
								}
							}
						}
					}
				} // if (line[i + 2] == 'n')
			} // if (line[i + 1] == 'o')
		} // if (line[i] == 'd')
		i++;
	} // while (i < (int)ft_strlen(line) - 6)
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
					break;
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

t_ull	find_all_mul_instructions(char *line, t_finder *state, int **map, int msize)
{
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
	first_num = 0;
	second_num = 0;
	pos = 0;
	while (next_mul != -1)
	{
		/* The "mul(" substring was found */
		next_mul = muls_searcher(line, &state);
		//printf("next_mul = %d\n", next_mul);
		if (next_mul != -1)
		{
			pos = next_mul + 4;
			first_num = get_num(line, pos);
			/* The first number was found and
			 * it fits the template mul(X,Y) */
			if (first_num != -1)
			{
				//printf("\tfirst_num = %d\n", first_num);
				pos += get_num_len(first_num);
				/* The first number was followed by a comma */
				if (line[pos] == ',')
				{
					second_num = get_num(line, pos);
					/* The first number was found and
					 * it fits the template mul(X,Y) */
					if (second_num != -1)
					{
						//printf("\tsecond_num = %d\n", second_num);
						pos += get_num_len(second_num);
						pos++;
						//printf("pos = %d\n", pos);
						/* The insctuction must end
						 * with a closing parenthesis */
						if (line[pos] == ')')
						{
							/* If the mul instruction is correct
							 * and it is located in a good area
							 * when multiplication is permitted
							 * or if the dont was not found in
							 * this line we perform multiplication */
								//printf("\t");
							for (int q = next_mul; q < pos + 1; q++)
								printf("%c", line[q]);
							printf("\n");
							line_mul_result += first_num * second_num;
						}
					}
				}
			} // if (first_num != -1)
		} // if (next_mul != -1)
	} // while (next_mul != -1)	
	printf("local mul result is: %llu\n", line_mul_result);
	return (line_mul_result);
}
