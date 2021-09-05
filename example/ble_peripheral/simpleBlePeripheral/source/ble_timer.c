/**
 * @file       ble_timer.c
 * @copyright  Copyright (C) 2020 Thuan Le. All rights reserved.
 * @license    This project is released under the Thuan Le License.
 * @version    2.0.0
 * @date       2020-06-11
 * @author     Thuan Le
 * @brief      Timer for smart gadget.
 * @note       None
 */

/* Includes ----------------------------------------------------------- */
#include "ble_timer.h"
#include "OSAL.h"

/* Private define ----------------------------------------------------- */
/* Private variable --------------------------------------------------- */
static uint8_t m_task_id = 0;

/* Function definitions ----------------------------------------------- */
void ble_timer_init(uint8_t task_id)
{
  m_task_id = task_id;
}

void ble_timer_start(uint16_t event_id)
{
  switch (event_id)
  {
  case TIMER_EXPIRED_CLICK_EVT:
    osal_start_timerEx(m_task_id, event_id, TIMER_EXPIRED_CLICK_TIME);
    break;
  
  case TIMER_BUTTON_HANDLER_EVT:
    osal_start_reload_timer(m_task_id, event_id, TIMER_BUTTON_HANDLER_TIME);
    break;
  
  case TIMER_HALL_HANDLER_EVT:
    osal_start_reload_timer(m_task_id, event_id, TIMER_HALL_HANDLER_TIME);
    break;
  
  case TIMER_DISPENSER_DETECTED_EVT:
    osal_start_timerEx(m_task_id, event_id, TIMER_DISPENSER_DETETED_TIME);
    break;
  
  case TIMER_CASE2_EXPIRED_EVT:
    osal_start_timerEx(m_task_id, event_id, TIMER_CASE2_EXPIRED_TIME);
    break;
  
  case TIMER_CASE2_LED_INDICATE_EVT:
    osal_start_reload_timer(m_task_id, event_id, TIMER_CASE2_LED_INDICATE_TIME);
    break;

  case TIMER_WAKEUP_EVT:
    osal_start_reload_timer(m_task_id, event_id, TIMER_WAKEUP_TIME);
    break;
  
  default:
    break;
  }
}

void ble_timer_stop(uint16_t event_id)
{
  osal_stop_timerEx(m_task_id, event_id);
}

uint16_t ble_timer_process_event(uint8_t task_id, uint16_t events)
{
  if (events & TIMER_EXPIRED_CLICK_EVT)
  {
    ble_timer_expired_click();
    return (events ^ TIMER_EXPIRED_CLICK_EVT);
  }
  else if (events & TIMER_BUTTON_HANDLER_EVT)
  {
    ble_timer_button_handler();
    return (events ^ TIMER_BUTTON_HANDLER_EVT);
  }
  else if (events & TIMER_HALL_HANDLER_EVT)
  {
    ble_timer_hall_handler();
    return (events ^ TIMER_HALL_HANDLER_EVT);
  }
  else if (events & TIMER_DISPENSER_DETECTED_EVT)
  {
    ble_timer_dipenser_detected_handler();
    return (events ^ TIMER_DISPENSER_DETECTED_EVT);
  }
  else if (events & TIMER_CASE2_EXPIRED_EVT)
  {
    ble_timer_case2_expired_handler();
    return (events ^ TIMER_CASE2_EXPIRED_EVT);
  }
  else if (events & TIMER_CASE2_LED_INDICATE_EVT)
  {
    ble_timer_case2_led_indicate_handler();
    return (events ^ TIMER_CASE2_LED_INDICATE_EVT);
  }
  else if (events & TIMER_WAKEUP_EVT)
  {
    ble_timer_wakeup_handler();
    return (events ^ TIMER_WAKEUP_EVT);
  }

  return 0;
}

/* End of file -------------------------------------------------------- */
