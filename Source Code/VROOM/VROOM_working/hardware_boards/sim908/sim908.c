/**********************************************************************//**
 * @file sim908.c
 *************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "sim908.h"
#include "at_commands.h"
#include "../../application/vroom_config.h"
#include "../../application/scheduler/scheduler.h"
#include "../../data_comm/uart/uart.h"
#include "../../accident_logic/accident_data.h"
#include "../../util/time/time.h"
#include "../../util/r2r_led/r2r_led.h"

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Defines for the flag list responses
 * @defgroup sim908_flag Flag codes
 * @{
 *************************************************************************/
#define SIM908_FLAG_WAITING			0U
#define SIM908_FLAG_OK				1U
#define SIM908_FLAG_ERROR			2U

#define SIM908_FLAG_FTP_PUT_OPEN	10U
#define SIM908_FLAG_FTP_PUT_CLOSE	11U
#define SIM908_FLAG_FTP_PUT_SUCCESS 12U
#define SIM908_FLAG_FTP_PUT_ERROR	13U
#define SIM908_FLAG_FTP_SENDING		14U

#define SIM908_FLAG_GPS_OK			20U
#define SIM908_FLAG_GPS_PULL		21U
#define SIM908_FLAG_GPS_PULL_OK		22U

#define SIM908_FLAG_RUNNING			100U
/** @} */

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief defines the AT response string literals
 * @defgroup sim908_response Response literals
 * @{
 *************************************************************************/
#define SIM908_RESPONSE_RDY			"RDY"

#define SIM908_RESPONSE_SKIP		"AT+"
#define SIM908_RESPONSE_OK			"OK"
#define SIM908_RESPONSE_ERROR		"ERR"
#define SIM908_RESPONSE_CR_LF		"\r\n"
#define SIM908_RESPONSE_LF_CR		"\n\r"
#define SIM908_RESPONSE_AT			"AT"
#define SIM908_RESPONSE_GPS_READY	"GPS"
#define SIM908_RESPONSE_GPS_PULL	"0,"
#define SIM908_RESPONSE_FTP_PUT		"+F"
#define SIM908_RESPONSE_CREG		"+CREG: "		/* +CREG: 1 = connected */
/** @} */

#define RX_BUFFER_SIZE 128U

/* Local variables */
static volatile uint8_t _CR_counter = 0U;
static volatile uint8_t _LF_counter = 0U;

static volatile char _rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t _rx_buffer_tail = 0U;
static volatile uint8_t _rx_response_length = 0U;

static volatile uint8_t _system_running_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _wait_for_ok_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _gps_pull_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _ftp_sending_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _ack_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_ftp_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_gps_response_flag = SIM908_FLAG_WAITING;

static uint8_t _gps_response_tail = 0U;
static uint8_t _gps_response_length = 0U;

#ifdef DEBUG_TASK_MEASURE
static uint8_t _task_prev_id_start_module = 0U;
static uint8_t _task_prev_id_cmd_send = 0U;
static uint8_t _task_prev_id_wait_for_response = 0U;
static uint8_t _task_prev_id_callback = 0U;
#endif

/* External from accident_data */
char EXT_MSD_FILENAME[24];

#define CR	0x0DU /* '\r' */
#define LF	0x0AU/* '\n' */

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define RETRY_ATTEMPTS 20
#define CONNECTION_RETRY_DELAY_IN_MS 500U

/* Prototypes */
static void _setup_GSM(void);
static void _setup_GPS(void);
static void _setup_GPRS_FTP(void);
static void _GSM_enable(void);
static void _GPS_enable(void);
static void _wait_for_connection(void);
static bool _wait_response(volatile uint8_t *__flag, uint8_t __ok_def);
static bool _check_response(const char *__defined_response);
static void _get_GPS_response(void);
static char _char_at(uint8_t __index, uint8_t __tail, uint8_t __length);
static void _raw_to_array(char **__output);
static uint32_t _set_UTC_sec(const char *__utc_raw);
static int32_t _get_lat_long(const char *__lat_long_raw);
static uint8_t _set_direction(const char *__direction_raw);
static void _set_MSD_filename(const char *__UTC_raw);
void _SIM908_callback(char data);

#ifdef  DEBUG_UART_ENABLE
void _PC_callback(char data);
#endif

/**********************************************************************//**
 * @ingroup sim908_pub
 * Sets up ports and uart communication
 *************************************************************************/
void SIM908_init(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Set all related pins to output */
	DDR(DRIVER_PORT) |= (1<<CE_PIN);
	DDR(GSM_PORT) |= (1<<GSM_ENABLE_PIN);
	DDR(GPS_PORT) |= (1<<GPS_ENABLE_PIN);

	/* Toggle driver pin to start up SIM908 module */
	DRIVER_PORT |= _BV(CE_PIN);
	_delay_ms(1000);
	DRIVER_PORT &= ~_BV(CE_PIN);
	_GSM_enable();

	/* Restore interrupt */
	SREG = SREG_cpy;

	/* Setting up UART for internal communication */
 	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _SIM908_callback);

	#ifdef DEBUG_UART_ENABLE
	/* Setting up UART for external communication */
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _PC_callback);
	#endif
}

/**********************************************************************//**
 * @ingroup sim908_pub
 * Sends initialization commands to the module.
 *************************************************************************/
void SIM908_start(void)
{
#ifdef DEBUG_TASK_MEASURE
	_task_prev_id_start_module = r2r_start_task(DEBUG_ID_SIM908_START_MODULE);
#endif
	while (_system_running_flag == SIM908_FLAG_WAITING){}

	/* Set baud rate to the host baud rate */
	while(!SIM908_cmd(AT_BAUD_115K2, true));

	/* Synchronizing baud rate */
	while(!SIM908_cmd(AT_DIAG_TEST, true));

	/* Disable Echo */
	while(!SIM908_cmd(AT_DIAG_ECHO_DISABLE, true));

	/* Enable CREG unsolicited result code */
	while(!SIM908_cmd(AT_ENABLE_CREG, true));
	
	/* Enable CREG unsolicited result code */
	while(!SIM908_cmd(AT_ENABLE_CREG, true));
	
	/* Disable report mobile equipment error */
	while(!SIM908_cmd(AT_CMEE_DISABLE, true));

	#ifdef CONFIG_PIN
		/* wait for +CPIN: SIM PIN - is going to be deleted */
		_delay_ms(1000);
		while(!SIM908_cmd(AT_ENTER_SIM_PIN(CONFIG_PIN), true));
	#endif

	_setup_GSM();
	_setup_GPRS_FTP();	
	_setup_GPS();
#ifdef DEBUG_TASK_MEASURE
	r2r_stop_task(_task_prev_id_start_module);
#endif
}

/**********************************************************************//**
 * @ingroup sim908_pub
 * Sends a command via uart. If '__wait_for_ok' it waits and returns a
 * bool. True if successful, false if not
 *************************************************************************/
bool SIM908_cmd(const char *__cmd, bool __wait_for_ok)
{
#ifdef DEBUG_TASK_MEASURE
	_task_prev_id_cmd_send = r2r_start_task(DEBUG_ID_SIM908_CMD_SEND);
#endif
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();
	/* Implement number of retries functionality / parameter*/
	_ack_response_flag = _ack_ftp_response_flag = _ack_gps_response_flag = SIM908_FLAG_WAITING;
	_rx_response_length = _CR_counter = _LF_counter = 0U;

	uart0_send_string(__cmd);
	uart0_send_char(CR);
	uart0_send_char(LF);
	#ifdef DEBUG_UART_ECHO
		uart1_send_string(__cmd);
		uart1_send_char(CR);
		uart1_send_char(LF);
	#endif
	/* Restore interrupt */
	SREG = SREG_cpy;
#ifdef DEBUG_TASK_MEASURE
	r2r_stop_task(_task_prev_id_cmd_send);
#endif

	return __wait_for_ok ? _wait_response(&_ack_response_flag, SIM908_FLAG_OK) : true;
}

/**********************************************************************//**
 * @ingroup sim908_pub
 * Sets the parameters of the MSD data set
 *************************************************************************/
void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course)
{
	char **output;
	output = (char**)malloc(9U * sizeof(char*));
	for (uint8_t i = 0U; i < 9U; i++) {
		*(output + i) = malloc(18U * sizeof(char));
	}

	/* GPS raw data: <mode>,<longitude>,<latitude>,<altitude>,<UTC time>,<TTFF>,<num>,<speed>,<course> */
	_get_GPS_response();
	_raw_to_array(output);

	*__longitude = _get_lat_long(*(output + 1));
	*__latitude = _get_lat_long(*(output + 2));
	*__UTC_sec = _set_UTC_sec(*(output + 4));
	*__course = _set_direction(*(output + 8));

	_set_MSD_filename(*(output + 4));

	for (uint8_t i = 0U; i < 9U; i++) {
		free(*(output + i));
	}
	free(output);
}

/**********************************************************************//**
 * @ingroup sim908_pub
 * Enables incomming calls and calles emergency number
 *************************************************************************/
void call_PSAP(void)
{
	_wait_for_connection();

	/* Enable incoming calls */
	while(!SIM908_cmd(AT_ENABLE_INCOMING_CALLS, true));

	while(!SIM908_cmd(AT_CALL_DIAL(CONFIG_EMERGENCY_PHONE_NUMBER), true));
}

/**********************************************************************//**
 * @ingroup sim908_pub
 * Procedure:
 * 1. Create filename:		(AT+FTPPUTNAME="<filename>")
 * 2. Open bearer:			(AT+SAPBR=1,1)
 * 3. Open FTP PUT session:	(AT+FTPPUT=1)
 * 4. Set write data:		(AT+FTPPUT=2,140)
 * 5. Write text:			(140 bytes)
 * 6. End write session:	(AT+FTPPUT=2,0)
 * 7. Close bearer:			(AT+SAPBR=0,1)
 *************************************************************************/
void send_MSD(const char *__vroom_id)
{
    _wait_for_connection();

    int8_t _retry_ctr = RETRY_ATTEMPTS;
    char *filename = malloc(60U * sizeof(char));
    /* Format: 2014-10-12_13.17.34-(60192949).vroom */
    strcpy(filename, AT_FTP_PUT_FILE_NAME);
    strcat(filename, "\"");
    strcat(filename, EXT_MSD_FILENAME);
    strcat(filename, "-(");
    strcat(filename, __vroom_id);
    strcat(filename, ").vroom\"");

    while(!SIM908_cmd(filename, true));
    free(filename);

	_ftp_sending_flag = SIM908_FLAG_FTP_SENDING;

    while ((!SIM908_cmd(AT_FTP_OPEN_BEARER1, true)) && (_retry_ctr-- > 0)) {
        _delay_ms(1000);
    }

    _retry_ctr = RETRY_ATTEMPTS;
    do {
	    SIM908_cmd(AT_FTP_PUT_OPEN_SESSION, false);
    } while ((!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_OPEN)) && (_retry_ctr-- > 0));

    _retry_ctr = RETRY_ATTEMPTS;
    do {
	    SIM908_cmd(AT_FTP_PUT_FILE_SIZE(CONFIG_FTP_FILE_SIZE), false);
    } while ((!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_SUCCESS)) && (_retry_ctr-- > 0));

    _retry_ctr = RETRY_ATTEMPTS;
    do {
	    EXT_MSD.msg_identifier = RETRY_ATTEMPTS - _retry_ctr + 1U;
	    uart0_send_data((char*)(&EXT_MSD.version), 1U);
	    uart0_send_data((char*)(&EXT_MSD.msg_identifier), 1U);
	    uart0_send_data((char*)(&EXT_MSD.control), 1U);
	    uart0_send_data((char*)(&EXT_MSD.vehicle_class), 1U);
	    uart0_send_data(&EXT_MSD.VIN[0], 20U);
	    uart0_send_data((char*)(&EXT_MSD.fuel_type), 1U);
	    uart0_send_data((char*)(&EXT_MSD.time_stamp), 4U);
	    uart0_send_data((char*)(&EXT_MSD.latitude), 4U);
	    uart0_send_data((char*)(&EXT_MSD.longitude), 4U);
	    uart0_send_data((char*)(&EXT_MSD.direction), 1U);
	    uart0_send_data(&EXT_MSD.optional_data[0], 102U);

	    uart0_send_char(CR);
	    uart0_send_char(LF);
    } while ((!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_OPEN)) && (_retry_ctr-- > 0));

    _retry_ctr = RETRY_ATTEMPTS;
	/* There needs to be a small delay in order to let the connection prepare for closure */
    _delay_ms(100);
    do {
	    SIM908_cmd(AT_FTP_PUT_CLOSE_SESSION, true);
    } while ((!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_CLOSE)) && (_retry_ctr-- > 0));

    while(!SIM908_cmd(AT_FTP_CLOSE_BEARER1, true));

	_ftp_sending_flag = SIM908_FLAG_WAITING;
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief GSM settings on SIM908 module
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _setup_GSM(void)
{
	/* Enable automatic answer */
	while(!SIM908_cmd(AT_DIAG_AUTO_ANSWER("1"), true));

	/* Setup phone functionality */
	while(!SIM908_cmd(AT_FULL_FUNCTIONALITY, true));

	/* Forbid incoming calls */
	while(!SIM908_cmd(AT_FORBID_INCOMING_CALLS, true));
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief GPS settings on SIM908 module
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _setup_GPS(void)
{
	while(_ack_gps_response_flag == SIM908_FLAG_WAITING){}
	/* Enable GPS */
	while(!SIM908_cmd(AT_GPS_POWER_ON, true));

	/* Set GPS reset to autonomous */
	while(!SIM908_cmd(AT_GPS_RST_AUTONOMY, true));
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Settings up GPRS - FTP on SIM908 module
 * 1. Set bearer parameter:
 *	  + AT+SAPBR=3,1,"Contype","GPRS"
 *	  + AT+SAPBR=3,1,"APN","<APN>"
 * 2. Use bearer profile:
 *	  + AT+FTPCID=1
 * 3. FTP login:
 *	  + AT+FTPSERV="<Server add>"
 *	  + AT+FTPPORT=<Server port>
 *	  + AT+FTPUN="<Username>"
 *	  + AT+FTPPW=<Password>
 * 4. Configure put
 *	  + AT+FTPPUTPATH="<Path>"
 *	  + AT+FTPTYPE="I" - (binary)
 *	  + AT+FTPPUTOPT="STOR"
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _setup_GPRS_FTP(void)
{
	uint8_t _ctr_max = 10;
	uint8_t _ctr = 0;
	/* Set bearer parameters */
	while(!SIM908_cmd(AT_FTP_BEARER1_CONTYPE(AT_CONTYPE_GPRS), true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_BEARER1_APN(CONFIG_APN), true) && _ctr++ < _ctr_max);

	/* Use bearer profile 1 */
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_USE_PROFILE1, true) && _ctr++ < _ctr_max);

	/* FTP login */
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_SET_SERVER_ADDRESS(CONFIG_FTP_SERVER_ADDRESS), true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_SET_CONTROL_PORT(CONFIG_FTP_PORT), true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_SET_USER_NAME(CONFIG_FTP_USER), true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_SET_PASSWORD(CONFIG_FTP_PW), true) && _ctr++ < _ctr_max);

	/* Set put information */
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_SET_DATA_TYPE_BINARY, true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_PUT_FILE_STORING, true) && _ctr++ < _ctr_max);
	_ctr = 0;
	while(!SIM908_cmd(AT_FTP_PUT_FILE_PATH(CONFIG_FTP_FILE_PATH), true) && _ctr++ < _ctr_max);
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Enable communication with GSM module
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _GSM_enable(void)
{
	GPS_PORT |= (1<<GPS_ENABLE_PIN);
	GSM_PORT &= ~(1<<GSM_ENABLE_PIN);
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Enable communication with GPS module
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _GPS_enable(void)
{
	GPS_PORT &= ~(1<<GPS_ENABLE_PIN);
	GSM_PORT |= (1<<GSM_ENABLE_PIN);
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Waiting for network connection
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static void _wait_for_connection(void) {
	while ((EXT_CONNECTION_CREG_FLAG != CREG_REGISTERED_HOME_NETWORK) && (EXT_CONNECTION_CREG_FLAG != CREG_REGISTERED_ROAMING)) {
		while(!SIM908_cmd(AT_CONN_NETWORK_REGISTRATION_STATUS, true));
		_delay_ms(CONNECTION_RETRY_DELAY_IN_MS);
	}
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Waiting for response
 *
 * @param volatile uint8_t *__flag - the flag to check on
 * @param uint8_t __ok_def - the definition of an 'OK' response
 *
 * @return bool - true if 'OK' else false
 *************************************************************************/
static bool _wait_response(volatile uint8_t *__flag, uint8_t __ok_def) {
#ifdef DEBUG_TASK_MEASURE
	_task_prev_id_wait_for_response = r2r_start_task(DEBUG_ID_SIM908_WAIT_FOR_RESPONSE);
#endif
	scheduler_pause();
	volatile bool _ret = false;
	while(*__flag == SIM908_FLAG_WAITING) {
		_delay_ms(10);
	}
	if (*__flag == __ok_def) {
		_ret = true;
	}
	scheduler_resume(false);
#ifdef DEBUG_TASK_MEASURE
	r2r_stop_task(_task_prev_id_wait_for_response);
#endif

	return _ret;
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Checking if the last response is the same as the defined response
 *
 * @param const char *__defined_response - the string literal to check on
 *
 * @return bool - true if response is considered the same else false
 *************************************************************************/
static bool _check_response(const char *__defined_response) {
	char c;
	bool ret = true;
	uint8_t i;

	for(i = 0U; (i < strlen(__defined_response)) && (ret != false); i++) {
		c = _char_at(i, _rx_buffer_tail, _rx_response_length);
		ret = (c == __defined_response[i]) ? true : false;
	}
	return ret;
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Returns the character at a specific location in the rx buffer
 *
 * @param uint8_t __index - index of the char to return
 * @param uint8_t __tail - index of the last char of the response
 * @param uint8_t __length - the length of the response
 *
 * @return char - the character at the desired location
 *************************************************************************/
static char _char_at(uint8_t __index, uint8_t __tail, uint8_t __length) {
	volatile uint8_t i = (RX_BUFFER_SIZE + __tail - __length + __index + 1U) % RX_BUFFER_SIZE;
	return _rx_buffer[i];
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Sends the get gps info command and waits for the response
 *
 * @param void
 *
 * @return void
 *
 * @note response format: <mode, longitude, latitude, altitude, UTC time, TTFF, Satellite in view, speed over ground, course over ground>
 *************************************************************************/
static void _get_GPS_response(void)
{
	do {
		_gps_pull_flag = SIM908_FLAG_GPS_PULL;
		SIM908_cmd(AT_GPS_GET_LOCATION, false);
	} while (!_wait_response(&_ack_gps_response_flag, SIM908_FLAG_GPS_PULL_OK));
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Set the timestamp in MSD structure
 *
 * @param const char *__utc_raw - pointer to array of the timestamp string from GPS response
 *
 * @return uint32_t - 4 bytes unsigned in UTC seconds (time in seconds since 1970)
 *************************************************************************/
static uint32_t _set_UTC_sec(const char *__utc_raw)
{
	char year[5] = {__utc_raw[0],  __utc_raw[1], __utc_raw[2], __utc_raw[3], '\0'};
	char month[3] = {__utc_raw[4],  __utc_raw[5], '\0'};
	char day[3] = {__utc_raw[6],  __utc_raw[7], '\0'};
	char hour[3] = {__utc_raw[8],  __utc_raw[9], '\0'};
	char minute[3] = {__utc_raw[10],  __utc_raw[11], '\0'};
	char second[3] = {__utc_raw[12],  __utc_raw[13], '\0'};

	FIXED_TIME t;
	t.year = (uint16_t)atoi(year);
	t.mon = (uint8_t)atoi(month);
	t.day = (uint8_t)atoi(day);
	t.hour = (uint8_t)atoi(hour);
	t.min = (uint8_t)atoi(minute);
	t.sec = (uint8_t)atoi(second);

	return calc_UTC_seconds(&t);
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Gets the latitude or longitude in an 32bit integer from a string
 *
 * @param const char *__lat_long_raw - pointer to string containing the coordinate
 *
 * @return int32_t - coordinate in a milliarcsecond format
 *************************************************************************/
static int32_t _get_lat_long(const char *__lat_long_raw) {
    int8_t lat_long_i = 0;
    int8_t i;
    char lat_long_deg[3];

    for (i = 0; i < 6; i++) {
        if (__lat_long_raw[i] == '.') {
            lat_long_i = i - 2;
        }
    }

    for (i = 0; i < lat_long_i; i++) {
        lat_long_deg[i] = __lat_long_raw[i];
    }
    lat_long_deg[lat_long_i] = '\0';

    /* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
    double __decimal_degree = ((double)(atoi(lat_long_deg)) + atof(&__lat_long_raw[lat_long_i]) / 60.0);

    /* From decimal degrees to milliarcseconds */
    return (int32_t)((double)(__decimal_degree * 3600000.0 + 0.5));
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Set the direction in MSD structure
 *
 * @param const char *__direction_raw - pointer to array of the course over ground string from GPS response
 *
 * @return uint8_t - direction in degrees
 *************************************************************************/
static uint8_t _set_direction(const char *__direction_raw)
{
	/* (0 <= __direction_raw >= 255) */
	return (uint8_t)((double)(255.0*atof(__direction_raw)/360.0 + 0.5));
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Cuts up the gps response and puts it into memory with a pointer to each value
 *
 * @param char **__output - pointer to reserved memory where the data is going to be stored
 *
 * @return void
 *************************************************************************/
static void _raw_to_array(char **__output) {
	uint8_t i = 0U, j = 0U, ij = 0U;
	for (i = 0U; i < _gps_response_length; i++) {
		char c = _char_at(i, _gps_response_tail, _gps_response_length);
		if (c == ',') {
			j++;
			ij = 0U;
			} else {
			*(*(__output + j) + ij++) = c;
		}
	}
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Set the filename of the MSD
 *
 * @param const char *__UTC_raw - pointer to array of the timestamp string from GPS response
 *
 * @return void
 * @note Format: 2014-10-12_13.17.34
 *************************************************************************/
static void _set_MSD_filename(const char *__UTC_raw)
{
	uint8_t i = 0U;

	for (i = 0U; i < 19U; i++)
	{
		if ((i == 4U) || (i == 7U))
		{
			EXT_MSD_FILENAME[i] = '-';
		}
		else if (i == 10U)
		{
			EXT_MSD_FILENAME[i] = '_';
		}
		else if ((i == 13U) || (i == 16U))
		{
			EXT_MSD_FILENAME[i] = '.';
		}
		else
		{
			EXT_MSD_FILENAME[i] = *__UTC_raw++;
		}
	}
	EXT_MSD_FILENAME[i] = '\0';
}

/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Callback function to handle all communication from the sim908 module
 * Procedure:
 * 1. Store in circular buffer
 * 2. Check for carriage return/line feed characters
 * 3. If there has been one carriage return and one line feed, check for response
 * 4. If UART_DEBUG is defined, echo data char to uart1
 *
 * @param char data - single char received via uart from the sim908 module
 *
 * @return void
 *************************************************************************/
void _SIM908_callback(char data) {
	scheduler_pause();
#ifdef DEBUG_TASK_MEASURE
	_task_prev_id_callback = r2r_start_task(DEBUG_ID_SIM908_CALLBACK);
#endif
	#ifdef DEBUG_UART_ECHO
		uart1_send_char(data);																	/* Mirroring communication from sim908 to uart1 */
	#endif
	_rx_response_length++;
	_rx_buffer[_rx_buffer_tail = (_rx_buffer_tail + 1U) % RX_BUFFER_SIZE] = data;				/* Stores received data in buffer. This technically starts from index '1', but as the buffer is circular, it does not matter */

	if (data == CR) {																			/* Checking and counting for CR and LF */
		_CR_counter++;
	} else if (data == LF) {
		_LF_counter++;
	}

	if ((_CR_counter > 0U) && (_LF_counter > 0U)) {
		_CR_counter = _LF_counter = 0U;
		if (_rx_response_length > 2U) {				/* Skipping empty lines */
			if ((_ftp_sending_flag == SIM908_FLAG_FTP_SENDING) &&
				(_check_response(SIM908_RESPONSE_FTP_PUT) == true)) {				/* FTPPUT */
					/* 	FTP PUT OPEN SESSION:	"+FTPPUT:1,1,1260"
						FTP PUT RESPONSE:		"+FTPPUT:2,140"
						FTP PUT CLOSE SESSION:	"+FTPPUT:1,0"		*/
					char c1 = _char_at(8U, _rx_buffer_tail, _rx_response_length);
					char c2 = _char_at(10U, _rx_buffer_tail, _rx_response_length);
					char c3 = _char_at(11U, _rx_buffer_tail, _rx_response_length);
					if ((c1 == '1') && (c2 == '1') && (c3 == ',')) {
						_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_OPEN;
						#ifdef DEBUG_SIM908_CALLBACK
						uart1_send_string("rx:> ftp put open\r\n");
						#endif
					} else if((c1 == '2') && (c2 == '1') && (c3 == '4')) {
						_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_SUCCESS;
						#ifdef DEBUG_SIM908_CALLBACK
						uart1_send_string("rx:> ftp put success\r\n");
						#endif
					} else if((c1 == '1') && (c2 == '0')) {
						_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_CLOSE;
						#ifdef DEBUG_SIM908_CALLBACK
						uart1_send_string("rx:> ftp put close\r\n");
						#endif
					} else {
						_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_ERROR;
						#ifdef DEBUG_SIM908_CALLBACK
						uart1_send_string("rx:> ftp put error\r\n");
						#endif
					}
			} else
			if ((_gps_pull_flag == SIM908_FLAG_GPS_PULL) &&
				(_check_response(SIM908_RESPONSE_GPS_PULL))) {				/* GPS pull */
					_gps_response_tail = _rx_buffer_tail;
					_gps_response_length = _rx_response_length;
					_ack_gps_response_flag = SIM908_FLAG_GPS_PULL_OK;
					_gps_pull_flag = SIM908_FLAG_WAITING;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> gps pull\r\n");
					#endif
			} else
			if ((_rx_response_length == 4U) &&
				(_check_response(SIM908_RESPONSE_OK) == true)) {			/* OK */
					_ack_response_flag = SIM908_FLAG_OK;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> ok\r\n");
					#endif
			} else
			if ((_rx_response_length == 7U) &&
				(_check_response(SIM908_RESPONSE_ERROR) == true)) {			/* Error */
					_ack_response_flag = SIM908_FLAG_ERROR;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> error\r\n");
					#endif
			} else
			if (((_rx_response_length == 10U) ||
				(_rx_response_length == 12U)) &&
				(_check_response(SIM908_RESPONSE_CREG) == true)) {			/* CREG */
					EXT_CONNECTION_CREG_FLAG = _char_at(_rx_response_length - 3U, _rx_buffer_tail, _rx_response_length) - '0';	/* Subtracting '0' (0x30) to get the value as an integer */
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> creg\r\n");
					#endif
			} else
			if ((_rx_response_length == 11U) &&
				(_check_response(SIM908_RESPONSE_GPS_READY) == true)) {		/* GPS Ready */
					_ack_gps_response_flag = SIM908_FLAG_GPS_OK;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> gps ready\r\n");
					#endif
			} else
			if ((_system_running_flag == SIM908_FLAG_WAITING) &&
				(_rx_response_length == 5U) &&
				(_check_response(SIM908_RESPONSE_RDY) == true)) {			/* System ready */
					_system_running_flag = SIM908_FLAG_RUNNING;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> rdy\r\n");
					#endif
			}
			else if ((_rx_response_length == 4U) &&
				(_check_response(SIM908_RESPONSE_AT) == true)) {			/* Sync AT cmd */
					_rx_response_length = 0U;
					#ifdef DEBUG_SIM908_CALLBACK
					uart1_send_string("rx:> at\r\n");
					#endif
			} else {
				#ifdef DEBUG_SIM908_CALLBACK_NOT_CAUGHT
				uart1_send_string("rx:> NOT CAUGHT: >>");
				for (uint8_t i = 0U; i < _rx_response_length; i++) {
					uart1_send_char(_char_at(i, _rx_buffer_tail, _rx_response_length));
				}
				#endif
			}
		}
		_rx_response_length = 0U;
	}
	scheduler_resume(false);
#ifdef DEBUG_TASK_MEASURE
	r2r_stop_task(_task_prev_id_callback);
#endif
}

#ifdef DEBUG_UART_ENABLE
/**********************************************************************//**
 * @ingroup sim908_priv
 * @brief Callback function to handle debug comminication with pc on uart1
 * When receiving data it is echoed to both uart0 (sim908) and back to pc
 * on uart1
 *
 * @param char data - single char received via uart from pc
 *
 * @return void
 *************************************************************************/
void _PC_callback(char data)
{
	uart0_send_char(data);
	uart1_send_char(data);
}
#endif
