/*	Nordic Semiconductor
 *	
 * 	Assignment:	Circular Buffer in C
 *	Author: Richard Hermstad McCrae
 *  	File:	CicularBuffer.c
 *	Date:   15/01/2020
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
 
#include "CircularBuffer.h"

#define  PRINT_INFO		0


/*** Circular Buffer API ***/

void
init (queue_t* q, size_t size)
{
	assert(0 != q);
	q->size = size;
	q->buffer = calloc(q->size, sizeof(q->buffer));
	// Queue size is one element more than defined by BUFFER_SIZE
	q->buffer_end = q->buffer + q->size;
	q->data_start = q->buffer;
	q->data_end = q->buffer;
	q->count=0;
}


int
enqueue(queue_t* q, int val)
{
	if (q == NULL || q->buffer == NULL)
	{
		return QUEUE_UNINTIALIZED;
	}
	if (q->count >= q->size)
	{
		return QUEUE_IS_FULL;
	}

	*q->data_end = val;
	q->count++;
	
	if (PRINT_INFO) 
		printf("Enqueued %d to data_end at MEM: %p\n", *q->data_end, q->data_end);
		
	q->data_end++;
	
	// Wrap queue
	if (q->data_end == q->buffer_end)
	{
		if (PRINT_INFO)
			printf("data_end wrapping...\n");
		q->data_end = q->buffer;
	}

	if (PRINT_INFO) 
	{
		printf("data_end now at MEM: %p\n", q->data_end);
		printf("count is now: %d\n", q->count);	
	}	
	
	return SUCCESS;
}

int
dequeue(queue_t* q, int* val)
{
	if (q == NULL || q->buffer == NULL)
	{
		return QUEUE_UNINTIALIZED;
	}
	if (q->count == 0)
	{
		return QUEUE_IS_EMPTY;
	}
	
	if (PRINT_INFO)
	{
		printf("Dequeue %d from data_start MEM: %p\n", *q->data_start, q->data_start);
	}
	
	*val = *q->data_start;
	q->data_start++;
	q->count--;
	
	// Wrap
	if(q->data_start == q->buffer_end)
	{
		if (PRINT_INFO)
			printf("data_start wrapping...\n");
		q->data_start = q->buffer;
	}
	
	if (PRINT_INFO)
	{
		printf("Count now: %d, Data_start MEM now: %p\n", q->count, q->data_start);
	}
	
	return SUCCESS;
}


bool
is_empty(queue_t* q)
{
	assert(q != NULL);
	assert(q->buffer != NULL);

	if (q->count > 0 )
		return false;
	else
		return true;
}
