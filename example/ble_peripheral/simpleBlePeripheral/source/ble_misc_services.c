/**
 * @file       ble_misc_services.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    2.0.0
 * @date       2020-06-02
 * @author     Thuan Le
 * @brief      MCS (Miscellaneous Service)
 * @note       None
 */

/* Includes ----------------------------------------------------------- */
#include "ble_misc_services.h"
#include "log.h"

#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "damos_ram.h"

/* Private Defines ---------------------------------------------------------- */
#define MCS_UUID_SERV                      (0xFFF3)
#define MCS_UUID_CHAR_IDENTIFICATION       (0xFFF4)
#define MCS_UUID_CHAR_MODE_SELECTION       (0xFFF5)
#define MCS_UUID_CHAR_CLICK_AVAILABLE      (0xFFF6)
#define MCS_UUID_CHAR_BOTTLE_REPLACEMENT   (0xFFF7)

#define CHAR_IDENTIFICATION_VALUE_POS      (2)
#define CHAR_MODE_SELECTION_VALUE_POS      (4)
#define CHAR_CLICK_COUNT_VALUE_POS         (6)
#define CHAR_BOTTLE_REPLACEMENT_VALUE_POS  (8)

/* Private Macros ----------------------------------------------------------- */
/* Private Defines ---------------------------------------------------------- */
// GATT Profile Miscellaneous Service UUID
static CONST uint8 MCS_UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MCS_UUID_SERV), HI_UINT16(MCS_UUID_SERV)
};

// Characteristic Identification UUID
static CONST uint8 MCS_CHAR_IDENTIFICATION_UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MCS_UUID_CHAR_IDENTIFICATION), HI_UINT16(MCS_UUID_CHAR_IDENTIFICATION)
};

// Characteristic Mode Selection UUID
static CONST uint8 MCS_CHAR_MODE_SELECTION_UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MCS_UUID_CHAR_MODE_SELECTION), HI_UINT16(MCS_UUID_CHAR_MODE_SELECTION)
};

// Characteristic Click Available UUID
static CONST uint8 MCS_CHAR_CLICK_COUNT_UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MCS_UUID_CHAR_CLICK_AVAILABLE), HI_UINT16(MCS_UUID_CHAR_CLICK_AVAILABLE)
};

// Characteristic Bottle Replacement UUID
static CONST uint8 MCS_CHAR_BOTTLE_REPLACEMENT_UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MCS_UUID_CHAR_BOTTLE_REPLACEMENT), HI_UINT16(MCS_UUID_CHAR_BOTTLE_REPLACEMENT)
};

// Characterictic property
uint8_t MCS_CHAR_PROPS[] =
{
  0,
  GATT_PROP_READ | GATT_PROP_WRITE |GATT_PROP_NOTIFY,
  GATT_PROP_READ | GATT_PROP_WRITE |GATT_PROP_NOTIFY,
  GATT_PROP_READ | GATT_PROP_WRITE |GATT_PROP_NOTIFY,
  GATT_PROP_READ | GATT_PROP_WRITE |GATT_PROP_NOTIFY,
};

// Profile Service attribute
static CONST gattAttrType_t mcs_service = { ATT_BT_UUID_SIZE, MCS_UUID };

// Profile struct
static struct
{
  mcs_cb_t app_cb;
  struct
  {
    struct
    {
      uint8_t identification[4];     // Charaterictic identification value;
      uint8_t mode_selection[1];     // Charaterictic mode selection value;
      uint8_t click_count[1];        // Charaterictic click availble value
      uint8_t bottle_replacement[1]; // Charaterictic bottle replacement value;
    }
    value;
  }
  chars;
}
m_mcs;

// Profile atrribute
static gattAttribute_t mcs_atrr_tbl[] =
{
  // Profile Service
  {
    {ATT_BT_UUID_SIZE, primaryServiceUUID}, /* type */
    GATT_PERMIT_READ,                       /* permissions */
    0,                                      /* handle */
    (uint8 *)&mcs_service                   /* p_value */
  },

  // Characteristic Identification Declaration
  {
    {ATT_BT_UUID_SIZE, characterUUID},
    GATT_PERMIT_READ,
    0,
    &MCS_CHAR_PROPS[MCS_ID_CHAR_IDENTIFICATON]
  },

  // Characteristic Identification Value
  {
    {ATT_BT_UUID_SIZE, MCS_CHAR_IDENTIFICATION_UUID},
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    m_mcs.chars.value.identification
  },

  // Characteristic Mode Selection Declaration
  {
    {ATT_BT_UUID_SIZE, characterUUID},
    GATT_PERMIT_READ,
    0,
    &MCS_CHAR_PROPS[MCS_ID_CHAR_MODE_SELECTION]
  },

  // Characteristic Mode Selection Value
  {
    {ATT_BT_UUID_SIZE, MCS_CHAR_MODE_SELECTION_UUID},
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    m_mcs.chars.value.mode_selection
  },

  // Characteristic Click Availble Declaration
  {
    {ATT_BT_UUID_SIZE, characterUUID},
    GATT_PERMIT_READ,
    0,
    &MCS_CHAR_PROPS[MCS_ID_CHAR_CLICK_COUNT]
  },

  // Characteristic Click Availble Value
  {
    {ATT_BT_UUID_SIZE, MCS_CHAR_CLICK_COUNT_UUID},
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    m_mcs.chars.value.click_count
  },

  // Characteristic Bottle Replacement Declaration
  {
    {ATT_BT_UUID_SIZE, characterUUID},
    GATT_PERMIT_READ,
    0,
    &MCS_CHAR_PROPS[MCS_ID_CHAR_BOTTLE_REPLACEMENT]
  },

  // Characteristic Bottle Replacement Value
  {
    {ATT_BT_UUID_SIZE, MCS_CHAR_BOTTLE_REPLACEMENT_UUID},
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    m_mcs.chars.value.bottle_replacement
  },
};

/* Private function prototypes ---------------------------------------- */
static bStatus_t mcs_read_attr_cb(uint16           conn_handle,
                                  gattAttribute_t *p_attr,
                                  uint8           *p_value, 
                                  uint8           *p_len, 
                                  uint16           offset, 
                                  uint8            max_len);

static bStatus_t mcs_write_attr_cb(uint16          conn_handle, 
                                  gattAttribute_t *p_attr,
                                  uint8           *p_value, 
                                  uint8            len, 
                                  uint16           offset);

/*********************************************************************
 * PROFILE CALLBACKS
 */
static CONST gattServiceCBs_t mcs_callbacks =
{
  mcs_read_attr_cb,
  mcs_write_attr_cb,
  NULL
};

/* Public function ----------------------------------------- */
bStatus_t mcs_add_service(mcs_cb_t cb)
{
  uint8 status = SUCCESS;

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService(mcs_atrr_tbl,
                                       GATT_NUM_ATTRS(mcs_atrr_tbl),
                                       &mcs_callbacks);

  LOG("mcs_add_service:%x\n", status);

  m_mcs.app_cb = cb;

  return ( status );
}

bStatus_t mcs_set_parameter(mcs_id_t char_id, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (char_id)
  {
  case MCS_ID_CHAR_CLICK_COUNT:
    osal_memcpy(m_mcs.chars.value.click_count, value, sizeof(m_mcs.chars.value.click_count));
    break;

  case MCS_ID_CHAR_IDENTIFICATON:
    osal_memcpy(m_mcs.chars.value.identification, value, sizeof(m_mcs.chars.value.identification));
    break;

  case MCS_ID_CHAR_MODE_SELECTION:
    osal_memcpy(m_mcs.chars.value.mode_selection, value, sizeof(m_mcs.chars.value.mode_selection));
    break;

  case MCS_ID_CHAR_BOTTLE_REPLACEMENT:
    osal_memcpy(m_mcs.chars.value.bottle_replacement, value, sizeof(m_mcs.chars.value.bottle_replacement));
    break;

  default:
    ret = INVALIDPARAMETER;
    break;
  }

  return (ret);
}

bStatus_t mcs_get_parameter(mcs_id_t char_id, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (char_id)
  {
  case MCS_ID_CHAR_CLICK_COUNT:
    osal_memcpy(value, m_mcs.chars.value.click_count, sizeof(m_mcs.chars.value.click_count));
    break;

  case MCS_ID_CHAR_IDENTIFICATON:
    osal_memcpy(value, m_mcs.chars.value.identification, sizeof(m_mcs.chars.value.identification));
    break;

  case MCS_ID_CHAR_MODE_SELECTION:
    osal_memcpy(value, m_mcs.chars.value.mode_selection, sizeof(m_mcs.chars.value.mode_selection));
    break;

  case MCS_ID_CHAR_BOTTLE_REPLACEMENT:
    osal_memcpy(value, m_mcs.chars.value.bottle_replacement, sizeof(m_mcs.chars.value.bottle_replacement));
    break;

  default:
    ret = INVALIDPARAMETER;
    break;
  }

  return (ret);
}

bStatus_t mcs_notify(mcs_id_t char_id, uint16 conn_handle, attHandleValueNoti_t *p_noti)
{
  bStatus_t ret;

  // Set the handle
  switch (char_id)
  {
  case MCS_ID_CHAR_CLICK_COUNT:
    p_noti->handle = mcs_atrr_tbl[CHAR_CLICK_COUNT_VALUE_POS].handle;
    break;

  case MCS_ID_CHAR_IDENTIFICATON:
    p_noti->handle = mcs_atrr_tbl[CHAR_IDENTIFICATION_VALUE_POS].handle;
    break;

  case MCS_ID_CHAR_MODE_SELECTION:
    p_noti->handle = mcs_atrr_tbl[CHAR_MODE_SELECTION_VALUE_POS].handle;
    break;
 
  case MCS_ID_CHAR_BOTTLE_REPLACEMENT:
    p_noti->handle = mcs_atrr_tbl[CHAR_BOTTLE_REPLACEMENT_VALUE_POS].handle;
    break;

  default:
    break;
  }

  ret = GATT_Notification(conn_handle, p_noti, FALSE);

  if (SUCCESS == ret)
  {
    LOG("mcs_notify success\n");
  }

  return ret; 
}

/* Private Function definitions ----------------------------------------------- */
/**
 * @brief       Write an attribute.
 *
 * @param[in]   conn_handle     Connection message was received on
 *              p_attr          Pointer to attribute
 *              p_value         Pointer to data to be read
 *              p_len           Length of data to be read
 *              offset          Offset of the first octet to be read
 *
 * @return      Success or Failure
 */
static bStatus_t mcs_write_attr_cb(uint16          conn_handle, 
                                  gattAttribute_t *p_attr,
                                  uint8           *p_value, 
                                  uint8            len, 
                                  uint16           offset)
{
  bStatus_t status = SUCCESS;
  mcs_evt_t evt;

  LOG("Miscellaneous write callback\n");

  // If attribute permissions require authorization to write, return error
  if (gattPermitAuthorWrite(p_attr->permissions))
    return (ATT_ERR_INSUFFICIENT_AUTHOR); // Insufficient authorization

  if (p_attr->type.len == ATT_BT_UUID_SIZE)
  {
    uint16 uuid = BUILD_UINT16(p_attr->type.uuid[0], p_attr->type.uuid[1]);

    switch (uuid)
    {
    case MCS_UUID_CHAR_IDENTIFICATION:
      osal_memcpy(m_mcs.chars.value.identification, p_value, 4);

      evt.evt_id = MSC_EVT_IDENTIFICATON_RECEIVED;
      m_mcs.app_cb(&evt);
      break;

    case MCS_UUID_CHAR_MODE_SELECTION:
      osal_memcpy(m_mcs.chars.value.mode_selection, p_value, 1);

      evt.evt_id = MSC_EVT_MODE_RECEIVED;
      m_mcs.app_cb(&evt);
      break;

    default:
      break;
    }
  }

  return (status);
}

/**
 * @brief       Read an attribute.
 *
 * @param[in]   conn_handle     Connection message was received on
 *              p_attr          Pointer to attribute
 *              p_value         Pointer to data to be read
 *              p_len           Length of data to be read
 *              offset          Offset of the first octet to be read
 *              max_len         Maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 mcs_read_attr_cb(uint16 conn_handle,
                              gattAttribute_t *p_attr,
                              uint8 *p_value,
                              uint8 *p_len,
                              uint16 offset,
                              uint8 max_len)
{
  bStatus_t status = SUCCESS;
  mcs_evt_t evt;

  LOG("Miscellaneous read callback\n");

  // If attribute permissions require authorization to write, return error
  if (gattPermitAuthorWrite(p_attr->permissions))
    return (ATT_ERR_INSUFFICIENT_AUTHOR); // Insufficient authorization

  if (p_attr->type.len == ATT_BT_UUID_SIZE)
  {
    uint16 uuid = BUILD_UINT16(p_attr->type.uuid[0], p_attr->type.uuid[1]);

    switch (uuid)
    {
    case MCS_UUID_CHAR_IDENTIFICATION:
      evt.evt_id = MSC_EVT_IDENTIFICATON_READ;
      m_mcs.app_cb(&evt);

      *p_len = 4;
      osal_memcpy(p_value, (uint8_t *) m_mcs.chars.value.identification, 4);
      break;

    case MCS_UUID_CHAR_MODE_SELECTION:
      evt.evt_id = MSC_EVT_MODE_READ;
      m_mcs.app_cb(&evt);

      *p_len = 1;
      osal_memcpy(p_value, m_mcs.chars.value.mode_selection, 1);
      break;

    case MCS_UUID_CHAR_CLICK_AVAILABLE:
      evt.evt_id = MSC_EVT_CLICK_COUNT_READ;
      m_mcs.app_cb(&evt);

      *p_len = 1;
      osal_memcpy(p_value, m_mcs.chars.value.click_count, 1);
      break;

    case MCS_UUID_CHAR_BOTTLE_REPLACEMENT:
      evt.evt_id = MSC_EVT_BOTTLE_REPLACEMENT_READ;
      m_mcs.app_cb(&evt);

      *p_len = 1;
      osal_memcpy(p_value, m_mcs.chars.value.bottle_replacement, 1);
      break;

    default:
      break;
    }
  }

  return (status);
}

/* End of file -------------------------------------------------------- */
