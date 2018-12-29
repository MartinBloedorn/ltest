/**
 ******************************************************************************
 * @file    filter.c
 * @author  Martin Vincent Bloedorn
 * @date    29-December-18
 * @brief   Test routines for l_filter component.
 *
 ******************************************************************************
 */

/******** INCLUDES ***********************************************************/

#include "l_test.h"

#include "l_filter.h"

/******** DEFINES ************************************************************/

/******** STATIC *************************************************************/

static lFilter fir, iir;
static float coeff[]= {0.2, 0.2, 0.2, 0.2, 0.2};						   //5
static float data[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; //10

/******** PUBLIC *************************************************************/

L_TEST_MODULE(filter, "Test routines for FIR and IIR filters.") {

	L_TEST_CASE("FIR steady state") {
		L_TEST_ASSERT(l_filter_init_fir(&fir, coeff, 5));

		float y;
		for(int i=0; i<10; i++) y = l_filter_in(&fir, 1.0);

		L_TEST_ASSERTEQ_FLT(y, 1.0);
	}

	L_TEST_CASE("FIR using l_filter_eval") {
		l_filter_flush(&fir);
		l_filter_evalf(&fir, data, data, 10);

		L_TEST_ASSERTEQ_FLT(data[0], coeff[0]);
		L_TEST_ASSERTEQ_FLT(data[5], 1.0);
	}

	L_TEST_CASE("IIR steady state") {
		L_TEST_ASSERT(l_filter_init_iirlp(&iir, 20.0, 0.005, lFilterOrder_1));

		float y;
		for(int i=0; i<20; i++) y = l_filter_in(&iir, 1.0);

		L_TEST_ASSERTEQ_FLT(y, 1.0);
	}
}
