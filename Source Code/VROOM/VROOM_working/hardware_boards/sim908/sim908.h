/**********************************************************************//**
 * @file: sim908.h
 *
 * @created 25-09-2014 13:09:36
 * @author Andi Degn
 * @author Kenneth René Jensen
 * @version 0.6
 * @defgroup sim908 SIM908 GSM/GPRS/GPS
 * @{
	@ingroup hardware_boards
	@brief Driver for GSM/GPRS/GPS module sim908
	@note Complies MISRA 2004 standards
	@defgroup sim908_pub Public
	@defgroup sim908_priv Private
 * @}
 *************************************************************************/

#ifndef SIM908_GSM_H_
#define SIM908_GSM_H_

#include <avr/io.h>
#include <stdbool.h>

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Defines the port(s) and pins for the communition to the SIM908 module
 * @defgroup sim908_port Port
 * @{
 *************************************************************************/
 #define DRIVER_PORT			CONFIG_PORT_DRIVER_PORT
 #define GSM_PORT				CONFIG_PORT_GSM_PORT
 #define GPS_PORT				CONFIG_PORT_GPS_PORT
 #define GSM_ENABLE_PIN			CONFIG_PORT_GSM_ENABLE_PIN
 #define GPS_ENABLE_PIN			CONFIG_PORT_GPS_ENABLE_PIN
 #define CE_PIN					CONFIG_PORT_CE_PIN
/** @} */

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Initiates the SIM908 module
 *
 * @param void
 *
 * @return void
 *
 * @note UART0 is used to communicate with the module.
 *************************************************************************/
void SIM908_init(void);

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Starts the SIM908 module
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void SIM908_start(void);

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Used for sending AT SET commands.
 *
 * @param *__cmd - the AT command as a string
 * @param __wait_for_ok - flag determing the wait for response  
 *
 * @return bool - true if successful else false
 *************************************************************************/
bool SIM908_cmd(const char *__cmd, bool __wait_for_ok);

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Sets the MSD with relevant information given from GPS response
 *
 * @param uint32_t *__UTC_sec - pointer to the timestamp in MSD structure
 * @param uint32_t *__latitude - pointer to the latitude in MSD structure
 * @param uint32_t *__longitude - pointer to the longitude in MSD structure
 * @param uint8_t *__course - pointer to the direction in MSD structure
 *
 * @return void
 *************************************************************************/
void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course);

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Calling Public-safety answering point
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void call_PSAP(void);

/**********************************************************************//**
 * @ingroup sim908_pub
 * @brief Sends MSD binary file to FTP server
 *
 * @param const char *__vroom_id - array with the VROOM ID (defined in vroom_config.h)
 *
 * @return void
 *************************************************************************/
void send_MSD(const char *__vroom_id);

#endif /* SIM908_GSM_H_ */