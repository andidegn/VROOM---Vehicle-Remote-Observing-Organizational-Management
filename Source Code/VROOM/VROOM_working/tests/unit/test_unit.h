/**********************************************************************//**
 * @file unit_test.h
 *
 * @author: Andi Degn
 * @author: Kenneth René Jensen
 * @version: 0.2
 * @defgroup test_unit Unit test
 * @{
	 @ingroup test
	 @brief Unit test platform using MinUnit framework
	 @defgroup test_unit_pub Public
	 @defgroup test_unit_priv Private
 * @}
 *************************************************************************/

#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <stdint.h>

/**********************************************************************//**
 * @ingroup test_unit_pub
 * @brief MinUnit macros for unit testing
 * @defgroup test_unit_macro Unit test macro
 * @{
 *************************************************************************/
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)
/** @} */

/**********************************************************************//**
 * @ingroup test_unit_pub
 * @brief extern variable which counts numbers of unit test runs
 *************************************************************************/
extern uint16_t tests_run;

/**********************************************************************//**
 * @ingroup test_unit_pub
 * @brief Run all unit tests
 * @return char* \n- if all tests pass: "ALL TESTS PASSED"
			     \n- if test fail: the error code line where it fails
 *************************************************************************/
char* run_all_test(void);

#endif /* UNIT_TEST_H_ */