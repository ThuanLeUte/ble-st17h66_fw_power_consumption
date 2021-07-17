/**
 * @file       ble_dvs_services.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    2.0.0
 * @date       2020-06-02
 * @author     Thuan Le
 * @brief      DSS (Device Seting Service)
 * @note       None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BLE_DVS_SERVICES_H
#define __BLE_DVS_SERVICES_H

/* Includes ----------------------------------------------------------- */
#include "att.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum 
{
  DSS_ID_SERVICE = 0x00,
  DSS_ID_CHAR_IDENT_SETTING,
  DSS_ID_CHAR_MODE_SETTING,
  DSS_ID_MAX
}
dss_id_t;

typedef enum
{
  DSS_EVT_IDENT_SETTING_RECEIVED,
  DSS_EVT_MODE_SETTING_RECEIVED,
  DSS_EVT_IDENT_SETTING_READ,
  DSS_EVT_MODE_SETTING_READ
}
dss_evt_id_t;

typedef struct
{
  dss_evt_id_t  evt_id;
  dss_id_t      char_id;
  void*         data;
}
dss_evt_t;

typedef void (*dss_cb_t)(dss_evt_t* pev);

/* Public function prototypes ----------------------------------------- */
/**
 * @brief      Initializes the BLE service by registering
 *             GATT attributes with the GATT server.
 *
 * @param[in]  None
 *
 * @return
 *  - 0: Success
 *  - 1: Failure
 */
bStatus_t dss_add_service(dss_cb_t cb);

/**
 * @brief      Set characterictics value.
 *
 * @param[in]  char_id        Characteristic ID
 *             len            Length of `value`
 *             value          Pointer to data to write. This is dependent on
 *                            the parameter ID and WILL be cast to the appropriate 
 *                            data type (example: data type of uint16 will be cast to 
 *                            uint16 pointer).
 *
 * @return
 *  - SUCCESS
 *  - INVALIDPARAMETER
 */
bStatus_t dss_set_parameter(dss_id_t char_id, void *value);
/**
 * @brief      Get characterictics value.
 *
 * @param[in]  char_id        Characteristic ID
 *             value          Pointer to data to write. This is dependent on
 *                            the parameter ID and WILL be cast to the appropriate 
 *                            data type (example: data type of uint16 will be cast to 
 *                            uint16 pointer).
 *
 * @return
 *  - SUCCESS
 *  - INVALIDPARAMETER
 */
bStatus_t dss_get_parameter(dss_id_t char_id, void *value);

/**
 * @brief       Send a notification containing a humidity measurement.
 *
 * @param[in]  char_id        Characteristic ID
 *             conn_handle    Connection handle
 *             p_noti         Pointer to notification structure.
 *
 * @return     Success or Failure
 * 
 */
bStatus_t dss_notify(dss_id_t char_id, uint16 conn_handle, attHandleValueNoti_t *p_noti);

#endif // __BLE_DVS_SERVICES_H

/* End of file -------------------------------------------------------- */
