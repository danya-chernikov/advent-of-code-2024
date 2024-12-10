#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		64
# define MAX_LINE_NUM		64

typedef struct s_point
{
	int	X;
	int	Y;

}	t_point;

int	look_up(char (*m)[MAX_LINE_NUM], int x, int y, int ch);
int	look_right(char (*m)[MAX_LINE_NUM], int w, int x, int y, int ch);
int	look_down(char (*m)[MAX_LINE_NUM], int h, int x, int y, int ch);
int	look_left(char (*m)[MAX_LINE_NUM], int x, int y, int ch);
int nine_was_found(t_point *nines_found, int *nine_cnt, t_point nine);
int	find_all_nines(char (*m)[MAX_LINE_NUM],
                   int w,
                   int h,
                   int x,
                   int y,
                   int ch,
                   t_point *nines_found,
                   int *nine_cnt);



int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	int		line_cnt;
	int		line_len; /* The length of the last line read from a file */
	int		ch;
	int		i;
	FILE	*fptr;


	t_point nines_found[MAX_LINE_NUM * MAX_LINE_LEN];
	char	m[MAX_LINE_NUM][MAX_LINE_LEN];
	int		width;
	int		height;
    int     nine_cnt;
    int     trail_heads_sum;

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


    trail_heads_sum = 0;
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			if (m[yi][xi] == '0')
			{
                nine_cnt = 0;
                find_all_nines(m, width, height, xi, yi, '0', nines_found, &nine_cnt);
                trail_heads_sum += nine_cnt;
			}
		}
	}
    
    printf("trail heads sum = %d\n", trail_heads_sum);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}

int	look_up(char (*m)[MAX_LINE_NUM], int x, int y, int ch)
{
	int	res;

	res = 0;
	if (y - 1 >= 0)
		if (m[y - 1][x] == ch)
			res = 1;
	return (res);
}

int	look_right(char (*m)[MAX_LINE_NUM], int w, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (x + 1 < w)
		if (m[y][x + 1] == ch)
			res = 1;
	return (res);
}

int	look_down(char (*m)[MAX_LINE_NUM], int h, int x, int y, int ch)
{
	int	res;

	res = 0;
	if (y + 1 < h)
		if (m[y + 1][x] == ch)
			res = 1;
	return (res);
}

int	look_left(char (*m)[MAX_LINE_NUM], int x, int y, int ch)
{
	int	res;

	res = 0;
	if (x - 1 >= 0)
		if (m[y][x - 1] == ch)
			res = 1;
	return (res);
}

int nine_was_found(t_point *nines_found, int *nine_cnt, t_point nine)
{
    int found;

    found = 0;
    for (int i = 0; i < *nine_cnt; i++)
    {
        if (nines_found[i].X == nine.X && nines_found[i].Y == nine.Y)
            found = 1;
    }
    return (found);
}

int	find_all_nines(char (*m)[MAX_LINE_NUM],
                   int w,
                   int h,
                   int x,
                   int y,
                   int ch,
                   t_point *nines_found,
                   int *nine_cnt)
{

	int     score;
	
	score = 0;
	if (m[y][x] == '9' &&
        !nine_was_found(nines_found, nine_cnt, (t_point){.X=x, .Y=y}))
	{
        nines_found[*nine_cnt].X = x;
        nines_found[*nine_cnt].Y = y;
        (*nine_cnt)++;
		score++;
        return (score);
	}

    for (int hi = 0; hi < 9; hi++) /* Height index */
    {
        if (ch == hi + '0')
        {
            /* Looking up */
            if (look_up(m, x, y, (hi + 1) + '0'))
            {
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x,
                                        y - 1,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
            }
            /* Looking right */
            if (look_right(m, w, x, y, (hi + 1) + '0'))
            {
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x + 1,
                                        y,
                                        (hi + 1) + '0',
                                        nines_found, nine_cnt);
            }
            /* Looking down */
            if (look_down(m, h, x, y, (hi + 1) + '0'))
            {
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x,
                                        y + 1,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
            }
            /* Looking left */
            if (look_left(m, x, y, (hi + 1) + '0'))
            {
                score += find_all_nines(m,
                                        w,
                                        h,
                                        x - 1,
                                        y,
                                        (hi + 1) + '0',
                                        nines_found,
                                        nine_cnt);
            }
        }
    }

	return (score);
}
