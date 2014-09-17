/*
 * ab_encoder.h
 *
 * Created: 15-03-2014 15:04:58
 *  Author: IHA
 */ 


#ifndef AB_ENCODER_H_
#define AB_ENCODER_H_

void ab_encoder_init();
int16_t ab_encoder_current_position(uint8_t reset_after_read);

#endif /* AB-ENCODER_H_ */