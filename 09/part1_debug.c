#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

# define MAX_FILE_NAME_BUF	256
# define MAX_ERR_BUF_SIZE	256
# define MAX_LINE_LEN		32000

typedef unsigned long long	t_ull;
/* If id is -1 it means
 * this is not a block
 * representing a file
 * but a free space block */
typedef struct s_block
{
	int	id;
	int	pos;
}	t_block;

int	is_disk_compacted(t_block *b, int blk_num)
{
	int	fs_found;
	int	res;
	
	/* Let's assume
	 * it is compacted */
	res = 1; 

	/* There can be
	 * no file blocks
	 * after the first
	 * free block found */
	fs_found = 0; 

	for (int i = 0; i < blk_num; i++)
	{
		if (b[i].id == -1)
			fs_found = 1;
		if (b[i].id != -1 && fs_found)
		{
			res = 0;
			/* There is no point in
			 * checking any further */
			break; 
		}
	}
	return (res);
}

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


	t_block	blocks[MAX_LINE_LEN * 10];

	/* Positions of the free space
	 * blocks before rearrangement */
	int		fs_blks[MAX_LINE_LEN];

	t_ull	checksum;

	int		blk_pos;
	int		free_blk_cnt;

	/* The id value of the
	 * last NOT free space
	 * block found */
	int		last_blk_id; 

	int		fbi = 0; /* Free block index */
	int		j;


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

						fs_blks[free_blk_cnt] = blk_pos + j;
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

	/*printf("%s\n", line);

	for (int bi = 0; bi < blk_pos; bi++)
	{
		if (blocks[bi].id == -1)
			printf(".");
		else
			printf("%d", blocks[bi].id);
	}
	printf("\n");*/

	/* Let's rearrange the disk */
	for (int bi = blk_pos - 1; bi >= 0; bi--)
	{
		if (blocks[bi].id != -1)
		{
			if (is_disk_compacted(blocks, blk_pos))
				break;

			blocks[ fs_blks[fbi] ].id = blocks[bi].id;
			blocks[bi].id = -1;
			fbi++;

			/*for (int bi = 0; bi < blk_pos; bi++)
			{
				if (blocks[bi].id == -1)
					printf(".");
				else
					printf("%d", blocks[bi].id);
			}
			printf("\n");*/
		}
	}

	checksum = 0;
	/* Let's count the result checksum */
	for (int i = 0; i < blk_pos; i++)
	{
		if (blocks[i].id == -1)
			break;
		checksum += (t_ull)i * (t_ull)blocks[i].id;
	}

	printf("The resulting filesystem checksum is %llu\n", checksum);

	fclose(fptr);
	exit (EXIT_SUCCESS);
}
