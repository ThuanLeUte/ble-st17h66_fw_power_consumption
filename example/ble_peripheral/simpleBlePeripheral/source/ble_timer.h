/**
 * @file       ble_timer.h
 * @copyright  Copyright (C) 2020 Thuan Le. All rights reserved.
 * @license    This project is released under the Thuan Le License.
 * @version    1.0.0
 * @date       2020-06-11
 * @author     Thuan Le
 * @brief      Timer for smart gadget.
 * @note       None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BLE_TIMER_H
#define __BLE_TIMER_H

/* Includes ----------------------------------------------------------- */
#include "stdint.h"

/* Public defines ---------------------------------------------------- */
#define TIMER_EXPIRED_CLICK_EVT          (0x0001)
#define TIMER_BUTTON_HANDLER_EVT         (0x0002)
#define TIMER_HALL_HANDLER_EVT           (0x0004)
#define TIMER_DISPENSER_DETECTED_EVT     (0x0008)
#define TIMER_CASE2_EXPIRED_EVT          (0x0010)
#define TIMER_CASE2_LED_INDICATE_EVT     (0x0020)

#define TIMER_EXPIRED_CLICK_TIME         (60 * 1000)           // 60 Seconds
#define TIMER_BUTTON_HANDLER_TIME        (100)                 // 0.1 Seconds
#define TIMER_HALL_HANDLER_TIME          (100)                 // 0.1 Seconds
#define TIMER_DISPENSER_DETETED_TIME     (6 * 60 * 60 * 1000)  // 6 Hours
#define TIMER_CASE2_EXPIRED_TIME         (3 * 60 * 1000)       // 3 Minutes
#define TIMER_CASE2_LED_INDICATE_TIME    (5 * 1000)            // 5 Seconds

/* Public Callbacks -------------------------------------------------- */
/**
 * @brief           Function is called in timer 1s to execute all the functions.
 * 
 * @param           None
 * 
 * @attention       None
 * 
 * @return          None
 */
void ble_timer_expired_click(void); 
void ble_timer_button_handler(void); 
void ble_timer_hall_handler(void);
void ble_timer_dipenser_detected_handler(void);
void ble_timer_case2_expired_handler(void);
void ble_timer_case2_led_indicate_handler(void);

/* Public function prototypes ----------------------------------------- */
/**
 * @brief           Timer 1s init
 *
 * @param[in]       <task_id>  Task ID.
 *  
 * @attention       None
 *
 * @return          None
 */
void ble_timer_init(uint8_t task_id);

/**
 * @brief           Timer stop
 *
 * @param[in]       <task_id>   Task ID.
 *                  <event_id>  Event ID.
 *  
 * @attention       None
 *
 * @return          None
 */
void ble_timer_stop( uint16_t event_id);
void ble_timer_start( uint16_t event_id);

/**
 * @brief           Timer process event 
 *
 * @param[in]       <task_id>   Task ID.
 *                  <events>    Task Event
 *  
 * @attention       None
 *
 * @return          None
 */
uint16_t ble_timer_process_event(uint8_t task_id, uint16_t events);

#endif // __BLE_TIMER_H

/* End of file -------------------------------------------------------- */
