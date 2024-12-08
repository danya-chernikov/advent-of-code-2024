
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

typedef long long t_ll;

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

t_ll	count_digits(t_ll num)
{
	t_ll	dnum;

	dnum = 0;
	while (num != 0)
	{
		num /= 10;	
		dnum++;
	}
	return (dnum);
}

/*t_ll	join_numbers(t_ll num1, t_ll num2)
{
	t_ll	res;
	char	num1_buf[MAX];


	return (res);
}*/

t_ll	join_nums(t_ll num1, t_ll num2)
{
	t_ll	res;
	char	bnum1[MAX_NUM_STR_LEN + 1];
	char	bnum2[MAX_NUM_STR_LEN + 1];
	char	bret[MAX_NUM_STR_LEN + 1];

	snprintf(bnum1, MAX_NUM_STR_LEN, "%lld", num1);
	snprintf(bnum2, MAX_NUM_STR_LEN, "%lld", num2);
	ft_strlcpy(bret, bnum1, MAX_NUM_STR_LEN);
	ft_strlcat(bret, bnum2, MAX_NUM_STR_LEN);
	
	res = ft_atoll(bret);
	return (res);
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

	char	ops[MAX_NUMS_STR + 1];	/* Operators */
	int		nums_found;				/* Number of numbers found in an equation */
	t_ll	eq[MAX_NUMS_STR];		/* Current equation */

	/* Number of positions (spaces)
	 * between numbers in an equation */
	int		pos_num;

	int		add_flag;
	int		perms_num;				/* Permutations number */
	t_ll	total_calib_res;		/* Total calibration result */

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

	/* Let's read the ordering rules */
	i = 0;
	ch = 0;
	line_cnt = 0;
	total_calib_res = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				i = 0;

				nums_found = get_nums(line, eq, MAX_NUMS_STR);
				if (nums_found != 1)
				{
					for (int j = 0; j < nums_found; j++)
						printf("%lld ", eq[j]);
					printf("\n");

					pos_num = nums_found - 2; /* Minus test value */
					perms_num = (int)pow((double)2, (double)pos_num);

					/* Number of bits in permutation is equal the number of positions */
					for (int perm = 0; perm < perms_num; perm++)
					{
						for (int bit_ind = 0; bit_ind < pos_num; bit_ind++)
						{
							/* Checking if the bit number
							 * `bit_ind` in `perm` number
							 * is set or not  */
							if (perm & (1 << bit_ind)) /* If it's set */
								ops[bit_ind] = '|';
							else
								ops[bit_ind] = '?';
						}

						/* ############################################################ */
						/* Now when another equation template was generated.
						 * Instead of '?' symbol we need to paste '+' or '*'
						 * symbols */
						
						/* The same as ops, contains
						 * '|' and '?' operators */
						char	ops_copy[MAX_NUMS_STR + 1];

						/* Contains only '+' and '*' operators */
						char	l_ops[MAX_NUMS_STR + 1];

						/* Let's count first how many
						 * positions do we need to fill in */

						/* number of positions
						 * where '?' is placed */
						int	l_pos_num = 0;
						for (int pi = 0; pi < pos_num; pi++)
						{
							if (ops[pi] == '?')
								l_pos_num++;
						}

						int	l_perms_num = (int)pow((double)2, (double)l_pos_num);

					    add_flag = 1;
						for (int l_perm = 0; l_perm < l_perms_num; l_perm++)
						{
							/* Let's create a copy of ops array */
							for (int ci = 0; ci < pos_num; ci++)
								ops_copy[ci] = ops[ci];

							/* Let's generate local l_ops array containing
							 * only '+' and '*' operators. But only if we
							 * are dealing with an operation array containing
							 * at least one '?' (which means: put here '+' or '*'
							 * operators) */
							if (l_pos_num > 0) // If there were at least one '?'
							{
								for (int lbi = 0; lbi < l_pos_num; lbi++)
								{
									if (l_perm & (1 << lbi))
										l_ops[lbi] = '+';
									else
										l_ops[lbi] = '*';
								}
		
								int	l_pos_ind = 0;
								/* Let's unite ops_copy array with l_pos */
								for (int q = 0; q < pos_num; q++)
								{
									if (ops_copy[q] == '?')
									{
										ops_copy[q] = l_ops[l_pos_ind];
										l_pos_ind++;
									}
								}
							}

							/* Printing the generated operation */
                            printf("\t");
							for (int j = 1; j < nums_found; j++)
							{
								printf("%lld ", eq[j]);
								if (j <= pos_num)
								{
									printf("%c ", ops_copy[j - 1]);
								}
							}
							printf("= ");

							/* Partial reults of calculations to concatenate.
							 * The results of calculations the expressions
							 * that are located between '|' operators */

							/* Calculate the result of the
							 * intermediate operation */
							t_ll	parts[MAX_NUM_STR_LEN];

							/* Result of the
							 * intermediate operation */
							t_ll	op_res; 

							int		part_ind;
							int		j;

							part_ind = 0;
							op_res = eq[1];
							j = 2;
							while (j - 1 < pos_num + 2)
							{
								if (ops_copy[j - 2] == '|' || j == nums_found)
								{
                                    //printf("parts[%d] = %lld\n", part_ind, op_res);
									parts[part_ind] = op_res;
									printf("%lld|", parts[part_ind]);
									part_ind++;
									if (j < nums_found)
										op_res = eq[j];
								}
								if (j < nums_found)
								{
									if (ops_copy[j - 2] == '+')
										op_res += eq[j];
									if (ops_copy[j - 2] == '*')
										op_res *= eq[j];
								}
								else
									break;

								j++;
							}

							printf(" = ");
                            
							/* Now let's concatenate all the parts */
							int     c;
                            t_ll    cres;

                            c = 1;
							cres = parts[0];
							while (c < part_ind)
							{
								cres = join_nums(cres, parts[c]);
								c++;
							}


							if (cres == eq[0])
							{
								if (add_flag)
									total_calib_res += cres;
								add_flag = 0;
								printf(" \033[32m%lld !!!\033[37m", cres);
							}
							else
								printf(" %lld", cres);

						    printf("\n");

						} // for (int l_perm = 0; l_perm < l_perms_num; l_perm++)

						printf("\n");

						/* ############################################################ */

					} // for (int perm = 0; perm < perms_num; perm++)
					printf("\n");
				} // if (nums_found != 1)

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

	printf("total calibration result is %lld\n", total_calib_res);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
