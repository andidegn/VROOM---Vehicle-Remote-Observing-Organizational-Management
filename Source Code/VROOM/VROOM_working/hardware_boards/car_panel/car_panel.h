/********************************************//**
@file car_panel.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup 
@{
	
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef CAR_PANEL_H_
#define CAR_PANEL_H_

typedef enum 
{
	INIT, 
	OFFLINE, 
	ONLINE		
} Status;

typedef enum
{
	WAITING,
	ACTIVATED,
	DEACTIVATED
} Control;

void init_car_panel(void);
void set_status(Status s);
void set_control(Control c);

#endif /* CAR_PANEL_H_ */