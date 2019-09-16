#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

t_queue* queue_create(const unsigned int nbElements, long unsigned int elemSize)
{
	t_queue* queue = malloc(sizeof(*queue));

	queue->data = malloc(nbElements * elemSize);
	queue->nbElements = 0;
	queue->nextIndex = 0;
	queue->elemSize = elemSize;

	return queue;
}

void queue_add(t_queue* queue)
{
	if (queue != NULL)
	{
		if (queue->nextIndex >= queue->nbElements)
		{
			#ifdef _DEBUG_
			printf("font reallocating\n");
			#endif
			if (queue->data != NULL)
				queue->data = realloc(queue->data, queue->elemSize * (queue->nbElements+1));
			else
			{
				#ifdef _DEBUG_
				printf("*queue->data == NULL\n");
				#endif
			}
		}
	}
}

void queue_destroy(t_queue* queue)
{
	if (queue == NULL)
		return;
	free(queue->data);
	free(queue);
}
