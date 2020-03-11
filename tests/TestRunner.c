/*	Nordic Semiconductor
 *	
 * 	Assignment:	Circular Buffer in C
 *	Author: Richard Hermstad McCrae
 *  	File:	TestRunner.c
 *	Date:   15/01/2020
*/

#include <unity/unity_fixture.h>

static void runAllTests()
{
	RUN_TEST_GROUP(init);
	// RUN_TEST_GROUP(full);
}


int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, runAllTests);
}
