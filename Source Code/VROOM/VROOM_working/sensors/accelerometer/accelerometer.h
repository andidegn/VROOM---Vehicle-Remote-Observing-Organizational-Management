/**********************************************************************//**
 * @file: accelerometer.h
 *
 * @Created: 01-09-2014 09:22:18
 * @Author: Andi Degn
 * @Version: 0.1
 * @defgroup acc Accelerometer Driver
 * @{
	 This is a driver for the accelerometer LIS331
	 on the ATMEGA family processors.
	 @defgroup acc_priv Private
	 @defgroup acc_pub Public
 * @}
 * @note Complies MISRO 2004 standards
 *************************************************************************/

#ifndef AIRCRAFT_H_
#define AIRCRAFT_H_

/* @define Defines the pin for chip select */
#define ACC_CS_PIN PB0

/* @define Defines all the control registers and the specific pins */
#define ACC_CTRL_REG1	0x20
#define ACC_Xen			0
#define ACC_Yen			1
#define ACC_Zen			2
#define ACC_DR0			3
#define ACC_DR1			4
#define ACC_PM0			5
#define ACC_PM1			6
#define ACC_PM2			7

#define ACC_CTRL_REG2	0x21
#define ACC_HPCF0		0
#define ACC_HPCF1		1
#define ACC_HPen1		2
#define ACC_HPen2		3
#define ACC_FDS			4
#define ACC_HPM0		5
#define ACC_HPM1		6
#define ACC_BOOT		7

#define ACC_CTRL_REG3	0x22
#define ACC_I1_CFG0		0
#define ACC_I1_CFG1		1
#define ACC_LIR1		2
#define ACC_I2_CFG0		3
#define ACC_I2_CFG1		4
#define ACC_LIR2		5
#define ACC_PP_OD		6
#define ACC_IHL			7

#define ACC_CTRL_REG4	0x23
#define ACC_SIM			0
#define ACC_ST			1
#define ACC_STsign		3
#define ACC_FS0			4
#define ACC_FS1			5
#define ACC_BLE			6
#define ACC_BDU			7

#define ACC_CTRL_REG5	0x24
#define ACC_TurnOn0		0
#define ACC_TurnOn1		1

#define ACC_READ_BIT 7
#define ACC_MULTI_BIT 6

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Specifies the available power modes \n
 *							Output data rate: \n
 * * ACC_POWER_DOWN		: -- \n
 * * ACC_NORMAL_MODE	: ODR \n
 * * ACC_LOW_POWER_0_5	: 0.5 Hz \n
 * * ACC_LOW_POWER_1	: 1 Hz \n
 * * ACC_LOW_POWER_2	: 2 Hz \n
 * * ACC_LOW_POWER_5	: 5 Hz \n
 * * ACC_LOW_POWER_10	: 10 Hz \n
 * @{
 *************************************************************************/
typedef enum {
	ACC_POWER_DOWN		= 0,
	ACC_NORMAL_MODE		= _BV(ACC_PM0),
	ACC_LOW_POWER_0_5	= _BV(ACC_PM1),
	ACC_LOW_POWER_1		= _BV(ACC_PM1) | _BV(ACC_PM0),
	ACC_LOW_POWER_2		= _BV(ACC_PM2),
	ACC_LOW_POWER_5		= _BV(ACC_PM2) | _BV(ACC_PM0),
	ACC_LOW_POWER_10	= _BV(ACC_PM2) | _BV(ACC_PM1)
} ACC_POWER_MODE;

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Specifies the available Data Output Rates \n
 *					Output data rate| LP Cut-off \n
 * * ACC_ODR_50		:         50 Hz |     37 Hz \n
 * * ACC_ODR_100	:        100 Hz |     74 Hz \n
 * * ACC_ODR_400	:        400 Hz |    292 Hz \n
 * * ACC_ODR_1000	:       1000 Hz |    780 Hz \n
 * @{
 *************************************************************************/
typedef enum {
	ACC_ODR_50		= 0,
	ACC_ODR_100		= _BV(ACC_DR0),
	ACC_ODR_400		= _BV(ACC_DR1),
	ACC_ODR_1000	= _BV(ACC_DR1) | _BV(ACC_DR0)
} ACC_OUTPUT_DATA_RATE;

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Specifies the available High-pass filter cut-off frequency configuration \n
 * Look in datasheet page 24 for further information.
 * @{
 *************************************************************************/
typedef enum {
	ACC_HPcoeff00	= 0,
	ACC_HPcoeff01	= _BV(ACC_HPCF0),
	ACC_HPcoeff10	= _BV(ACC_HPCF1),
	ACC_HPcoeff11	= _BV(ACC_HPCF1) | _BV(ACC_HPCF0)
} ACC_High_Pass_CUT_OFF;


/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Specifies the available Full-scale selections \n
 * * ACC_6G	 :  +-6g
 * * ACC_12G : +-12g
 * * ACC_24G : +-24g
 * @{
 *************************************************************************/
typedef enum {
	ACC_6G	= 0,
	ACC_12G	= _BV(ACC_FS0),
	ACC_24G = _BV(ACC_FS1) | _BV(ACC_FS0)
} ACC_FULL_SCALE;

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Initializes the driver
 *
 * @return void
 *************************************************************************/
void acc_init(ACC_POWER_MODE power_mode, ACC_OUTPUT_DATA_RATE output_data_rate, ACC_FULL_SCALE full_scale);

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Starts one measure routine
 *
 * @return void
 *************************************************************************/
void acc_measure(void);

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Gets the latest measured x axis value
 *
 * @return float - the acceleration in g [N/Kg] Range: -1 < x < 1
 *************************************************************************/
float acc_get_x_axis(void);

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Gets the latest measured y axis value
 *
 * @return float - the acceleration in g [N/Kg] Range: -1 < x < 1
 *************************************************************************/
float acc_get_y_axis(void);

/**********************************************************************//**
 * @ingroup acc_pub
 * @brief Gets the latest measured z axis value
 *
 * @return float - the acceleration in g [N/Kg] Range: -1 < x < 1
 *************************************************************************/
float acc_get_z_axis(void);

#endif /* AIRCRAFT_H_ */
