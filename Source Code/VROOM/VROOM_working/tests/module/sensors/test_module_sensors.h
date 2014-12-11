/**********************************************************************//**
 * @file test_module_sensors.h
 * @author: Kenneth René Jensen
 * @Version: 0.1
 * @defgroup test_module_sensors Module Test - Sensors
 * @{
	 @ingroup test
	 @brief This is a test for for the sensor module consisting of TC72
	 temperature sensor and accelerometer using SPI.
	 The module test uses the LCD display to visualize the sensors output.
	 @defgroup test_sensors_pub Public
 * @}
 *************************************************************************/

#ifndef TEST_MODULE_SENSORS_H_
#define TEST_MODULE_SENSORS_H_

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * @brief Initiates the sensor test
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void sensors_init(void);

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * @brief Reads the data from the sensors
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void sensors_read_data(void);

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * @brief Prints the data from the sensors
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void sensors_print_data(void);

#endif /* TEST_MODULE_SENSORS_H_ */