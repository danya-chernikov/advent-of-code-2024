#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF		256
# define MAX_ERR_BUF_SIZE		256
# define MAX_NUM_STR_LEN		16
# define MAX_LINE_LEN			1024
# define MAX_LINE_NUM			512
# define MAX_MAP_WIDTH			64
# define MAX_MAP_HEIGHT			64
# define MAX_MOVES_LINE_LEN		16384

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN + 1];
	int		line_cnt;

	/* The length of the last
	 * line read from a file */
	int		line_len; 
	int		ch;
	int		i;
	FILE	*fptr;

	/* A map representing
	 * page ordering rules */
	char	map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	char	moves[MAX_MOVES_LINE_LEN];
	int		width; /* Actual width of the map */
	int		height; /* Actual height of the map */
	int		moves_line_cnt;
	int		r_x; /* x position of the robot */
	int		r_y; /* y position of the robot */


	if (argc != 2)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Usage: %s FILENAME", argv[0]);
		printf("%s\n", ebuf);
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

	/*r = (t_map *)malloc(MAX_LINE_NUM * sizeof (t_map));
	if (!r)
	{
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to allocate memory");
		perror(ebuf);
		exit(EXIT_FAILURE);
	}*/

	/* Let's read the map */
	i = 0;
	ch = 0;
	line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				map[line_cnt][i] = '\0';
				line_len = i;
				i = 0;
				if (line_cnt == 0)
					width = line_len;

				line_cnt++;

				if (line_len == 0) /* The map section ended */
					break;

				continue;
			}
			else
			{
				line_len = i;
				map[line_cnt][i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			map[line_cnt][i] = ch;
		else
		{
			line_len = i;
			map[line_cnt][i] = '\0';
			break;
		}
		i++;
	}
	/* We substracting one because
	 * there is an empty line dividing
	 * the map section and moves */
	height = line_cnt - 1; 

	printf("map width = %d\n", width);
	printf("map height = %d\n", height);

	printf("\n");
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
			printf("%c", map[yi][xi]);
		printf("\n");
	}
	printf("\n");

	moves[0] = '\0';
	
	/* Let's read the moves */
	i = 0;
	ch = 0;
	moves_line_cnt = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == '\n')
		{
			if (line_cnt < MAX_LINE_NUM)
			{
				line[i] = '\0';
				line_len = i;
				i = 0;

				ft_strlcat(moves, line, MAX_MOVES_LINE_LEN);

				moves_line_cnt++;

				continue;
			}
			else
			{
				line_len = i;
				line[i] = '\0';
				break;
			}
		}
		if (i < MAX_LINE_LEN)
			line[i] = ch;
		else
		{
			line_len = i;
			line[i] = '\0';
			break;
		}
		i++;
	}

	printf("We got %d moves lines\n", moves_line_cnt);

	printf("\n%s\n", moves);


	fclose(fptr);
	exit (EXIT_SUCCESS);
}
