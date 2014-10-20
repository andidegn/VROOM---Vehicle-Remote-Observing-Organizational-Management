/********************************************//**
@file accident_data.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Followed eCall standards for data structure.
@}
@note Complies MISRO 2004 standards
************************************************/

#include "accident_data.h"

#define POSITION_TRUSTED		(1<<4)
#define TEST_CALL				(1<<5)
#define MANUAL_ACTIVATION		(1<<6)
#define AUTOMATIC_ACTIVATION	(1<<7)

MSD _msd;

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the control byte \n
 * 	Content of control byte: |---Reserved--- | No confidence in position | Test call | Manual activation | Automatic activation | \n
	Bit:					 | 0 | 1 | 2 | 3 |			  4				 |     5	 |	    6			 |	       7		    | \n
 * @return void
 * @param test_call - Flag for testing purposes
 * @param position_can_be_trusted - confidence in position
 * @param automatic_activation - Flag to show if it is an manual or automatic activated alarm
 *************************************************************************/
void set_control_byte(bool test_call, bool position_can_be_trusted, bool automatic_activation) {
    if (test_call) {
        _msd.control |= TEST_CALL;
    } else {
        _msd.control &= ~TEST_CALL;
    }

    if (position_can_be_trusted) {
        _msd.control |= POSITION_TRUSTED;
    } else {
        _msd.control &= ~POSITION_TRUSTED;
    }

    if (automatic_activation) {
        _msd.control |= AUTOMATIC_ACTIVATION;
        _msd.control &= ~MANUAL_ACTIVATION;
    } else {
        _msd.control |= MANUAL_ACTIVATION;
        _msd.control &= ~AUTOMATIC_ACTIVATION;
    }
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle identification number
 * @return void
 * @param VIN - an array of 4 bytes
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
void set_VIN(char VIN[]) {
    _msd.VIN[0] = VIN[0];
    _msd.VIN[1] = VIN[1];
    _msd.VIN[2] = VIN[2];
    _msd.VIN[3] = VIN[3];
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle location
 * @return void
 * @param latitude - Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000)
 * @param longitude - Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000)
 *************************************************************************/
void set_location(int32_t latitude, int32_t longitude) {
    _msd.latitude = latitude;
    _msd.longitude = longitude;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle direction
 * @return void
 * @param value - The nearest integer of 360.0*value/255.0 (0 ? value ? 255)
 *************************************************************************/
void set_direction(uint8_t value) {
    /* 360.0*degrees/255.0 (0 ? value ? 255) */
    _msd.direction = ((360*value)/255.0);

}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the service provider
 * @return void
 * @param sp - An array of 4 bytes consisting the SP in IPV4 format
 * @note May also be blank field
 *************************************************************************/
void set_service_provider(uint8_t sp[]) {
    _msd.sp[0] = sp[0];
    _msd.sp[1] = sp[1];
    _msd.sp[2] = sp[2];
    _msd.sp[3] = sp[3];
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 * @return void
 * @param s - Maximum 102 bytes string allowed
 * @note May also be blank field
 *************************************************************************/
void set_optional_data(char *s) {
    /* MAX SIZE OF DATA = 102 bytes */
    _msd.optional_data = s;
}

void get_lat_long(char *__raw_at_str, double *__latitude, double *__longitude) {
	char *lat_raw, *long_raw;
	strtok(__raw_at_str, ",");
	lat_raw = strtok(NULL, ",");
	long_raw = strtok(NULL, ",");

	int lat_i = 0;
	int long_i = 0;
	int i;
	for (i = 0; i < 5; i++) {
		if (lat_raw[i] == '.') {
			lat_i = i - 2;
		}
		if (long_raw[i] == '.') {
			long_i = i - 2;
		}
	}

	char lat_deg[3];
	char long_deg[3];

	for (i = 0; i < lat_i; i++) {
		lat_deg[i] = lat_raw[i];
	}
	lat_deg[lat_i] = '\0';
	/* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
	*__latitude = atoi(lat_deg) + atof(&lat_raw[+lat_i]) / 60;

	for (i = 0; i < long_i; i++) {
		long_deg[i] = long_raw[i];
	}
	long_deg[long_i] = '\0';
	/* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
	*__longitude = atoi(long_deg) + atof(&long_raw[long_i]) / 60;
}
