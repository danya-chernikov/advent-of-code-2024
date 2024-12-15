#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* MAX_NUMS_STR - the maximum
 * number of numbers in a string */
# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_NUM_STR_LEN	16
# define MAX_LINE_LEN		256
# define MAX_LINE_NUM		256
# define MAX_NUMS_STR		32

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];

	/* Number of lines read from a file */
	int		line_cnt;

	/* The length of the last
	 * line read from a file */
	int		line_len; 

	int		ch;
	int		i;
	FILE	*fptr;

	char	m[MAX_LINE_LEN][MAX_LINE_NUM];
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

	/* Let's read the ordering rules */
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
				line_len = i;
				i = 0;

				line_cnt++;

				if (line_len == 0) /* The ordering rule section ended */
					break;

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

	printf("width = %d\n", width);
	printf("height = %d\n", height);

	printf("\n");

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	/* Let's determine the initial position of the guard */
	int	x;
	int	y;
	int	found = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m[i][j] == '^')
			{
				x = j;
				y = i;
				found = 1;
				break;
			}
		}
		if (found)
			break;
	}

	printf("x = %d\n", x);
	printf("y = %d\n", y);

	printf("\n");

	/* Direction of the guard.
	 *     d = 0 - up;
	 *     d = 1 - right;
	 *     d = 2 - down;
	 *     d = 3 - left. */
	int	d = 0;

	int	pos_cnt = 0;

	while ((x >= 0 && x < width) && (y >= 0 && y < height))
	{
		/* Trying to step up */
		if (d == 0)
		{
			if (m[y - 1][x] == '#')
			{
				d = (d + 1) % 4;
			}
			else
			{
				if (m[y][x] != 'X')
				{
					m[y][x] = 'X';
					pos_cnt++;
				}
				y--;
			}
			continue;
		}
		/* Trying to step right */
		if (d == 1)
		{
			if (m[y][x + 1] == '#')
			{
				d = (d + 1) % 4;
			}
			else
			{
				if (m[y][x] != 'X')
				{
					m[y][x] = 'X';
					pos_cnt++;
				}
				x++;
			}
			continue;
		}
		/* Trying to step down */
		if (d == 2)
		{
			if (m[y + 1][x] == '#')
			{
				d = (d + 1) % 4;
			}
			else
			{
				if (m[y][x] != 'X')
				{
					m[y][x] = 'X';
					pos_cnt++;
				}
				y++;
			}
			continue;
		}
		/* Trying to step left */
		if (d == 3)
		{
			if (m[y][x - 1] == '#')
			{
				d = (d + 1) % 4;
			}
			else
			{
				if (m[y][x] != 'X')
				{
					m[y][x] = 'X';
					pos_cnt++;
				}
				x--;
			}
			continue;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("number of distinct positions is %d\n", pos_cnt);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
