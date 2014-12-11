/**********************************************************************//**
 * @file unit_test.c
 *************************************************************************/

#include "test_unit.h"
#include "../../sensors/thermometer/tc72.h"
#include "../../util/time/time.h"

uint16_t tests_run = 0U;

/* Prototypes */
static char* test_tc72_calc(void);
static char* test_UTC_sec_calc(void);

/**********************************************************************//**
 * @ingroup test_unit_pub
 *************************************************************************/
char* run_all_test(void)
{
	mu_run_test(test_tc72_calc);
	mu_run_test(test_UTC_sec_calc);

	return "ALL TESTS PASSED";
}

/**********************************************************************//**
 * @ingroup test_unit_priv
 * @brief Test of function: calculate_temperature(uint8_t MSB, uint8_t LSB) in tc72.c
 * @return char* - if test pass: 0
			     - if test fail: the error code line where it fails
 * @note Test parameters is taken from tc72 datasheet
 *************************************************************************/
static char* test_tc72_calc(void)
{
	mu_assert( "Err-01-100", calculate_temperature(0b11001001, 0b00000000) == -55.00 );
	mu_assert( "Err-01-101", calculate_temperature(0b11100111, 0b00000000) == -25.00 );
	mu_assert( "Err-01-102", calculate_temperature(0b11111111, 0b11000000) == -0.25 );
	mu_assert( "Err-01-103", calculate_temperature(0b00000000, 0b00000000) == 0.00 );
	mu_assert( "Err-01-104", calculate_temperature(0b00000000, 0b01000000) == 0.25 );
	mu_assert( "Err-01-105", calculate_temperature(0b00000000, 0b10000000) == 0.50 );
	mu_assert( "Err-01-106", calculate_temperature(0b00011001, 0b00000000) == 25.00 );
	mu_assert( "Err-01-107", calculate_temperature(0b01111101, 0b00000000) == 125.00 );

	return 0;
}

/**********************************************************************//**
 @ingroup test_unit_priv
 @brief Test of function: calc_UTC_seconds(FIXED_TIME *buf) in time.
 @return char* - if tests pass: 0
			   - if test fail: the error code line where it fails
 @note Test parameters is calculated using http://www.epochconverter.com/
 *************************************************************************/
static char* test_UTC_sec_calc(void)
{
	FIXED_TIME t0 = {1970,1,1,0,0,0};
	mu_assert( "Err-02-100", calc_UTC_seconds(&t0) == 0 );

	FIXED_TIME t1 = {2014,12,11,10,56,36};
	mu_assert( "Err-02-101", calc_UTC_seconds(&t1) == 1418295396 );

	FIXED_TIME t2 = {2006,4,12,3,21,8};
	mu_assert( "Err-02-102", calc_UTC_seconds(&t2) == 1144812068 );

	FIXED_TIME t3 = {1989,10,1,22,3,59};
	mu_assert( "Err-02-103", calc_UTC_seconds(&t3) == 623282639 );

	FIXED_TIME t4 = {2048,2,25,2,9,1};
	mu_assert( "Err-02-104", calc_UTC_seconds(&t4) == 2466209341 );

	FIXED_TIME t5 = {1982,7,18,11,24,13};
	mu_assert( "Err-02-105", calc_UTC_seconds(&t5) == 395839453 );

	FIXED_TIME t6 = {2014,12,12,12,0,0};
	mu_assert( "Err-02-106", calc_UTC_seconds(&t6) == 1418385600 );

	FIXED_TIME t7 = {2106,2,7,6,28,15};
	mu_assert( "Err-02-107", calc_UTC_seconds(&t7) == 4294967295  );

	return 0;
}