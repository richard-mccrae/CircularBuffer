/*	Circular Buffer API header
 *	
 *	Author: Richard Hermstad McCrae
 *  File:	CicularBuffer.h
 *	Date:   15/01/2020
 *
 *  TODO:   Add element printing
 *          User defines element data type
 *              User provides buffer struct to init funct, therefor must
 *              user is also responsible for strct mem allocation
 *          Thread safety
*/


#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdlib.h>
#include <stdbool.h>


#define QUEUE_UNINTIALIZED          -1
#define SUCCESS				        0
#define BUFFER_EMPTY		        1
#define ERROR_BUFFER_FULL 		    -2
#define SUCCESS_BUFFER_FULL         2

// Opache circular buffer structure
typedef struct circular_buf_t circular_buf_t;

// Handle declaration, a pointer to the struct
typedef circular_buf_t* cbuf_handle_t;

// Pass in a storage buffer address and size
// User must allocate memeory for buffer themselves, we will allocate
//  for the circular buffer structure memory
// Returns a circular buffer handle
cbuf_handle_t circular_buf_init (uint8_t* buffer, size_t size);

// Free circular buffer structure from memory
// Does not free data buffer, that is user's responsibility
void circular_buf_free (cbuf_handle_t cbuf);

// Reset circular buffer to empty, head == tail
void circular_buf_reset (cbuf_handle_t cbuf);

// Enquque element
// Bool flag if overwriting is desired, or a return of BUFFER_FULL
int cirular_buf_enqueue (cbuf_handle_t cbuf, uint8_t element, bool overwrite);

// Dequeue element, write to passed element reference
// Returns SUCCESS or BUFFER_EMPTY
int circular_buf_dequeue (cbuf_handle_t cbuf, uint8_t * element );

// Returns if buffer is full or not
bool circular_buf_full (cbuf_handle_t cbuf);

// Returns if buffer is empty or not
bool circular_buf_empty (cbuf_handle_t cbuf);

// Returns maximum capacity of buffer
size_t circular_buf_size (cbuf_handle_t cbuf);

// Returns number of elements currently in buffer
size_t circular_buf_count (cbuf_handle_t cbuf);


#endif /* CIRCULAR_BUFFER_H_ */
