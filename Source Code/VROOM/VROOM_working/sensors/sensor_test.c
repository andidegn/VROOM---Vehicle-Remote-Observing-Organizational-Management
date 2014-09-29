///********************************************//**
//@file sensor_test.c
//@author: Kenneth René Jensen
//@Version: 0.1
//@defgroup test Module_test_sensors
//@{
	//This is a test for for the sensor module consisting of TC72 temperature sensor and accelerometer using SPI.
	//The module test uses the LCD display to visualize the sensor output.
//@}
//@note NOT YET Complies MISRO 2004 standards
//************************************************/
//const char degree = 0b011011111;
//static int x_axis, y_axis, z_axis;
//static float temp;
//static char buf[10];
 //
//void sensors_init()
//{
	//x_axis = 0;
	//y_axis = 0;
	//z_axis = 0;
	//temp = 0.0;
	//scheduler_start();
//}
//
//void sensors_read_data(void)
//{
	//x_axis = (int)(acc_get_x_axis() * 1000);
	//y_axis = (int)(acc_get_y_axis() * 1000);
	//z_axis = (int)(acc_get_z_axis() * 1000);
	//temp = get_temperature();
//}
//
//void sensors_print_data(void)
//{
	//lcd_clrscr();
	//lcd_gotoxy(0, 0);
	//sprintf(buf, "x %05d", x_axis);
	//lcd_puts(buf);
//
	//lcd_gotoxy(8, 0);
	//sprintf(buf, "y %05d", y_axis);
	//lcd_puts(buf);
//
	///* listening for switch press */
	//if (btn_lcd_is_pressed(BTN_PIN1)) {
		//_listening = !_listening;
		//lop_toggle_led(LOP_YELLOW);
		//while (btn_lcd_is_pressed(BTN_PIN1));
	//}
//
	//if (_listening) {
		//lcd_gotoxy(0, 1);
		//lcd_puts(_uart_callback_data);
		//} else {
		//lcd_gotoxy(0, 1);
		//sprintf(buf, "z %05d", z_axis);
		//lcd_puts(buf);
//
		//lcd_gotoxy(8, 1);
		//lcd_puts(dtostrf( temp, 2, 2, buf ));
		//lcd_putc(degree);
		//lcd_putc('C');
	//}
//}