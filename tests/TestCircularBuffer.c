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
static uint8_t* buffer;
static cbuf_handle_t buffer_handle = NULL;
int ret, i;


//---------------------------------------------------------------------/

TEST_GROUP(init);

TEST_SETUP(init)
{
	/* initialization steps are executed before each TEST */
	buffer = malloc(size* sizeof(uint8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(uint8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(init)
{
	/* clean up steps are executed after each TEST */
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

// Test elements to enqeueue
static uint8_t element[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'
};
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
	free(buffer);
}

TEST(enqueue, return_success)
{
	overwrite = false;
	ret = cirular_buf_enqueue(buffer_handle, element[0], overwrite);

	TEST_ASSERT_EQUAL_INT(SUCCESS, ret);
}

TEST(enqueue, increment_count)
{
	cirular_buf_enqueue(buffer_handle, element[0], overwrite);
	ret = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(1, ret);
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

TEST(enqueue, overwritetrue_return_successfull_with_overwrite)
{
	overwrite = true;

	for (i=0; i <= size; i++)
	{
		ret = cirular_buf_enqueue(buffer_handle, element[i], overwrite);
	}

	TEST_ASSERT_EQUAL_INT(SUCCESS_BUFFER_FULL, ret);
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
	RUN_TEST_CASE(enqueue, increment_count);
	RUN_TEST_CASE(enqueue, overwritefalse_return_errorfull);
	RUN_TEST_CASE(enqueue, overwritefalse_count_doesnt_increment);
	RUN_TEST_CASE(enqueue, overwritefalse_is_full);
	RUN_TEST_CASE(enqueue, overwritetrue_return_successfull_with_overwrite);
	RUN_TEST_CASE(enqueue, overwritetrue_is_full_with_overwrite);
}


//---------------------------------------------------------------------/

TEST_GROUP(full);

TEST_SETUP(full)
{
	buffer = malloc(size* sizeof(uint8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(uint8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(full)
{
	free(buffer);
}

TEST(full, return_true_when_full)
{
	// Push until full
	TEST_ASSERT_TRUE(circular_buf_full(buffer_handle));
}

TEST(full, return_false_when_not_full)
{
	// First assert when empty, then push until full and pop one.
	TEST_ASSERT_FALSE(circular_buf_full(buffer_handle));
}

TEST_GROUP_RUNNER(full)
{
	RUN_TEST_CASE(full, return_true_when_full);
	RUN_TEST_CASE(full, return_false_when_not_full);
}

//---------------------------------------------------------------------/

TEST_GROUP(empty);

TEST_SETUP(empty)
{
	buffer = malloc(size* sizeof(uint8_t));
	assert(buffer);
	memset(buffer, 0, size * sizeof(uint8_t));
	buffer_handle = circular_buf_init(buffer, size);
}

TEST_TEAR_DOWN(empty)
{
	free(buffer);
}

TEST(empty, return_true_when_empty)
{
	// Push, then pop until empty
	TEST_ASSERT_TRUE(circular_buf_empty(buffer_handle));
}

TEST(empty, return_false_when_not_empty)
{
	// Push and test
	TEST_ASSERT_FALSE(circular_buf_empty(buffer_handle));
}

TEST_GROUP_RUNNER(empty)
{
	RUN_TEST_CASE(empty, return_true_when_empty);
	RUN_TEST_CASE(empty, return_false_when_not_empty);
}