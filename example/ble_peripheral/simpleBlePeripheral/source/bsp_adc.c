/**
 * @file       bsp_acd.h
 * @copyright  Copyright (C) 2020 Thuan Le. All rights reserved.
 * @license    This project is released under the Thuan Le License.
 * @version    1.0.0
 * @date       2021-04-21
 * @author     Thuan Le
 * @brief      Board Support Package ADC
 * @note       None
 */

/* Includes ---------------------------------------------------------- */
#include "OSAL.h"
#include "gpio.h"
#include "clock.h"
#include "adc.h"
#include "bsp_adc.h"
#include "log.h"
#include "damos_ram.h"
#include "battservice.h"

/* Private defines ---------------------------------------------------- */
#define BATT_VOLTAGE_MAX (800)
#define BATT_VOLTAGE_MIN (640)

/* Private variable --------------------------------------------------- */
static uint8 adc_demo_task_id;

static adc_Cfg_t adc_cfg = {
    .channel              = ADC_BIT(ADC_CH2P_P14) | ADC_BIT(ADC_CH3N_P15) | ADC_BIT(ADC_CH3P_P20),
    .is_continue_mode     = FALSE,
    .is_differential_mode = 0x00,
    .is_high_resolution   = 0x7f,
};

/* Private function prototypes ---------------------------------------- */
static void m_bsp_adc_measure_task(void);
static void m_bsp_adc_evt(adc_Evt_t *pev);

/* Function definitions ----------------------------------------------- */
void bsp_adc_init(uint8 task_id)
{
  adc_demo_task_id = task_id;
  m_bsp_adc_measure_task();
}

uint16 bsp_adc_process_event(uint8 task_id, uint16 events)
{
  VOID task_id;

  if (events & ADC_MEASURE_TASK_EVT)
  {
    m_bsp_adc_measure_task();
    return (events ^ ADC_MEASURE_TASK_EVT);
  }

  return 0;
}
/* Private Function definitions ----------------------------------------------- */
static void m_bsp_adc_evt(adc_Evt_t *pev)
{
  float   value                = 0;
  bool    is_high_resolution   = TRUE;
  bool    is_differential_mode = FALSE;

  if (pev->ch == ADC_CH2P_P14)
  {
    value = hal_adc_value_cal((adc_CH_t)pev->ch, pev->data, pev->size, is_high_resolution, is_differential_mode);

    uint16_t batt_voltage;

    batt_voltage = (int)(value * 1000);

    if (BATT_VOLTAGE_MAX <= batt_voltage)
      battery_level = 100;
    else if (BATT_VOLTAGE_MIN >= batt_voltage)
      battery_level = 0;
    else
      battery_level = (float)((float)(batt_voltage - BATT_VOLTAGE_MIN) / (float)(BATT_VOLTAGE_MAX - BATT_VOLTAGE_MIN)) * 100;

    LOG("P%d %d mv\n", 14, (int)(value * 1000));
    LOG("Battery Level: %d\n", battery_level);
    LOG("Send batt BLE\n");
    Batt_MeasLevel();
  }

  if (adc_cfg.is_continue_mode == FALSE)
    osal_start_timerEx(adc_demo_task_id, ADC_MEASURE_TASK_EVT, 1000);
}

static void m_bsp_adc_measure_task(void)
{
  hal_adc_config_channel(adc_cfg, m_bsp_adc_evt);
  hal_adc_start();
}
