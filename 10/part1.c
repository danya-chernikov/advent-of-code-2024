#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		64
# define MAX_LINE_NUM		64

struct s_point
{
	int	X;
	int	Y;

}	t_point;

int	look_up(char (*m)[MAX_LINE_NUM], int w, int h, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (y - 1 >= 0)
		if (m[y - 1][x] == '1')
			res = 1;
	return (res);
}

int	look_right(char (*m)[MAX_LINE_NUM], int w, int h, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (x + 1 < w)
		if (m[y][x + 1] == ch)
			res = 1;
	return (res);
}

int	look_down(char (*m)[MAX_LINE_NUM], int w, int h, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (y + 1 < h)
		if (m[y + 1][x] == ch)
			res = 1;
	return (res);
}

int	look_left(char (*m)[MAX_LINE_NUM], int w, int h, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (x - 1 >= 0)
		if (m[y][x - 1] == ch)
			res = 1;
	return (res);
}

int	find_all_nines(char (*m)[MAX_LINE_NUM], int w, int h, int x, int y, int ch)
{
	int	score;
	
	score = 0;
	if (m[y][x] == '9' && !nine_was_found(x, y))
	{
		score++;
	}

	if (ch == 1)
	{
		/* Looking up */
		if (look_up(m, w, h, x, y, ch))
			find_all_nines(m, w, h, x, y - 1, '2');
		/* Looking right */
		if (look_right(m, w, h, x, y, ch))
			find_all_nines(m, w, h, x + 1, y, '2');
		/* Looking down */
		if (look_down(m, w, h, x, y + 1, ch))
			find_all_nines(m, w, h, x, y + 1, '2');
		/* Looking left */
		if (look_left(m, w, h, x - 1, y, ch))
			find_all_nines(m, w, h, x - 1, y, '2');
	}

	if (ch == 2)
	{
		/* Looking up */
		if (look_up(m, w, h, x, y, ch))
			find_all_nines(m, w, h, x, y - 1, '3');
		/* Looking right */
		if (look_right(m, w, h, x, y, ch))
			find_all_nines(m, w, h, x + 1, y, '3');
		/* Looking down */
		if (look_down(m, w, h, x, y + 1, ch))
			find_all_nines(m, w, h, x, y + 1, '3');
		/* Looking left */
		if (look_left(m, w, h, x - 1, y, ch))
			find_all_nines(m, w, h, x - 1, y, '3');
	}





	return (score);
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

	t_point	found_nines[MAX_LINE_LEN];

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

	/*printf("width = %d\nheight = %d\n", width, height);

	printf("\n");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			printf("%c", m[i][j]);
		printf("\n");
	}*/

	int	score;

	score = 0;
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			if (m[yi][xi] == '0')
			{

			}
		}
	}
	


	fclose(fptr);
	exit (EXIT_SUCCESS);
}
