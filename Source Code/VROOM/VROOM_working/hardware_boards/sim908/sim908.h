/************************************************//*
 * @file: sim908.h
 *
 * @Created: 25-09-2014 13:09:36
 * @Author: Kenneth René Jensen
 * @Version: 0.5
 * @defgroup sim908 Sim908_GSM
 * @{
	 This is the driver for GSM/GPRS/GPS module sim908
	 @defgroup sim908_priv Private
	 @defgroup sim908_pub Public
 * @}
 */

#ifndef SIM908_GSM_H_
#define SIM908_GSM_H_

#include <avr/io.h>
#include <stdbool.h>

/* Uncomment for Arduino default port settings */
// #define ARDUINO_ATMEGA2560_DEFAULT
#define STK600

#ifdef STK600
 #define DRIVER_PORT			PORTE
 #define GSM_PORT				PORTE
 #define GPS_PORT				PORTE
 #define GSM_ENABLE_PIN			PE3
 #define GPS_ENABLE_PIN			PE4
 #define CE_PIN					PE5
#endif /* STK600 */

#ifdef ARDUINO_ATMEGA2560_DEFAULT
 #define DRIVER_PORT			PORTE
 #define GSM_PORT				PORTE
 #define GPS_PORT				PORTG
 #define GSM_ENABLE_PIN			PE5		/* Pin 3 */
 #define GPS_ENABLE_PIN			PG5		/* Pin 4 */
 #define CE_PIN					PE3		/* Pin 5 */
#endif /* ARDUINO_ATMEGA2560_DEFAULT */

/* *************************************************************************** */

/* Flag list for response */
#define SIM908_FLAG_WAITING			0
#define SIM908_FLAG_OK				1
#define SIM908_FLAG_ERROR			2

#define SIM908_FLAG_FTP_PUT_OPEN	10
#define SIM908_FLAG_FTP_PUT_CLOSE	11
#define SIM908_FLAG_FTP_PUT_SUCCESS 12
#define SIM908_FLAG_FTP_PUT_ERROR	13

#define SIM908_FLAG_GPS_OK			20
#define SIM908_FLAG_GPS_PULL		21

#define SIM908_FLAG_RUNNING			30

/* AT response compare string literals */
#define SIM908_RESPONSE_RDY			"RDY"
#define SIM908_RESPONSE_OK			"OK"
#define SIM908_RESPONSE_ERROR		"ERROR"
#define SIM908_RESPONSE_CR_LF		"\r\n"
#define SIM908_RESPONSE_LF_CR		"\n\r"
#define SIM908_RESPONSE_AT			"AT"
#define SIM908_RESPONSE_GPS_READY	"GPS Ready"
#define SIM908_RESPONSE_GPS_PULL	"0,"
#define SIM908_RESPONSE_FTP_PUT		"+FTPPUT:"
#define SIM908_RESPONSE_CREG		"+CREG: " /* +CREG: 1 = connected */

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Initiates the SIM908 module
 @return void
 @note UART0 is used to communicate with the module.
 ************************************************************************************************************************/
void SIM908_init(void);

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Starts the SIM908 module in following procedure.
		setup global settings -> setup GSM -> setup GPS -> enable GSM communication -> setup FTP
 @return void
 ************************************************************************************************************************/
void SIM908_start(void);

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Used for sending AT SET commands.
 @param *cmd is the AT command as a string
 @return true if successful else false
 ************************************************************************************************************************/
bool SIM908_cmd(const char *cmd, bool __wait_for_ok);

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sets the MSD with relevant information given from GPS response
 @param *__UTC_sec points to the timestamp in MSD structure
		*__latitude points to the latitude in MSD structure
		*__longitude points to the longitude in MSD structure
		*__course points to the direction in MSD structure
		*__IPV4 points to sp in MSD structure
 @return void
 ************************************************************************************************************************/
void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course, uint8_t *__IPV4);

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Calling Public-safety answering point
 @return void
 ************************************************************************************************************************/
void call_PSAP(void);

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sends MSD binary file to FTP server
	*   1:	Create filename:		AT+FTPPUTNAME="<filename>"
	*   2:	Open bearer				AT+SAPBR=1,1
	*   3:	Open FTP PUT session	AT+FTPPUT=1
	*	4:  Set write data			AT+FTPPUT=2,140
	*	5:	Write text (140 bytes)
	*	6:	End write session		AT+FTPPUT=2,0
	*	7:  Close bearer			AT+SAPBR=0,1
 @param const char array with the VROOM ID (defined in vroom_config.h)
 @return void
 ************************************************************************************************************************/
void send_MSD(const char *__vroom_id);

#endif /* SIM908_GSM_H_ */