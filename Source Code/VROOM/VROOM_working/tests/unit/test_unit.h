/**********************************************************************//**
 * @file unit_test.h
 * @author: Andi Degn
 * @author: Kenneth René Jensen
 * @Version: 0.2
 * @defgroup 
 * @{
		@brief Unit test platform using MinUnit framework
 * @}
 *************************************************************************/

#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <stdint.h>

/**********************************************************************//**
 * @ingroup ut
 * @brief MinUnit macros for unit testing
 * @code
 *************************************************************************/
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)
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
 @return char* - if all tests pass: "ALL TESTS PASSED"
			   - if test fail: the error code line where it fails
 *************************************************************************/
char* run_all_test(void);

#endif /* UNIT_TEST_H_ */