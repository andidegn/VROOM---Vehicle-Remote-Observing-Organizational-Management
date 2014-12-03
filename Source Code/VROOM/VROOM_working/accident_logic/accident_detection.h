/**********************************************************************//**
 * @file accident_detection.h
 *
 * @author Andi Degn
 * @author Kenneth René Jensen
 * @version 0.1
 * @defgroup ac_det Accident Detection
 * @{
	@ingroup accident_logic
	@brief This file holds the logic for detecting an accident
	@note NOT YET Complies MISRO 2004 standards
	@defgroup ac_det_pub Public
	@defgroup ac_det_priv Private
 * @}
 *************************************************************************/

#ifndef ACCIDENT_DETECTION_H_
#define ACCIDENT_DETECTION_H_

/**********************************************************************//**
 * @ingroup ac_det_pub
 * @brief Init detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_init(void);

/**********************************************************************//**
 * @ingroup ac_det_pub
 * @brief Starts detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_start(void);

/**********************************************************************//**
 * @ingroup ac_det_pub
 * @brief Stops detection for accidents.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void accident_detection_stop(void);

/**********************************************************************//**
 * @ingroup ac_det_pub
 * @brief Checks if a crash has occurred.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void check_for_crash(void);

/**********************************************************************//**
 * @ingroup ac_det_pub
 * @brief Checks for fire.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void check_for_fire(void);

#endif /* ACCIDENT_DETECTION_H_ */