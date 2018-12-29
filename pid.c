/**
 ******************************************************************************
 * @file    pid.c
 * @author  Martin Vincent Bloedorn
 * @date    29-December-18
 * @brief   Test routines for l_pid component.
 *
 ******************************************************************************
 */

/******** INCLUDES ***********************************************************/

#include "l_test.h"

#include "l_pid.h"
#include "l_debug.h"

/******** DEFINES ************************************************************/

/******** STATIC *************************************************************/

static struct lPID * pid;

/******** PUBLIC *************************************************************/

L_TEST_MODULE(pid, "Test routines for PID controllers.") {

	L_TEST_CASE("PID steady state") {
		pid = l_pid_create();
		L_TEST_ASSERT(pid);

		l_pid_init(pid, 1.0, 8.0, 0.01);
		l_pid_flush(pid);

		float y = 0.0;
		for(int i=0; i<150; i++) {
			y = l_pid_in(pid, 1.0, y);
			HAL_Delay(10);
		}

		L_TEST_ASSERTEQ_FLT(y, 1.0);
	}
}
