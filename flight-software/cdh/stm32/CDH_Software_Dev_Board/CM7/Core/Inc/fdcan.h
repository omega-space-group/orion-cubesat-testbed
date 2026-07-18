/* USER CODE BEGIN Header */
/**
 * @file fdcan.h
 * @brief This file contains all the function prototypes for the fdcan.c file.
 * @details Implementation of the FDCAN peripheral control.
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan1;

/* USER CODE BEGIN Private defines */

/**
 * @brief A custom data structure with the standard FDCAN frame for Tx Messages.
 */
typedef struct {
    FDCAN_TxHeaderTypeDef Header; /**< Configuration header specifying ID, DLC, and frame type. */
    uint8_t               Data[8]; /**< Data payload buffer holding up to 8 bytes of message data. */
} CAN_TxPacket;

/**
 * @brief A custom data structure with the standard FDCAN frame for Rx Messages.
 */
typedef struct {
    FDCAN_RxHeaderTypeDef Header; /**< Configuration header specifying ID, DLC, and frame type. */
    uint8_t               Data[8]; /**< Data payload buffer holding up to 8 bytes of message data. */
} CAN_RxPacket;

/* USER CODE END Private defines */

void MX_FDCAN1_Init(void);

/* USER CODE BEGIN Prototypes */

void FDCAN_Start();
void FDCAN_Tx();

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

