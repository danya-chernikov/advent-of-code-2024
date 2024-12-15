/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 20:10:25 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/01 20:10:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		16

void	insert_sort(int *a, int size)
{
	int	i;
	int	j;
	int	tmp;

	i = 1;
	while (i < size)
	{
		tmp = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > tmp)
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = tmp;
		i++;
	}
}

void	get_nums(char *line, int *num1, int *num2)
{
	char	snum1[16];
	char	snum2[16];
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (ft_isdigit(line[i]) && line[i] != '\0')
	{
		snum1[j] = line[i];
		j++;
		i++;
	}
	snum1[j] = '\0';
	while (!ft_isdigit(line[i]))
	{
		i++;
	}
	j = 0;
	while (ft_isdigit(line[i]) && line[i] != '\0')
	{
		snum2[j] = line[i];
		j++;
		i++;
	}
	snum2[j] = '\0';
	*num1 = ft_atoi(snum1);
	*num2 = ft_atoi(snum2);
}

int main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		ch;
	int		i;
	int		line_cnt;
	int		arr_ind;
	int		num1, num2;
	int		*left, *right;
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
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[0]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	// first let's count lines in the file
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
			line_cnt++;
	}

	left = (int *)malloc((line_cnt + 1) * sizeof(int));
	if (left == NULL)
	{
		ft_strlcpy(ebuf, "Unable to allocate memory", MAX_ERR_BUF_SIZE);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	right = (int *)malloc((line_cnt + 1) * sizeof(int));
	if (right == NULL)
	{
		ft_strlcpy(ebuf, "Unable to allocate memory", MAX_ERR_BUF_SIZE);
		perror(ebuf);
		free(left);
		exit(EXIT_FAILURE);
	}

	arr_ind = 0;
	i = 0;
	ch = 0;
	rewind(fptr);
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			line[i] = '\0';
			i = 0;
			get_nums(line, &num1, &num2);
			left[arr_ind] = num1;
			right[arr_ind] = num2;
			arr_ind++;
			continue;
		}
		if (i < MAX_LINE_LEN)
		{
			line[i] = ch;
		}
		i++;
	}

	insert_sort(left, arr_ind);
	insert_sort(right, arr_ind);

	unsigned long long	total_dist = 0;
	i = 0;
	while (i < arr_ind)
	{
		total_dist += (unsigned long long)abs(left[i] - right[i]);
		i++;
	}
	printf("total distance = %llu\n", total_dist);

	free(left);
	free(right);
	fclose(fptr);
	exit (EXIT_SUCCESS);
}
