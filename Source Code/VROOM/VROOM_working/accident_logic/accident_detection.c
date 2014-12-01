/**********************************************************************//**
 * @file accident_detection.c
 *
 * @author: Kenneth René Jensen
 * @Version: 0.1
 * @{
		This file holds the logic for detecting an accident
 * @}
 * @note NOT YET Complies MISRO 2004 standards
 *************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include "accident_detection.h"
#include "accident_data.h"
#include "../vroom_config.h"
#include "../util/timer/timer.h"
#include "../hardware_boards/car_panel/car_panel.h"
#include "../sensors/sensor_scheduler.h"

float EXT_TEMPERATURE = 0;
float EXT_TOTAL_ACCELERATION_AVG = 0;

#define TEMPERATURE_INITIAL -100 /* Used to be sure there is two temperature readings before comparing values */

/* Local variables */
static volatile float cur_temp = 0.0F;
static volatile float prev_temp = TEMPERATURE_INITIAL;

void accident_detection_init(void)
{
	/* Interrupts every 100 ms */
	init_Timer3_CTC(TIMER_PS256, CONFIG_ALARM_DETECTION_RESOLUTION);
}

/**********************************************************************//**
 * @ingroup 
 * Gets the accelerometer data and analysis them to see if a crash has
 * occurred
 * Steps in function:\n
 * 1. Read accelerometer data\n
 * 2. Analysis data\n
 * 3. Sets EXT_EMERGENCY_FLAG to EMERGENCY_AUTO_ALARM if crash is detected\n
 **************************************************************************/
void ad_check_for_crash(void) {
	volatile bool _alarm = true;
	EXT_TOTAL_ACCELERATION_AVG = 0;
	int16_t *_acc_buffer = malloc(10 * sizeof(int16_t));

	scheduler_acc_get_last_readings_sum(_acc_buffer);

	for (uint8_t i = 0; i < CONFIG_ALARM_CRASH_NO_OF_READINGS; i++) {
		if (*(_acc_buffer + i) < CONFIG_ALARM_CRASH_TRIGGER_VALUE) {
			_alarm = false;
			break;
		}
		EXT_TOTAL_ACCELERATION_AVG += *(_acc_buffer + i);
	}
	free(_acc_buffer);

	if (_alarm && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM) {
		accident_detection_stop();
		if (!car_panel_wait_cancel_emmergency()) {
			EXT_TOTAL_ACCELERATION_AVG /= (CONFIG_ALARM_CRASH_NO_OF_READINGS*100);
			EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
		}else {
			accident_detection_start();
		}
	}
}

/**********************************************************************//**
 * @ingroup 
 * Gets the temperature and compare to last read value to see if temperature
 * has raised to a critical level
 * Steps in function:\n
 * 1. Read temperature\n
 * 2. Compare current value with last reading\n
 * 3. Sets EXT_EMERGENCY_FLAG to EMERGENCY_AUTO_ALARM if fire is detected\n
 **************************************************************************/
void ad_check_for_fire(void) 
{
	cur_temp = EXT_TEMPERATURE = scheduler_temp_get_last_reading();

	if (prev_temp != TEMPERATURE_INITIAL && (cur_temp - prev_temp) > CONFIG_ALARM_FIRE_TRIGGER_DEGREE && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM)
	{
		accident_detection_stop();
		if (!car_panel_wait_cancel_emmergency())
		{
			EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
		}	
		else
		{
			accident_detection_start();
		}
	}
	
	prev_temp = cur_temp;
}

void accident_detection_start(void)
{
	start_timer3();
}

void accident_detection_stop(void)
{
	stop_timer3();
}