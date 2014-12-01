/**********************************************************************//**
 * @file accident_detection.h
 *
 * @author: Kenneth René Jensen
 * @Version: 0.1
 * @{
		This file holds the logic for detecting an accident
 * @}
 * @note NOT YET Complies MISRO 2004 standards
 *************************************************************************/

#ifndef ACCIDENT_DETECTION_H_
#define ACCIDENT_DETECTION_H_

/**********************************************************************//**
 * @ingroup 
 * @brief Init detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_init(void);

/**********************************************************************//**
 * @ingroup 
 * @brief Starts detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_start(void);

/**********************************************************************//**
 * @ingroup 
 * @brief Stops detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_stop(void);

/**********************************************************************//**
 * @ingroup 
 * @brief Checks if a crash has occurred.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void ad_check_for_crash(void);

/**********************************************************************//**
 * @ingroup 
 * @brief Checks for fire.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void ad_check_for_fire(void); 

extern float EXT_TEMPERATURE;
extern float EXT_TOTAL_ACCELERATION_AVG;

#endif /* ACCIDENT_DETECTION_H_ */