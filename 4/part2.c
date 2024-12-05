/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:31:46 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/05 03:59:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		256
# define MAX_LINE_NUM		256

int	alg(char (*m)[MAX_LINE_NUM], int px, int py)
{
	int	xmas_cnt = 0;



	// 1 + 2
	if (((m[py][px] == m[py + 2][px]) && (m[py][px] == 'M') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py + 2][px]) && (m[py][px] == 'S') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

		if (((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'M') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'S') &&
			(m[py + 1][px + 1] == 'A')) ||
			((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'S') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'M') &&
			(m[py + 1][px + 1] == 'A')))
		{
			xmas_cnt+=2;
			return (xmas_cnt);
		}
	}


	// 1 + 4
	if (((m[py][px] == m[py + 2][px]) && (m[py][px] == 'M') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py + 2][px]) && (m[py][px] == 'S') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{
		if (((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'M') &&
			(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'S') &&
			(m[py + 1][px + 1] == 'A')) ||
			((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'S') &&
			(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'M') &&
			(m[py + 1][px + 1] == 'A')))
		{
			xmas_cnt+=2;
			return (xmas_cnt);
		}
	}


	// 3 + 2
	if (((m[py][px] == m[py][px + 2]) && (m[py][px] == 'M') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py][px + 2]) && (m[py][px] == 'S') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

		if (((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'M') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'S') &&
			(m[py + 1][px + 1] == 'A')) ||
			((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'S') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'M') &&
			(m[py + 1][px + 1] == 'A')))
		{
			xmas_cnt+=2;
			return (xmas_cnt);
		}
	}

	// 3 + 4
	if (((m[py][px] == m[py][px + 2]) && (m[py][px] == 'M') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py][px + 2]) && (m[py][px] == 'S') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

		if (((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'M') &&
			(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'S') &&
			(m[py + 1][px + 1] == 'A')) ||
			((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'S') &&
			(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'M') &&
			(m[py + 1][px + 1] == 'A')))
		{
			xmas_cnt+=2;
			return (xmas_cnt);
		}
		
	}

	// 1
	if (((m[py][px] == m[py + 2][px]) && (m[py][px] == 'M') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py + 2][px]) && (m[py][px] == 'S') &&
		(m[py][px + 2] == m[py + 2][px + 2]) && (m[py][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

			xmas_cnt++;
			return (xmas_cnt);
	}

	// 2
	if (((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'M') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'S') &&
			(m[py + 1][px + 1] == 'A')) ||
			((m[py + 1][px] == m[py][px + 1]) && (m[py + 1][px] == 'S') &&
			(m[py + 2][px + 1] == m[py + 1][px + 2]) && (m[py + 2][px + 1] == 'M') &&
			(m[py + 1][px + 1] == 'A')))
		{
			xmas_cnt++;
			return (xmas_cnt);
		}


	// 3
	if (((m[py][px] == m[py][px + 2]) && (m[py][px] == 'M') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py][px] == m[py][px + 2]) && (m[py][px] == 'S') &&
		(m[py + 2][px + 2] == m[py + 2][px]) && (m[py + 2][px + 2] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

			xmas_cnt++;
			return (xmas_cnt);
	}

	// 4
	if (((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'M') &&
		(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'S') &&
		(m[py + 1][px + 1] == 'A')) ||
		((m[py + 1][px] == m[py + 2][px + 1]) && (m[py + 1][px] == 'S') &&
		(m[py][px + 1] == m[py + 1][px + 2]) && (m[py][px + 1] == 'M') &&
		(m[py + 1][px + 1] == 'A')))
	{

			xmas_cnt++;
			return (xmas_cnt);
	}

	// ############################################################################



	return (xmas_cnt);
}

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	int		line_cnt;
	int		line_len; /* The length of the last line read from a file */
	int		ch;
	int		i;
	FILE	*fptr;

	char	m[MAX_LINE_NUM][MAX_LINE_LEN];
	char	**f; /* The matrix which state represents the found XMAS */
	int		width;
	int		height;

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

	i = 0;
	ch = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				m[line_cnt][i] = '\0';
				line_cnt++;
				line_len = i;
				i = 0;
				
				continue;
			}
			else
			{
				line_len = i;
				m[line_cnt][i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			m[line_cnt][i] = ch;
		else
		{
			line_len = i;
			m[line_cnt][i] = '\0';
			break;
		}
		i++;
	}

	width = line_len;
	height = line_cnt;

	f = (char **)malloc(height * sizeof (char *));
	if (!f)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < height; i++)
	{
		f[i] = (char *)ft_calloc(width, sizeof (char));
		if (!f)
		{
			snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
			perror(ebuf);
			exit(EXIT_FAILURE);
		}
	} 

	int	xmas_cnt = 0;

	/* Algorithm itself */
	for (int i = 0; i < height - 2; i++)
	{
		for (int j = 0; j < width - 2; j++)
		{
			xmas_cnt += alg(m, j, i);
		}
	}

	printf("XMAS appears %d times\n", xmas_cnt);

	for (int i = 0; i < height; i++)
		free(f[i]);
	free(f);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
