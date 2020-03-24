/*	Circular Buffer unit tests
 *	Author: Richard Hermstad McCrae
 *  	File:	TestCicularBuffer.c
 *	Date:   15/01/2020
 *
 * TODO: Look at unity_memory.h to test mem alloc and leaks
*/


#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define UNITY_FIXTURE_NO_EXTRAS /* Dont import unity_memory.h */
#include <unity/unity_fixture.h>
 
#include "../src/CircularBuffer.h"

#define  PRINT_INFO		0
#define  BUFFER_SIZE	10


//---------------------------------------------------------------------/

/*** Locals ***/
static const int size = BUFFER_SIZE;
static int8_t* buffer;
static cbuf_handle_t buffer_handle = NULL;
int ret, i;
// Test elements to enqeueue, dequeue, and print
static int8_t element[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'
};

//---------------------------------------------------------------------/

TEST_GROUP(init);

TEST_SETUP(init)
{
	/* initialization steps are executed before each TEST */
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(init)
{
	/* clean up steps are executed after each TEST */
	circular_buf_free(buffer_handle);
	free(buffer);
}


TEST(init, initialized_empty)
{
	TEST_ASSERT_TRUE(circular_buf_empty(buffer_handle));
}

TEST(init, buffer_size)
{
	ret = circular_buf_size(buffer_handle);

	TEST_ASSERT_EQUAL_INT(BUFFER_SIZE, ret);
}

TEST(init, count_init_to_zero)
{
	ret = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(0, ret);
}

TEST_GROUP_RUNNER(init)
{
	RUN_TEST_CASE(init, initialized_empty);
	RUN_TEST_CASE(init, buffer_size);
	RUN_TEST_CASE(init, count_init_to_zero);
}


//---------------------------------------------------------------------/

TEST_GROUP(enqueue);

static bool overwrite;

TEST_SETUP(enqueue)
{
	buffer = malloc(size* sizeof(uint8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(uint8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(enqueue)
{
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(enqueue, return_success)
{
	overwrite = false;
	ret = cirular_buf_enqueue(buffer_handle, element[0], overwrite);

	TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
}

TEST(enqueue, overwritefalse_return_errorfull)
{
	overwrite = false;

	for (i=0; i <= size; i++)
	{
		ret = cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	TEST_ASSERT_EQUAL_INT(ERROR_BUFFER_FULL, ret);
}

TEST(enqueue, overwritefalse_count_doesnt_increment)
{
	overwrite = false;

	for (i=0; i <= size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	ret = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(size, ret);
}

TEST(enqueue, overwritefalse_is_full)
{
	overwrite = false;

	for (i=0; i <= size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	TEST_ASSERT_TRUE(circular_buf_full(buffer_handle));
}

TEST(enqueue, overwritetrue_return_success_with_overwrite)
{
	overwrite = true;

	for (i=0; i <= size; i++)
	{
		ret = cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	TEST_ASSERT_EQUAL_INT(SUCCESS_BUFFER_FULL, ret);
}

TEST(enqueue, overwritetrue_count_doesnt_increment)
{
	overwrite = true;

	for (i=0; i <= (size+1); i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	ret = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(size, ret);
}

TEST(enqueue, overwritetrue_is_full_with_overwrite)
{
	overwrite = false;

	for (i=0; i <= size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	TEST_ASSERT_TRUE(circular_buf_full(buffer_handle));
}

//TODO: Add printing to test enqueue

TEST_GROUP_RUNNER(enqueue)
{
	RUN_TEST_CASE(enqueue, return_success);
	RUN_TEST_CASE(enqueue, overwritefalse_return_errorfull);
	RUN_TEST_CASE(enqueue, overwritefalse_count_doesnt_increment);
	RUN_TEST_CASE(enqueue, overwritefalse_is_full);
	RUN_TEST_CASE(enqueue, overwritetrue_return_success_with_overwrite);
	RUN_TEST_CASE(enqueue, overwritetrue_count_doesnt_increment);
	RUN_TEST_CASE(enqueue, overwritetrue_is_full_with_overwrite);
}


//---------------------------------------------------------------------/

TEST_GROUP(dequeue);
	// Return element for dequeue
	int8_t actual = -1;

TEST_SETUP(dequeue)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(dequeue)
{
	circular_buf_free(buffer_handle);
	free(buffer);
	actual = -1;
}

TEST(dequeue, return_success_when_not_empty)
{
	int8_t expected = element[0];

	cirular_buf_enqueue(buffer_handle, expected, false);
	ret = circular_buf_dequeue(buffer_handle, &actual);

	TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
}

TEST(dequeue, return_bufferempty_when_empty)
{
	ret = circular_buf_dequeue(buffer_handle, &actual);

	TEST_ASSERT_EQUAL_INT(BUFFER_EMPTY, ret);
}

TEST(dequeue, correct_elemement_dequeued_single_enqueue)
{
	int8_t expected = element[0];

	cirular_buf_enqueue(buffer_handle, expected, false);
	circular_buf_dequeue(buffer_handle, &actual);

	TEST_ASSERT_EQUAL_INT(expected, actual);
}

TEST(dequeue, no_element_returned_when_empty)
{
	int8_t result = actual;
	ret = circular_buf_dequeue(buffer_handle, &result);

	TEST_ASSERT_EQUAL_INT(actual, result);
}

TEST(dequeue, correct_element_dequeued_after_overwrite)
{
	int8_t expected = element[2];

	// Enqueue 12 elements, tail should then be at index 2
	for (i=0; i<=size+1; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], true);
	}
	circular_buf_dequeue(buffer_handle, &actual);

	TEST_ASSERT_EQUAL_INT(expected, actual);
}


TEST_GROUP_RUNNER(dequeue)
{
	RUN_TEST_CASE(dequeue, return_success_when_not_empty);
	RUN_TEST_CASE(dequeue, return_bufferempty_when_empty);
	RUN_TEST_CASE(dequeue, correct_elemement_dequeued_single_enqueue);
	RUN_TEST_CASE(dequeue, no_element_returned_when_empty);
	RUN_TEST_CASE(dequeue, correct_element_dequeued_after_overwrite);
}


//---------------------------------------------------------------------/

TEST_GROUP(full);

TEST_SETUP(full)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(full)
{
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(full, return_true_when_full)
{
	// Enquque until full
	for (i=0; i<size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], true);
	}
	
	TEST_ASSERT_TRUE(circular_buf_full(buffer_handle));
}

TEST(full, return_false_when_empty)
{
	TEST_ASSERT_FALSE(circular_buf_full(buffer_handle));
}

TEST(full, return_false_with_some_elements)
{
	// Enquque a few elements
	for (i=0; i<4; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], true);
	}

	TEST_ASSERT_FALSE(circular_buf_full(buffer_handle));
}

TEST(full, return_false_after_full_then_dequeue)
{
	int8_t data;

	// Enquque until full, then dequeue an element
	for (i=0; i<size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], true);
	}
	circular_buf_dequeue(buffer_handle, &data);

	TEST_ASSERT_FALSE(circular_buf_full(buffer_handle));
}

TEST_GROUP_RUNNER(full)
{
	RUN_TEST_CASE(full, return_true_when_full);
	RUN_TEST_CASE(full, return_false_when_empty);
	RUN_TEST_CASE(full, return_false_with_some_elements);
	RUN_TEST_CASE(full, return_false_after_full_then_dequeue);
}

//---------------------------------------------------------------------/

TEST_GROUP(empty);

TEST_SETUP(empty)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(empty)
{
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(empty, return_true_when_empty)
{
	TEST_ASSERT_TRUE(circular_buf_empty(buffer_handle));
}

TEST(empty, return_false_when_not_empty)
{
	cirular_buf_enqueue(buffer_handle, element[0], false);

	TEST_ASSERT_FALSE(circular_buf_empty(buffer_handle));
}

TEST(empty, return_true_after_dequeing)
{
	int8_t data;
	cirular_buf_enqueue(buffer_handle, element[0], false);
	circular_buf_dequeue(buffer_handle, &data);
	
	TEST_ASSERT_TRUE(circular_buf_empty(buffer_handle));
}

TEST_GROUP_RUNNER(empty)
{
	RUN_TEST_CASE(empty, return_true_when_empty);
	RUN_TEST_CASE(empty, return_false_when_not_empty);
	RUN_TEST_CASE(empty, return_true_after_dequeing);
}

//---------------------------------------------------------------------/

//TODO: Memory freeing test
TEST_GROUP(free);

TEST_SETUP(free)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(free)
{
	free(buffer);
}

TEST(free, ptr_null_after_freeing)
{
	circular_buf_free(buffer_handle);

	TEST_ASSERT_EQUAL_PTR(NULL, buffer_handle);
}

TEST_GROUP_RUNNER(free)
{
	RUN_TEST_CASE(free, ptr_null_after_freeing);
}

//---------------------------------------------------------------------/

TEST_GROUP(count);
int8_t count = -1;

TEST_SETUP(count)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(count)
{
	count = -1;
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(count, count_zero_when_empty)
{
	count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(0, count);
}

TEST(count, increment_count)
{
	cirular_buf_enqueue(buffer_handle, element[0], false);
	count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(1, count);

	cirular_buf_enqueue(buffer_handle, element[0], false);
	count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(2, count);
}

TEST(count, decrement_count)
{
	int8_t expected = element[0];

	cirular_buf_enqueue(buffer_handle, expected, false);
	circular_buf_dequeue(buffer_handle, &actual);
	count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(0, count);
}

TEST(count, count_maxsize_after_overwrite)
{
	for (i=0; i<=(size+1); i++)
	{
		cirular_buf_enqueue(buffer_handle, element[i], true);
	}
	
	count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(size, count);
}

TEST_GROUP_RUNNER(count)
{
	RUN_TEST_CASE(count, count_zero_when_empty);
	RUN_TEST_CASE(count, increment_count);
	RUN_TEST_CASE(count, decrement_count);
	RUN_TEST_CASE(count, count_maxsize_after_overwrite);
}

//---------------------------------------------------------------------/

TEST_GROUP(size);

TEST_SETUP(size)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(size)
{
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(size, size_correct)
{
	TEST_ASSERT_EQUAL_PTR(size, circular_buf_size(buffer_handle));
}

TEST_GROUP_RUNNER(size)
{
	RUN_TEST_CASE(size, size_correct);
}

//---------------------------------------------------------------------/

TEST_GROUP(reset);

TEST_SETUP(reset)
{
	buffer = malloc(size* sizeof(int8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(int8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(reset)
{
	circular_buf_free(buffer_handle);
	free(buffer);
}

TEST(reset, empty_after_reset)
{
	cirular_buf_enqueue(buffer_handle, element[1], false);
	circular_buf_reset(buffer_handle);
	
	TEST_ASSERT_TRUE(circular_buf_empty(buffer_handle));
}

TEST(reset, no_longer_full_after_reset)
{
	for (i=0; i<size; i++)
	{
		cirular_buf_enqueue(buffer_handle, element[1], false);
	}
	assert(circular_buf_full(buffer_handle));
	
	circular_buf_reset(buffer_handle);
	
	TEST_ASSERT_FALSE(circular_buf_full(buffer_handle));
}

TEST(reset, head_equal_tail_after_reset)
{
	int8_t enqueue = element[0];
	int8_t data;

	circular_buf_reset(buffer_handle);

	cirular_buf_enqueue(buffer_handle, enqueue, false);
	circular_buf_dequeue(buffer_handle, &data);

	TEST_ASSERT_EQUAL_INT(enqueue, data);
}

TEST_GROUP_RUNNER(reset)
{
	RUN_TEST_CASE(reset, empty_after_reset);
	RUN_TEST_CASE(reset, no_longer_full_after_reset);
	RUN_TEST_CASE(reset, head_equal_tail_after_reset);
}