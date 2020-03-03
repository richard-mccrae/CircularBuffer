/*	Nordic Semiconductor
 *	
 * 	Assignment:	Circular Buffer in C
 *	Author: Richard Hermstad McCrae
 *  File:	CicularBuffer.h
 *	Date:   15/01/2020
*/


#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_UNINTIALIZED -1
#define SUCCESS				0
#define NULL_PTR			-2
#define QUEUE_IS_EMPTY		1
#define QUEUE_IS_FULL		2

#define	BUFFER_SIZE			8


/*** Circular Buffer API ***/
typedef struct queue {
	int *			buffer;				/* MEM location constant, not value */
	int*			buffer_end;			
	int*	        data_start; 		/* First element */
    int*	        data_end; 			/* 1 past the last element */
	unsigned int 	count;				/* Number of elements in queue*/
	size_t	     	size;				/* Capacity of queue */
} queue_t;

int 	init(queue_t* q, size_t size);
int		enqueue(queue_t* q, int val);
int 	dequeue(queue_t* q, int* val);
bool	is_empty(queue_t* q);


#endif /* CIRCULAR_BUFFER_H_ */
