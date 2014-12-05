/**********************************************************************//**
 * @file accident_detection.c
 *************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include "accident_detection.h"
#include "accident_data.h"
#include "../hardware_boards/car_panel/car_panel.h"
#include "../application/scheduler/scheduler.h"
#include "../util/r2r_led/r2r_led.h"

/* Local variables */
static volatile float cur_temp = 0.0F;
static volatile float prev_temp = CONFIG_ALARM_FIRE_TEMP_INIT;

/**********************************************************************//**
 * @ingroup ac_det_pub
 * Gets the accelerometer data and analysis them to see if a crash has
 * occurred
 * Steps in function:
 * 1. Read accelerometer data
 * 2. Analysis data
 * 3. Sets EXT_EMERGENCY_FLAG to EMERGENCY_AUTO_ALARM if crash is detected
 **************************************************************************/
void check_for_crash(void) {
	volatile bool _alarm = true;
	uint8_t i = 0;
	int16_t *_acc_buffer = malloc(CONFIG_ALARM_CRASH_NO_OF_READINGS * sizeof(int16_t));

	scheduler_acc_get_last_readings_sum(_acc_buffer);

	for (i = 0; i < CONFIG_ALARM_CRASH_NO_OF_READINGS; i++) {
		if (*(_acc_buffer + i) < CONFIG_ALARM_CRASH_TRIGGER_VALUE) {
			_alarm = false;
			break;
		}
	}

	free(_acc_buffer);

	if (_alarm && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM) {
		scheduler_halt();
		if (!car_panel_wait_cancel_emmergency()) {
			EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
		}else {
			scheduler_resume(true);
		}
	}
}

/**********************************************************************//**
 * @ingroup ac_det_pub
 * Gets the temperature and compare to last read value to see if temperature
 * has raised to a critical level
 * Steps in function:
 * 1. Read temperature
 * 2. Compare current value with last reading
 * 3. Sets EXT_EMERGENCY_FLAG to EMERGENCY_AUTO_ALARM if fire is detected
 **************************************************************************/
void check_for_fire(void)
{
	cur_temp = scheduler_temp_get_last_reading();

	if (prev_temp != CONFIG_ALARM_FIRE_TEMP_INIT && (cur_temp - prev_temp) > CONFIG_ALARM_FIRE_TRIGGER_DEGREE && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM)
	{
		scheduler_halt();
		if (!car_panel_wait_cancel_emmergency())
		{
			EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
		}
		else
		{
			scheduler_resume(true);
		}
	}

	prev_temp = cur_temp;
}
