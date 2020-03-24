/*	Circular Buffer test runner
 *	Author: Richard Hermstad McCrae
 *  	File:	TestRunner.c
 *	Date:   15/01/2020
 *
*/

#include <unity/unity_fixture.h>

static void runAllTests()
{
	RUN_TEST_GROUP(init);
	RUN_TEST_GROUP(enqueue);
	RUN_TEST_GROUP(dequeue);
	RUN_TEST_GROUP(full);
	RUN_TEST_GROUP(empty);
	// RUN_TEST_GROUP(free);
	RUN_TEST_GROUP(count);
	RUN_TEST_GROUP(size);
	RUN_TEST_GROUP(reset);
}


int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, runAllTests);
}
