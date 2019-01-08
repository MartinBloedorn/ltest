/**
 ******************************************************************************
 * @file    l_test.c
 * @author  Martin Vincent Bloedorn
 * @date    28-December-18
 * @brief   Functionality for unit testing.
 *
 ******************************************************************************
 */

/******** INCLUDES ***********************************************************/

#include "l_test.h"

/******** DEFINES ************************************************************/

/******** STATIC *************************************************************/

lTestModule * entrymodule = NULL;

#ifdef L_UNIT_TESTS
static void _exec_module(lTestModule * mod) {
	// Print module information
	__L_PRINTF(__L_COLOR_BOLD "\r\nMODULE: %s\r\n" __L_COLOR_RESET, mod->name);
	if(mod->desc) __L_PRINTF("\t%s\r\n\r\n", mod->desc);

	mod->telapsed = 0;
	mod->task(mod);

	__L_PRINTF(__L_COLOR_BOLD "\r\n%s: Passed %lu/%lu tests (%lu ms)\r\n"
			 __L_COLOR_RESET,
			mod->name, (uint32_t)mod->ngood, (uint32_t)mod->ncases,
			mod->telapsed);
}
#endif

/******** PUBLIC *************************************************************/

DISABLED_L_TEST_MODULE(self_test, \
		"Assertions designed to fail, to ensure sanity.") {

	L_TEST_CASE("L_TEST_ASSERT") {
		L_TEST_ASSERT(0);
	}

	L_TEST_CASE("L_TEST_ASSERTEQ_INT") {
		int val_a = 13;
		int val_b = 46;
		L_TEST_ASSERTEQ_INT(val_a, val_b);
	}

	L_TEST_CASE("L_TEST_ASSERTEQ_FLT") {
		float val = 1.45;
		L_TEST_ASSERTEQ_FLT(val, 2.387);
	}

	L_TEST_CASE("L_TEST_ASSERTEQ_STR") {
		const char * str = "Keanu Reaves";
		const char * ref = "Keanu Reeves";
		L_TEST_ASSERTEQ_STR(str, ref);
		// In-place strings are also allowed:
		// L_TEST_ASSERTEQ_STR(str, "Neo");
	}
}

int l_test_main(void) {
#ifdef L_UNIT_TESTS
	lTestModule * mod = entrymodule;

	__L_PRINTF("\r\n* * * * * * * * * * * * * * * * * "
			"L_TEST START * * * * * * * * * * * * * * * * *\r\n");

	while(mod) {
		if(mod->task) _exec_module(mod);
		mod = mod->next;
	}

	__L_PRINTF("\r\n* * * * * * * * * * * * * * * * * "
			" L_TEST END  * * * * * * * * * * * * * * * * *\r\n");
#endif
	return 0;
}

void __ltest_module_pushback(lTestModule * mod) {
	if(!mod) return;

	// Initialize first module
	if(!entrymodule) entrymodule = mod;
	else {
		// Link next module in list
		lTestModule * m = entrymodule;
		while(m->next) m = m->next;
		m->next = mod;
	}
}

void __ltest_case_begin(lTestModule * mod) {
	lTestCase * tcase = &mod->tcase;

	// Notify start to user
	__L_PRINTF("CASE: %s\r\n", tcase->name);

	// Clear information for test case
	tcase->nsteps 	= 0;
	tcase->ngood	= 0;
	tcase->result	= lTestResult_Pass;
	tcase->startt	= __L_MILLIS();
	tcase->doexec	= 1;

	mod->ncases++;
}

int __ltest_case_end(lTestModule * mod) {
	lTestCase * tcase = &mod->tcase;

	// If doexec is set, we're still running...
	if(tcase->doexec--) return 1;

	uint32_t telapsed = (__L_MILLIS() - tcase->startt);
	mod->telapsed += telapsed;

	// Passed if no assertion changed tcase->result
	if(tcase->result == lTestResult_Pass) {
		mod->ngood++;
		__L_PRINTF(__L_COLOR_SUCCESS __L_COLOR_BOLD "PASSED!");
	} else {
		__L_PRINTF(__L_COLOR_FAIL __L_COLOR_BOLD "FAILED!");
	}

	// Notify time:
	__L_PRINTF(" (%lu ms)\r\n" __L_COLOR_RESET, telapsed);

	return 0;
}
