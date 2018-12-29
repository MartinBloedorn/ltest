/**
 ******************************************************************************
 * @file    flash.c
 * @author  Martin Vincent Bloedorn
 * @date    29-December-18
 * @brief   Test routines for d_flash driver.
 *
 ******************************************************************************
 */

/******** INCLUDES ***********************************************************/

#include "l_test.h"

#include "d_flash.h"
#include <string.h>

/******** DEFINES ************************************************************/

#define _BUFFER_SIZE 	32

/******** STATIC *************************************************************/

static int32_t taddr = 250;
static uint8_t buffer[_BUFFER_SIZE];
static const char * data = "Some test data";

/******** PUBLIC *************************************************************/

L_TEST_MODULE(flash, "SPI flash memory testing and validation routines.") {

	L_TEST_CASE("Flash initialization") {
		L_TEST_ASSERT(L_OK(d_flash_init()));
	}

	L_TEST_CASE("Flash sector erase") {
		L_TEST_ASSERT(L_OK(d_flash_erase_sector(taddr)));
	}

	L_TEST_CASE("Flash write") {
		L_TEST_ASSERT(L_OK(d_flash_write(taddr, (uint8_t*)data, strlen(data)+1)));
	}

	L_TEST_CASE("Flash read") {
		L_TEST_ASSERT(L_OK(d_flash_read(taddr, buffer, _BUFFER_SIZE)));
		L_TEST_ASSERTEQ_STR((char *)buffer, data);
	}
}
