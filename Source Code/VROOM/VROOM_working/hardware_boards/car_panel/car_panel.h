/********************************************//**
@file car_panel.h
@author: Kenneth Ren� Jensen
@Version: 0.1
@defgroup
@{

@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef CAR_PANEL_H_
#define CAR_PANEL_H_

#include <stdbool.h>

/* Time of butten press before activation/deactivation in 1/10 sec */
#define BUTTON_PRESS_TIME		30

typedef enum
{
	INIT,
	OFFLINE,
	ONLINE
} Status;

typedef enum
{
	ALARM_WAITING,
	ALARM_ACTIVATED,
	ALARM_NOT_ACTIVATED
} Control;

void car_panel_init(void);
bool car_panel_wait_cancel_emmergency(void);
void car_panel_set_status(Status s);
void car_panel_set_control(Control c);

#endif /* CAR_PANEL_H_ */