/**
 ******************************************************************************
 * @file    filter.c
 * @author  Martin Vincent Bloedorn
 * @date    29-December-18
 * @brief   Some simple test routines.
 *
 ******************************************************************************
 */

/******** INCLUDES ***********************************************************/

#include "l_test.h"

/******** DEFINES ************************************************************/

/******** STATIC *************************************************************/

int add(int a, int b) {
	return a + b;
}

float multf(float a, float b) {
	return a*b;
}

void to_uppercase_wrong(char * str) {
	while(*str) {
		if(*str != ' ')
			*str = (char)(*str - 32);
		str++;
	}
}

/******** PUBLIC *************************************************************/

L_TEST_MODULE(simple_test, "Test routines for some basic functions.") {

	L_TEST_CASE("Simple integer math") {
		int a = 3, b = 2;

		L_TEST_ASSERT(add(a, b) > a);
		L_TEST_ASSERTEQ_INT(add(a, b), 5);
	}

	L_TEST_CASE("Simple float math") {
		float a = 3.0, b = 0.2;

		L_TEST_ASSERT(multf(a, b) < a);
		L_TEST_ASSERTEQ_FLT(multf(a, b), 0.6);
	}

	L_TEST_CASE("Some string manipulation") {
		char str1[] = "this works";
		char str2[] = "This Does Not";

		to_uppercase_wrong(str1);
		to_uppercase_wrong(str2);

		L_TEST_ASSERTEQ_STR(str1, "THIS WORKS");
		L_TEST_ASSERTEQ_STR(str2, "THIS DOES NOT");
	}
}
