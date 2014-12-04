/**********************************************************************//**
@file unit_test.h
@author: Kenneth Ren� Jensen
@Version: 0.1
@defgroup ut unit_test
@{
	This is the unit test platform for all relevant functions
	(including arithmetic) in the VROOM system.
@}
@note Complies MISRO 2004 standards
 *************************************************************************/

#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <stdint.h>

/**********************************************************************//**
 * @ingroup ut
 * @brief macros for unit testing
 * @code
 *************************************************************************/
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; return message; } while (0)
/** @endcode */

/**********************************************************************//**
 * @ingroup ut
 * @brief extern variable which counts numbers of unit test runs
 * @code
 *************************************************************************/
extern uint16_t tests_run;
/** @endcode */

/**********************************************************************//**
 @ingroup ut
 @brief Run all unit tests
 @return char* - if all tests pass: "xx-xPASS"
			   - if test fail: the error code line where it fails
 *************************************************************************/
char* run_all_test(void);

#endif /* UNIT_TEST_H_ */