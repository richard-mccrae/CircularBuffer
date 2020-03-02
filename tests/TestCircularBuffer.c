/*	Nordic Semiconductor
 *	
 * 	Assignment:	Circular Buffer in C
 *	Author: Richard Hermstad McCrae
 *  	File:	TestCicularBuffer.c
 *	Date:   15/01/2020
*/


#include <stdlib.h>
#define UNITY_FIXTURE_NO_EXTRAS /* Dont import unity_memory.h */
#include <unity/unity_fixture.h>
 
#include "../src/CircularBuffer.h"

#define  PRINT_INFO		0

//---------------------------------------------------------------------/

/*** Locals ***/
static int ret;
static queue_t *q;
static size_t queue_size = 10;

TEST_GROUP(init);

TEST_SETUP(init)
{
	/* initialization steps are executed before each TEST */
	ret = 0;
	q = malloc(sizeof(queue_t));
}

TEST_TEAR_DOWN(init)
{
	/* clean up steps are executed after each TEST */
	free(q->buffer);
	free(q);
}

TEST(init, queue_uninitialized)
{
	//testing enqueue return not uninitialization....neccessary test? 
	ret = enqueue(q, 5);
	TEST_ASSERT_EQUAL_INT(QUEUE_UNINTIALIZED, ret);
}


TEST(init,  buffer)
{
	init(q,queue_size);
	
	TEST_ASSERT_NOT_NULL(q->buffer);
}

TEST(init,  buffersize)
{
	init(q,queue_size);
	
	TEST_ASSERT_EQUAL_INT(BUFFER_SIZE, q->size);
}

TEST(init,  endbuffer)
{
	init(q,queue_size);
	
	TEST_ASSERT_EQUAL_INT(q->buffer_end, q->buffer + q->size);
}

TEST(init, empty)
{
	init(q,queue_size);
	
	TEST_ASSERT_EQUAL_INT(0, q->count);
	TEST_ASSERT_EQUAL_INT(q->buffer, q->data_start);
	TEST_ASSERT_EQUAL_INT(q->buffer, q->data_end);
	TEST_ASSERT_EQUAL_PTR(q->data_end, q->data_start);
}

//test memory allocation
//test buffer size init
TEST(init, size)
{
	init(q, queue_size);

	TEST_ASSERT_EQUAL_INT(queue_size, q->size);
}
//test buffer data type init


TEST_GROUP_RUNNER(init)
{
	//TODO: Test names
	RUN_TEST_CASE(init, queue_uninitialized);
	RUN_TEST_CASE(init, buffer);
	RUN_TEST_CASE(init, );
	RUN_TEST_CASE(init, );
	RUN_TEST_CASE(init, );
	RUN_TEST_CASE(init, empty);
	RUN_TEST_CASE(init, size);
}

//---------------------------------------------------------------------/

TEST_GROUP(enqueue);

TEST_SETUP(enqueue)
{
	ret = 0;
	q = malloc(sizeof(queue_t));
	init(q, queue_size);
}

TEST_TEAR_DOWN(enqueue)
{
	free(q->buffer);
	free(q);
}

TEST(enqueue, value_enqueue)
{
	int 		ret = -1; 
	int			val = 4;

	ret = enqueue(q, val);
	TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
	TEST_ASSERT_EQUAL_INT(val, *q->data_start);
	TEST_ASSERT_EQUAL_PTR(q->buffer, q->data_start);
	TEST_ASSERT_EQUAL_INT(0, *q->data_end);
}

TEST(enqueue, data_end_shift)
{
	int			stop = 4;
	
	// Enqueue arbitray number of elements
	for (int i=0; i<stop; i++)
	{
		enqueue(q, i);
	}
	
	TEST_ASSERT_EQUAL_PTR(q->buffer + stop,q->data_end);
}

TEST(enqueue, count_increment)
{
	enqueue(q, 9);
	
	TEST_ASSERT_EQUAL_INT(1, q->count);
}

TEST(enqueue, queue_full)
{
	for (int i=0; i<(q->size+1); i++)
	{
		ret = enqueue(q, i);
	}
	
	TEST_ASSERT_EQUAL_INT(QUEUE_IS_FULL, ret);
	TEST_ASSERT_EQUAL_INT(q->size, q->count);
}

TEST(enqueue, buffer_wraps)
{
	// Enqueue until full, data_end should equal buffer
	for (int i=0; i<q->size; i++)
	{
		enqueue(q, i);
	}

	TEST_ASSERT_EQUAL_PTR(q->buffer, q->data_end);
}

TEST_GROUP_RUNNER(enqueue)
{
	RUN_TEST_CASE(enqueue, value_enqueue);
	RUN_TEST_CASE(enqueue, data_end_shift);
	RUN_TEST_CASE(enqueue, count_increment);
	RUN_TEST_CASE(enqueue, queue_full);
	RUN_TEST_CASE(enqueue, buffer_wraps);
}


//---------------------------------------------------------------------/

TEST_GROUP(dequeue);

static int enqueue_val, dequeue_val, i;

TEST_SETUP(dequeue)
{
	ret = 0;
	q = malloc(sizeof(queue_t));
	init(q, queue_size);
	enqueue_val = 11;
	dequeue_val = 0;
}

TEST_TEAR_DOWN(dequeue)
{
	free(q->buffer);
	free(q);
}

TEST(dequeue, value_dequeue)
{	
	enqueue(q, enqueue_val);
	ret = dequeue(q, &dequeue_val);
	
	TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
	TEST_ASSERT_EQUAL_INT(enqueue_val, dequeue_val);
}

TEST(dequeue, data_start_shift)
{
	// Enqueue arbitray number of elements
	for (i=0; i<3; i++)
	{
		enqueue(q, i);
	}
	dequeue(q, &dequeue_val);

	// A single dequeue should have shifted data_start once
	TEST_ASSERT_EQUAL_PTR(++q->buffer, q->data_start);
	q->buffer--;
} 

TEST(dequeue, count_decrement)
{
	enqueue(q, enqueue_val);
	dequeue(q, &dequeue_val);
	
	TEST_ASSERT_EQUAL_INT(0, q->count);
}

TEST(dequeue, queue_empty)
{
	ret = dequeue(q, &dequeue_val);
	TEST_ASSERT_EQUAL_INT(QUEUE_IS_EMPTY, ret);
}

TEST(dequeue, buffer_wraps)
{
	// Enqueue until full, data_end and data_start equal buffer address
	for (i=0; i<q->size; i++)
	{
		enqueue(q, i);
	}
	// Dequeue until empty to shift and wrap data_start
	for (i=0; i<q->size; i++)
	{
		dequeue(q, &dequeue_val);
	}

	TEST_ASSERT_EQUAL_PTR(q->buffer, q->data_start);
}

TEST_GROUP_RUNNER(dequeue)
{
	RUN_TEST_CASE(dequeue, value_dequeue);
	RUN_TEST_CASE(dequeue, data_start_shift);
	RUN_TEST_CASE(dequeue, count_decrement);
	RUN_TEST_CASE(dequeue, queue_empty);
	RUN_TEST_CASE(dequeue, buffer_wraps);
}

//---------------------------------------------------------------------/

TEST_GROUP(is_empty);

static int enqueue_val, dequeue_val, i;
static bool bIsEmpty;

TEST_SETUP(is_empty)
{
	ret = 0;
	q = malloc(sizeof(queue_t));
	init(q, queue_size);
	enqueue_val = 11;
	dequeue_val = 0;
	bIsEmpty = false;
}

TEST_TEAR_DOWN(is_empty)
{
	free(q->buffer);
	free(q);
}


TEST(is_empty, return_true)
{
	bIsEmpty = is_empty(q);
	TEST_ASSERT_TRUE(bIsEmpty);
}

TEST(is_empty, return_false)
{
	enqueue(q, enqueue_val);
	bIsEmpty = is_empty(q);	
	TEST_ASSERT_FALSE(bIsEmpty);
}


TEST_GROUP_RUNNER(is_empty)
{
	RUN_TEST_CASE(is_empty, return_true);
	RUN_TEST_CASE(is_empty, return_false);
}

//---------------------------------------------------------------------/

/* Just a final test for fun... */
TEST_GROUP(use);

static int dequeue_val, i;
static bool bIsEmpty;

TEST_SETUP(use)
{
	ret = 0;
	q = malloc(sizeof(queue_t));
	init(q, queue_size);
	i = 0;
	dequeue_val = 0;
	bIsEmpty = false;
}

TEST_TEAR_DOWN(use)
{
	free(q->buffer);
	free(q);
}

TEST(use, multiple_enqueue_dequeue)
{
	if ( PRINT_INFO){
		printf("Beginning of 'multiple_enqueue_dequeue' test run\n");
		printf("buffer: %p, data_end: %p, data_start: %p\n", q->buffer, q->data_end, q->data_start);
		printf("Enqueuing\n");
	}
	// Enqueue until full
	for (i=1; i<=q->size; i++)
	{
		ret = enqueue(q, i);
		TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
		TEST_ASSERT_EQUAL_INT(i, q->count);	
		if ( PRINT_INFO){
			printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p\n",
				q->buffer, 
				i,
				q->data_end, 
				*q->data_start,
				q->data_start
			);
		}	
	}
	TEST_ASSERT_FALSE(bIsEmpty);
	TEST_ASSERT_EQUAL_INT(8, q->count);
	TEST_ASSERT_EQUAL_PTR(q->buffer, q->data_end);
	TEST_ASSERT_EQUAL_PTR(q->buffer, q->data_start);
	
	// Dequeue aribtary number of times
	if ( PRINT_INFO){
		printf("Dequeuing\n");
	}
	for (i=1; i<=5; i++)
	{
		ret = dequeue(q, &dequeue_val);
		TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
		TEST_ASSERT_EQUAL_INT(8-i, q->count);
		if ( PRINT_INFO){
			printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p\n",
				q->buffer, 
				*q->data_end,
				q->data_end, 
				*q->data_start,
				q->data_start
			);	
		}
	}
	// Enqueue again
	if ( PRINT_INFO){
		printf("Enqueue again...\n");
	}
	for (i=11; i<=18; i++)
	{
		ret = enqueue(q, i);
		if ( PRINT_INFO){
			printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p\n",
				q->buffer, 
				i,
				q->data_end, 
				*q->data_start,
				q->data_start
			);	
		}	
	}
	TEST_ASSERT_EQUAL_INT(QUEUE_IS_FULL, ret);
	
	// Dequeue until past empty...
	if ( PRINT_INFO){
		printf("Prior to a final dequeue round\n");
		printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p, count=%d\n",
			q->buffer, 
			*q->data_end,
			q->data_end, 
			*q->data_start,
			q->data_start,
			q->count
		);	
		printf("Beging dequeuing...\n");
	}
	for (i=1; i<=9; i++)
	{
		ret = dequeue(q, &dequeue_val);
		if ( PRINT_INFO){
			printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p, count:%d\n",
				q->buffer, 
				*q->data_end,
				q->data_end, 
				*q->data_start,
				q->data_start,
				q->count
			);	
		}
	}
	TEST_ASSERT_EQUAL_INT(QUEUE_IS_EMPTY, ret);
	TEST_ASSERT_TRUE(is_empty(q));
	if ( PRINT_INFO){
		printf("buffer: %p, data_end=%d @ %p, data_start=%d @ %p\n",
			q->buffer, 
			*q->data_end,
			q->data_end, 
			*q->data_start,
			q->data_start
		);	
	}
}

TEST_GROUP_RUNNER(use)
{
	RUN_TEST_CASE(use, multiple_enqueue_dequeue);
}
