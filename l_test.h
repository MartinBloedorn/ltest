/**
 ******************************************************************************
 * @file    l_test.h
 * @author  Martin Vincent Bloedorn
 * @date    28-December-18
 * @brief   Functionality for unit testing.
 *
 ******************************************************************************
 */

#ifndef L_TEST_H_
#define L_TEST_H_

/******** INCLUDES ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "l_utils.h"

/******** DEFINES ************************************************************/

/// Supply your printf compliant function
#define __L_PRINTF(...)			printf(__VA_ARGS__)

// Supply a function to return a wall time (in ms)
#define __L_MILLIS				millis

/// Enable/disable all tests by commenting this symbol
/// #define L_UNIT_TESTS

#define __L_COLOR_BOLD 		"\033[1m"
#define __L_COLOR_RESET 	"\033[0m"
#define __L_COLOR_SUCCESS 	"\033[32m"
#define __L_COLOR_FAIL 		"\033[31m"
#define __L_COLOR_MAYBE 	"\033[35m"
#define __L_COLOR_DESC 		__L_COLOR_BOLD "\033[33m"

/// Declares a test module block. Cases are added via the L_TEST_CASE macro.
#if defined(L_UNIT_TESTS)
#define L_TEST_MODULE(mname, mdesc) \
	void __ltest_tmod_task_##mname(lTestModule *); \
	static lTestModule __ltest_tmod_##mname = { \
			.name = #mname, \
			.desc = #mdesc, \
			.task = __ltest_tmod_task_##mname, \
			.next = NULL, \
	}; \
	__attribute__((constructor)) \
	static void __ltest_ctor_##mname() { \
		__ltest_module_pushback(&__ltest_tmod_##mname); \
	} \
	void __ltest_tmod_task_##mname(lTestModule * __tmod)
#else
	#define L_TEST_MODULE(mname, mdesc) \
	void __attribute__((unused)) __ltest_tmod_task_##mname(lTestModule * __tmod)
#endif

/// To simplify disabling a test module
#define DISABLED_L_TEST_MODULE(mname, mdesc) \
	void __attribute__((unused)) __ltest_tmod_task_##mname(lTestModule * __tmod)

/// Declares a test case. Must be used within a L_TEST_MODULE block.
#define L_TEST_CASE(cname) \
	__tmod->tcase.name 		= cname; \
	for(__ltest_case_begin(__tmod); __ltest_case_end(__tmod);)

/// Prints a simple formatted, customizable error message
#define __L_TEST_PRINT_FAIL_ARGS(lnum, fname, ...) \
	__L_PRINTF(__L_COLOR_FAIL "[ERROR] In %s:%lu", fname, lnum); \
	__L_PRINTF(__VA_ARGS__); \
	__L_PRINTF("\r\n"__L_COLOR_RESET);

/// Asserts that val != 0; updates the information on the ongoing test case.
#define __L_TEST_BASE_ASSERT_ARGS(val, ...) \
	do { uint32_t lineno= __LINE__; \
		const char * fn = __FILE__; \
		(void)lineno; (void)fn; \
		 if(!val) { \
			 __tmod->tcase.result = lTestResult_Fail; \
			__ltest_case_end(__tmod); \
			__L_TEST_PRINT_FAIL_ARGS(lineno, fn, __VA_ARGS__); \
		 }\
	} while(0); \
	/* If assert failed, break out of for loop  */\
	if(__tmod->tcase.result == lTestResult_Fail) break;

#define L_TEST_ASSERT(val) \
	__L_TEST_BASE_ASSERT_ARGS((val), "\r\n\tAssertion failed.")

#define L_TEST_ASSERTN(val)	\
	L_TEST_ASSERT(!(val))

#define L_TEST_ASSERTEQ_INT(val, ref) \
	__L_TEST_BASE_ASSERT_ARGS((val == ref), \
			"\r\n\tAssertion failed. Expected " #ref "; got %d.", val);

#define L_TEST_ASSERTEQ_FLT(val, ref) \
	__L_TEST_BASE_ASSERT_ARGS((fabs(val - ref)/ref < 0.001), \
			"\r\n\tAssertion failed. " \
			"Expected " #ref "; got %d.%03d (>.1%% error)", \
			((int)val), (((int)(1000.0*val))%1000));

#define L_TEST_ASSERTEQ_BUF(val, ref, size) \
	__L_TEST_BASE_ASSERT_ARGS(!memcmp(val, ref, size), \
			"\r\n\tAssertion failed. Buffer mismatch.");

#define L_TEST_ASSERTEQ_STR(val, ref) \
	__L_TEST_BASE_ASSERT_ARGS(!strcmp(val, ref), \
			"\r\n\tAssertion failed. String mismatch." \
			"\r\n\tExpected: %s" \
			"\r\n\t     Got: %s", ref, val);

/******** ENUMS & STRUCTS ****************************************************/

typedef enum {
	lTestResult_Undefined = -1,
	lTestResult_Fail = 0,
	lTestResult_Pass = 1,
} lTestResult_e;

typedef struct lTestModule_s 	lTestModule;
typedef struct lTestCase_s 		lTestCase;

struct lTestCase_s {
	const char *	name;
	size_t 			nsteps;		///< Number of asserts used in case
	size_t			ngood;		///< Number of passed assertions
	uint32_t		startt;		///< Timestamp of test start
	lTestResult_e 	result;		///< Overall case result
	int				doexec;		///< Flag for test case for loop
};

struct lTestModule_s {
	const char * 	name;					///< Current module's name
	const char * 	desc;					///< Current module's description
	lTestCase		tcase;					///< Info on the active case
	void 			(*task)(lTestModule *);	///< Test task, with defined cases
	size_t			ncases;					///< Amount of test cases ran
	size_t			ngood;					///< Amount of test cases passed
	lTestModule * 	next;					///< Next test module
	uint32_t		telapsed;				///< Total elapsed time on module
};

/******** PUBLIC *************************************************************/

/** \brief Entry point for all tests. _write syscall must be implemented. */
int l_test_main(void);

/** \brief Adds module to queue of tests. Is automatically called from macro */
void __ltest_module_pushback(lTestModule * mod);

/** \brief Initializes a test case. Is automatically called from macro. */
void __ltest_case_begin(lTestModule * mod);

/** \brief Finalizes a test case. Is automatically called from macro. */
int  __ltest_case_end(lTestModule * mod);

#endif /* L_TEST_H_ */
