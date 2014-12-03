/**********************************************************************//**
 * @file btn_led_lcd.h
 *
 * @created 14-09-2014 22:43:41
 * @author Andi Degn
 * @ingroup hardware_boards
 *************************************************************************/


#ifndef BTN_LED_LCD_H_
#define BTN_LED_LCD_H_

#include <avr/io.h>
#include <stdbool.h>

#define BTN_LED_PORT PORTK

#ifndef BTN_LED_PORT
	#error BTN_LED_PORT must be defined!
#endif

#define BTN_PIN0 0
#define BTN_PIN1 1
#define BTN_PIN2 2
#define BTN_PIN3 3

#define LED_GREEN 4
#define LED_RED 5

#define LED_ON 1
#define LED_OFF 0

void btn_led_lcd_init(void);

bool btn_lcd_is_pressed(uint8_t pin);

void led_lcd_set(uint8_t led, uint8_t state);

void led_lcd_toggle(uint8_t led);

#endif /* BTN_LED_LCD_H_ */