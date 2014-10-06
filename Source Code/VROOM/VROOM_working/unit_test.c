/********************************************//**
@file unit_test.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup ut unit_test
@{
	This is the unit test platform for all relevant functions 
	(including arithmetic) in the WROOM system.
@}
@note Complies MISRO 2004 standards
************************************************/

#include "unit_test.h"

uint16_t tests_run = 0U;
/********************************************************************************************************************//**
 @ingroup ut
 @brief Test of function: calculate_temperature(uint8_t MSB, uint8_t LSB) in tc72.c
 @return char* - if all tests pass: "01-1PASS" 
			   - if test fail: the error code line where it fails
 @note Test parameters is taken from tc72 datasheet
 ************************************************************************************************************************/
char* test_tc72_calc(void)
{
	mu_assert( "Err-01-100", calculate_temperature(0b11001001, 0b00000000) == -55.00 );
	mu_assert( "Err-01-101", calculate_temperature(0b11100111, 0b00000000) == -25.00 );
	mu_assert( "Err-01-102", calculate_temperature(0b11111111, 0b11000000) == -0.25 );
	mu_assert( "Err-01-103", calculate_temperature(0b00000000, 0b00000000) == 0.00 );
	mu_assert( "Err-01-104", calculate_temperature(0b00000000, 0b01000000) == 0.25 );
	mu_assert( "Err-01-105", calculate_temperature(0b00000000, 0b10000000) == 0.50 );
	mu_assert( "Err-01-106", calculate_temperature(0b00011001, 0b00000000) == 25.00 );
	mu_assert( "Err-01-107", calculate_temperature(0b01111101, 0b00000000) == 125.00 );

	return "01-1PASS";
}

/********************************************************************************************************************//**
 @ingroup ut
 @brief Test of function: SIM908_cmd(const char *cmd, const char *res) in sim908.c
 @return char* - if all tests pass: "02-1PASS" 
			   - if test fail: the error code line where it fails
 @note Test is performed using AT set commands. The SIM908 module must be initialized in order to run this test
 ************************************************************************************************************************/
char* test_SIM908_cmd(void)
{
	mu_assert( "Err-02-100", SIM908_cmd("AT") == SIM908_RESPONSE_OK );
	mu_assert( "Err-02-101", SIM908_cmd("TAT") == SIM908_INVALID_COMMAND );

	return "02-1PASS";
}

/********************************************************************************************************************//**
 @ingroup ut
 @brief Run all unit tests
 @return char* - if all tests pass: "xx-xPASS" 
			   - if test fail: the error code line where it fails			   
************************************************************************************************************************/
char* run_all_tests(void) 
{
	//char* arr[] =
	//{
		//test_tc72_calc(),
		//test_SIM908_cmd()
	//};
	
	mu_run_test(test_tc72_calc);
	mu_run_test(test_SIM908_cmd);
	
	return "All test passed";
}
