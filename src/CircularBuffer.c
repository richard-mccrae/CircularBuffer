/*	
 *  Cicrular Buffer API
 *	Author: Richard Hermstad McCrae
 *  	File:	CicularBuffer.c
 *	Date:   15/01/2020
 *
 * TODO:
 * 	Add encapsulation
 * 	update current tests, improve and simplify
 * 	add memory tests
 * 	Improve handle?
 * 	Make thread safe via MUTEXs
 *  Allows users to initialize own buffer struct? Breaks encaps
*/

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "CircularBuffer.h"

#define  PRINT_INFO		0

struct circular_buf_t {
	uint8_t * buffer;
	size_t 	head;			// Incremented when element is added
	size_t 	tail;			// Incremented when element is removed
	size_t 	size; 			// Maximum number of elements
	bool 	full;
};


/*** Circular Buffer API ***/
//TODO: Add overwrite or error if full boolean
//TODO: implement handle?
cbuf_handle_t
circular_buf_init (uint8_t* buffer, size_t size)
{
	assert(buffer && size);

	cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
	assert(cbuf);

	cbuf->buffer = buffer;
	cbuf->size = size;
	circular_buf_reset(cbuf);

	assert(circular_buf_empty(cbuf));

	return cbuf;
}

void
circular_buf_reset(cbuf_handle_t cbuf)
{
	assert(cbuf);

	cbuf->head = 0;
	cbuf->tail = 0;
	cbuf->full = false;
}

void
circular_buf_free(cbuf_handle_t cbuf)
{
	assert(cbuf);
	free(cbuf);
}

bool
circular_buf_full(cbuf_handle_t cbuf)
{
	assert(cbuf);

	return cbuf->full;
}

bool
circular_buf_empty(cbuf_handle_t cbuf)
{
	assert(cbuf);

	// Full flag not set && head ptr = tail ptr
	return (!cbuf->full && (cbuf->head == cbuf->tail));
}

size_t
circular_buf_size(cbuf_handle_t cbuf)
{
	assert(cbuf);

	return cbuf->size;
}

size_t
circular_buf_count(cbuf_handle_t cbuf)
{
	assert(cbuf);
	size_t count = -1;

	if (!cbuf->full)
	{
		count = cbuf->head - cbuf->tail;
	} else {
		count = cbuf->size;
	}
	
	return count;
}