#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		32000

/* If id is -1 it means
 * this is not a block
 * representing a file
 * but a free space block */
typedef struct s_block
{
	int	id;
	int	pos;
}	t_block;

int	main(int argc, char *argv[])
{
	char	filename[MAX_FILE_NAME_BUF + 1];
	char	ebuf[MAX_ERR_BUF_SIZE + 1];
	char	line[MAX_LINE_LEN];

	/* The length of the last
	 * line read from a file */
	int		line_len; 

	int		ch;
	int		i;
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
		snprintf(ebuf, MAX_ERR_BUF_SIZE, "Unable to open the file %s", argv[1]);
		perror(ebuf);
		exit(EXIT_FAILURE);
	}

	t_block	blocks[MAX_LINE_LEN * 10];

	/* Positions of the free
	 * space blocks before
	 * rearrangement */
	int		fs_blks_pos[MAX_LINE_LEN];
	int		blk_pos;
	int		free_blk_cnt;

	/* The id value of the
	 * last NOT free space
	 * block found */
	int		last_blk_id; 
	int		j;

	i = 0;
	ch = 0;
	blk_pos = 0;
	free_blk_cnt = 0;
	last_blk_id = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (i < MAX_LINE_LEN)
		{
			if (ft_isdigit(ch))
			{
				/* If `i` is odd, `ch`
				 * represents the number
				 * of free space blocks */
				if (i % 2 != 0)
				{
					j = 0;
					while (j < (ch - '0'))
					{
						blocks[blk_pos + j].id = -1;
						blocks[blk_pos + j].pos = blk_pos + j;
						fs_blks_pos[free_blk_cnt] = blk_pos;
						free_blk_cnt++;
						j++;
					}
					blk_pos += j;
				}
				/* If `i` is even, `ch`
				 * represents the number
				 * of file blocks */
				else if (i % 2 == 0)
				{
					j = 0;
					while (j < (ch - '0'))
					{
						blocks[blk_pos + j].id = last_blk_id;
						blocks[blk_pos + j].pos = blk_pos + j;
						j++;
					}
					blk_pos += j;
					last_blk_id++;
				}
			} // if (ft_isdigit(ch))

		} // if (i < MAX_LINE_LEN)
		else
			break;

		line[i] = ch;
		i++;
	} // while ((ch = fgetc(fptr)) != EOF)

	line[i] = '\0';
	line_len = i - 1; /* Minus end of line symbol */

	printf("%s\n", line);

	printf("\n");
	for (int bi = 0; bi < blk_pos; bi++)
	{
		if (blocks[bi].id == -1)
			printf(".");
		else
			printf("%d", blocks[bi].id);
	}
	printf("\n");

		


	fclose(fptr);
	exit (EXIT_SUCCESS);
}
