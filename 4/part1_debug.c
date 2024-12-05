/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:31:32 by dchernik          #+#    #+#             */
/*   Updated: 2024/12/05 00:32:00 by dchernik         ###   ########.fr       */
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

int	alg(char (*m)[MAX_LINE_NUM], int w, int h, int px, int py, char **f)
{
	int	xmas_cnt = 0;

	/* Going north */
	if ((py > 0) && (m[py - 1][px] == 'M'))
	{
		if ((py - 1 > 0) && (m[py - 2][px] == 'A'))
		{
			if ((py - 2 > 0) && (m[py - 3][px] == 'S')) /* XMAS was found! */
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py - 1][px] = 1;
				f[py - 2][px] = 1;
				f[py - 3][px] = 1;
			}
		}
	}
	/* Going northeast */
	if ((py > 0) && (px < w - 1) && (m[py - 1][px + 1] == 'M'))
	{
		if ((py - 1 > 0) && (px + 1 < w - 1) && (m[py - 2][px + 2] == 'A'))
		{
			if ((py - 2 > 0) && (px + 2 < w - 1) && (m[py - 3][px + 3] == 'S')) /* XMAS was found! */
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py - 1][px + 1] = 1;
				f[py - 2][px + 2] = 1;
				f[py - 3][px + 3] = 1;
			}
		}
	}
	/* Going east */
	if ((px < w - 1) && (m[py][px + 1] == 'M'))
	{
		if ((px + 1 < w - 1) && (m[py][px + 2] == 'A'))
		{
			if ((px + 2 < w - 1) && (m[py][px + 3] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py][px + 1] = 1;
				f[py][px + 2] = 1;
				f[py][px + 3] = 1;
			}
		}
	}
	/* Going southeast */
	if ((px < w - 1) && (py < h - 1) && (m[py + 1][px + 1] == 'M'))
	{
		if ((px + 1 < w - 1) && (py + 1 < h - 1) && (m[py + 2][px + 2] == 'A'))
		{
			if ((px + 2 < w - 1) && (py + 2 < h - 1) && (m[py + 3][px + 3] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py + 1][px + 1] = 1;
				f[py + 2][px + 2] = 1;
				f[py + 3][px + 3] = 1;
			}
		}
	}
	/* Going south  */
	if ((py < h - 1) && (m[py + 1][px] == 'M'))
	{
		if ((py + 1 < h - 1) && (m[py + 2][px] == 'A'))
		{
			if ((py + 2 < h - 1) && (m[py + 3][px] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py + 1][px] = 1;
				f[py + 2][px] = 1;
				f[py + 3][px] = 1;
			}
		}
	}
	/* Going southwest */
	if ((py < h - 1) && (px > 0) && (m[py + 1][px - 1] == 'M'))
	{
		if ((py + 1 < h - 1) && (px - 1 > 0) && (m[py + 2][px - 2] == 'A'))
		{
			if ((py + 2 < h - 1) && (px - 2 > 0) && (m[py + 3][px - 3] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py + 1][px - 1] = 1;
				f[py + 2][px - 2] = 1;
				f[py + 3][px - 3] = 1;
			}
		}
	}
	/* Going west */
	if ((px > 0) && (m[py][px - 1] == 'M'))
	{	
		if ((px - 1 > 0) && (m[py][px - 2] == 'A'))
		{
			if ((px - 2 > 0) && (m[py][px - 3] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py][px - 1] = 1;
				f[py][px - 2] = 1;
				f[py][px - 3] = 1;
			}
		}
	}
	/* Going northwest */
	if ((px > 0) && (py > 0) && (m[py - 1][px - 1] == 'M'))
	{
		if ((px - 1 > 0) && (py - 1 > 0) && (m[py - 2][px - 2] == 'A'))
		{
			if ((px - 2 > 0) && (py - 2 > 0) && (m[py - 3][px - 3] == 'S'))
			{
				xmas_cnt++;
				f[py][px] = 1;
				f[py - 1][px - 1] = 1;
				f[py - 2][px - 2] = 1;
				f[py - 3][px - 3] = 1;
			}
		}
	}
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

	printf("width = %d\n", width);
	printf("height = %d\n", height);


	/*printf("\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	int	xmas_cnt = 0;

	/* Algorithm itself */
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m[i][j] == 'X')
			{
				//printf("m[%d][%d] = X\n", i, j);
				/* Looking from X in all directions in search of 'M' */
				xmas_cnt += alg(m, width, height, j, i, f);
			}
		}
	}

	printf("xmas_cnt = %d\n", xmas_cnt);


	/*printf("\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%d", f[i][j]);
		}
		printf("\n");
	}
	printf("\n");


	printf("\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (f[i][j])
				printf("\033[32m%c\033[37m", m[i][j]);
			else
				printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	for (int i = 0; i < height; i++)
		free(f[i]);
	free(f);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
