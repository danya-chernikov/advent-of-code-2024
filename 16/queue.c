#include <stdio.h>

# define QUEUE_CAPACITY	1024

/* It adds the element `elem` into the queue `q` */
void	enqueue(int *q, int *rear, int elem)
{
	/* Check if the queue is full */
	if (*rear == QUEUE_CAPACITY - 1)
		return;
	(*rear)++;
	q[*rear] = elem;
}

/* It removes the front element of the queue `q`.
 * In case if the queue is empty it returns 0 to
 * indicate an error */
int		dequeue(int *q, int *rear)
{
	/* Check if the queue is empty */
	if (*rear == -1)
		return (0);
	for (int i = 0; i < *rear; i++)
		q[i] = q[i + 1];
	(*rear)--;
	return (1);
}

int		queue_get_front(int *q)
{
	return (q[0]);
}

/* It printf out all elements of the queue `q`
 * starting from front element */
void	queue_display(int *q, int *rear)
{
	if (*rear != -1)
	{
		for (int i = 0; i <= *rear; i++)
			printf("%d ", q[i]);
		printf("\n");
	}
}

int main(void)
{
	int	queue[QUEUE_CAPACITY];
	int	front;
	int	rear;

	/* Marking the queue is
	 * empty at the beginning */
	rear = -1;

	/* Let's add an element in the queue */
	enqueue(queue, &rear, 1);
	enqueue(queue, &rear, 2);
	enqueue(queue, &rear, 10);

	front = queue_get_front(queue);

	queue_display(queue, &rear);

	front = queue_get_front(queue);
	printf("front element is: %d\n", front); 
	dequeue(queue, &rear);

	front = queue_get_front(queue);
	printf("front element is: %d\n", front);
	dequeue(queue, &rear);

	enqueue(queue, &rear, 200);
	enqueue(queue, &rear, -50);
	enqueue(queue, &rear, 126);

	queue_display(queue, &rear);

	front = queue_get_front(queue);
	printf("front element is: %d\n", front); 
	dequeue(queue, &rear);

	front = queue_get_front(queue);
	printf("front element is: %d\n", front); 
	dequeue(queue, &rear);

	front = queue_get_front(queue);
	printf("front element is: %d\n", front); 
	dequeue(queue, &rear);

	queue_display(queue, &rear);

	return (0);
}
