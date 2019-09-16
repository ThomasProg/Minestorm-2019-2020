#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct s_queue
{
	void* data;
	unsigned int nextIndex;
	unsigned int nbElements;
	long unsigned int elemSize;
} t_queue;

t_queue* queue_create(const unsigned int nbFonts, long unsigned int elemSize);
void     queue_add(t_queue* queue);
void     queue_destroy(t_queue* queue);

#endif
