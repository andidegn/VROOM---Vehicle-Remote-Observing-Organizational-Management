/*
 * collect_sensor_data.h
 *
 * Created: 03-09-2014 13:55:08
 *  Author: Kenneth René Jensen
 */

#ifndef COLLECT_SENSOR_DATA_H_
#define COLLECT_SENSOR_DATA_H_

#include "tc72/tc72.h"
#include "accelerometer/accelerometer.h"
#include "../data_comm/uart/uart.h"
#include "../timer.h"

#define SCHEDULER_DELAY 0

void scheduler_start(void (*callback_function_ptr)(char cfp));
void scheduler_release(void);

#endif /* COLLECT_SENSOR_DATA_H_ */