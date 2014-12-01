/**********************************************************************//**
 * collect_sensor_data.h
 *
 * Created: 03-09-2014 13:55:08
 *  Author: Kenneth René Jensen
  *************************************************************************/

#ifndef COLLECT_SENSOR_DATA_H_
#define COLLECT_SENSOR_DATA_H_

#include "thermometer/tc72.h"
#include "accelerometer/lis331hh.h"
#include "../util/timer/timer.h"

#define SCHEDULER_DELAY		0
#define ACC_BUFFER_SIZE		25

void scheduler_start(void (*callback_function_ptr)(char __data));
void scheduler_acc_get_last_readings_sum(int16_t *buffer);
void scheduler_acc_get_last_readings(int16_t *buffer);
float scheduler_temp_get_last_reading(void); 
void scheduler_release(void);

#endif /* COLLECT_SENSOR_DATA_H_ */