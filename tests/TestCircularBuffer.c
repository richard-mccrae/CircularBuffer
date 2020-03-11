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
	int ret_size = circular_buf_size(buffer_handle);

	TEST_ASSERT_EQUAL_INT(BUFFER_SIZE, ret_size);
}

TEST(init, count_init_to_zero)
{
	int ret_count = circular_buf_count(buffer_handle);

	TEST_ASSERT_EQUAL_INT(0, ret_count);
}

TEST_GROUP_RUNNER(init)
{
	RUN_TEST_CASE(init, initialized_empty);
	RUN_TEST_CASE(init, buffer_size);
	RUN_TEST_CASE(init, count_init_to_zero);
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