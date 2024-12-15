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
# define MAX_GOOD_SECT_NUM	256
# define MAX_GOOD_SECT_LEN	512
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
char	*get_substr(char *str, int spos, int epos);
int		get_num_len(int num);
int		get_nums(char *line, int *nums, int arr_size);
int		muls_searcher(char *line, t_finder *state);
int		dos_searcher(char *line, t_finder *state);
int		donts_searcher(char *line, t_finder *state);
int		get_num(char *line, int pos);
t_ull	find_all_mul_instructions(char *sections[], int sect_cnt, t_finder *state);

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		ch;
	int		i;
	FILE	*fptr;

	t_finder	state;

	/* Sections where execution of mul insctuction is permitted */
	char *good_sects[MAX_GOOD_SECT_NUM + 1];

	/*  Global result of execution of all
	 *  mul operations in all lines that
	 *  were found in the file */
	t_ull	global_mul_result;

	int		cnt; /* Good section counter */
	int		do_pos;
	int		dont_pos;
	int		file_size;
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

	/*good_sects = (char **)malloc((MAX_GOOD_SECT_NUM + 1) * sizeof (char *));
	if (good_sects == NULL)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < MAX_GOOD_SECT_NUM + 1; i++)
	{
		good_sects[i] = (char *)malloc((MAX_GOOD_SECT_LEN + 1) * sizeof (char));
		if (good_sects[i] == NULL)
		{
			snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
			perror(ebuf);
			exit(EXIT_FAILURE);
		}
	} */

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

	cnt = 0;
	do_pos = 0; /* Just for the first iteration we set it as zero */
	dont_pos = -1;
	file_size = ft_strlen(file_cnt);

	/* Trying to find the first "don't" */
	dont_pos = donts_searcher(file_cnt, &state);
	/* Failed to find "don't" */
	if (dont_pos == -1)
	{
		good_sects[cnt]	= get_substr(file_cnt, do_pos, file_size - 1); /* do_pos is zero */
		cnt++;
	}
	/* The first "don't" was found */
	else
	{
loop:
		/* On the first iteration the do_pos is equal to zero */
		good_sects[cnt]	= get_substr(file_cnt, do_pos, dont_pos);
		cnt++;
		/* Trying to find the first "do" after "don't" */
		while (do_pos < dont_pos && do_pos != -1)
		{
			do_pos = dos_searcher(file_cnt, &state);
			printf("do_pos = %d\n", do_pos);
		}
		/* Failed to find "do" */
		if (do_pos == -1)
		{
			goto exit; /* if after */
		}
		/* We found the first "do" that is located after the last "don't" found */
		/* Now we need to search for the next "don't" that exists after the "do" */
		while (dont_pos < do_pos && dont_pos != -1)
		{
			dont_pos = donts_searcher(file_cnt, &state);
			printf("dont_pos = %d\n", dont_pos);
		}
		/* Failed to find "don't" */
		if (dont_pos == -1)
		{
			/* We have a "do", but have not the last "don't" restriction,
			 * so the right border be the end of the file */
			good_sects[cnt] = get_substr(file_cnt, do_pos, file_size - 1);
			cnt++;
			goto exit;
		}
		/* We found "don't" after a "do" */
		goto loop;
	}
exit:
	printf("cnt = %d\n", cnt);
	for (int i = 0; i < cnt; i++)
	{
		printf("%s\n\n", good_sects[i]);
	}

	global_mul_result = 0;
	global_mul_result += find_all_mul_instructions(good_sects, cnt, &state);
	printf("multiplication result is: %llu\n", global_mul_result);

	for (int i = 0; i < cnt - 1; i++)
		free(good_sects[i]);

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

char	*get_substr(char *str, int spos, int epos)
{
	char	*ret;
	int		i;

	ret = (char *)malloc((epos - spos + 2) * sizeof (char));
	if(ret != NULL)
	{
		i = spos;
		while (i <= epos)
		{
			ret[i - spos] = str[i];
			i++;
		}
		ret[i] = '\0';
	}
	return (ret);
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

t_ull	find_all_mul_instructions(char *sections[], int sect_cnt, t_finder *state)
{
	int		first_num;
	int		second_num;

	/* Global position in a searched line up
	 * to which the found mul 
	 * corresponds to the template mul(X,Y) */
	int		pos;

	/* The position of the next
	 * "mul(" substring found */
	int		next_mul; 

	/* The local result of the execution of all
	 * mul instructions found in the line */
	t_ull	line_mul_result; 

		line_mul_result = 0;
	for (int sect_ind = 0; sect_ind < sect_cnt; sect_ind++)
	{
		state->mul_pos = 0;
		next_mul = 0;
		first_num = 0;
		second_num = 0;
		pos = 0;

		printf("doing %s\n", sections[sect_ind]);
		while (next_mul != -1)
		{
			/* The "mul(" substring was found */
			next_mul = muls_searcher(sections[sect_ind], state);
			//printf("next_mul = %d\n", next_mul);
			if (next_mul != -1)
			{
				pos = next_mul + 4;
				first_num = get_num(sections[sect_ind], pos);
				/* The first number was found and
				 * it fits the template mul(X,Y) */
				if (first_num != -1)
				{
					//printf("\tfirst_num = %d\n", first_num);
					pos += get_num_len(first_num);
					/* The first number was followed by a comma */
					if (sections[sect_ind][pos] == ',')
					{
						second_num = get_num(sections[sect_ind], pos);
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
							if (sections[sect_ind][pos] == ')')
							{
								//printf("\t");
								for (int q = next_mul; q < pos + 1; q++)
									printf("%c", sections[sect_ind][q]);
								printf("\n");
								line_mul_result += first_num * second_num;
							}
						}
					}
				} // if (first_num != -1)
			} // if (next_mul != -1)
		} // while (next_mul != -1)	
	} // for (int sect_ind = 0; sect_ind < sect_cnt; sect_ind++)

	printf("local mul result is: %llu\n", line_mul_result);
	return (line_mul_result);
}
