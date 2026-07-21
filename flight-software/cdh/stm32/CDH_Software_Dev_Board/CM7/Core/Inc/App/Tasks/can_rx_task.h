/*
 * can_rx_task.h
 *
 *  Created on: Jun 5, 2026
 *      Author: adaro
 */
/**
 * @file can_rx_task.h
 * @brief This file contains all the function prototypes for the can_rx_task.c file.
 * @details Implementation of the CAN Rx Task responsible for parsing any received FDCAN messages.
 * For more details look into CAN_RX_Handler().
 */
#ifndef INC_APP_TASKS_CAN_RX_TASK_H_
#define INC_APP_TASKS_CAN_RX_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"

/**
 * @brief  Subsystem enumeration
 * @note This enum should be modified to accommodate extra subsystems
 */
typedef enum{
	COMMS = 0, /**< Communication Subsystem */
	EPS   = 1, /**< Electrical Power System */
	PL    = 2, /**< Payload */
	ADCS  = 3  /**< Attitude Determination and Control System */
}subsystemName;

void CAN_RX_Task_Init(void);
QueueHandle_t CAN_RX_Task_GetQueue(void);
int GetSubsystemStatus(subsystemName sub);
void SetSubsystemStatus(subsystemName sub, int status);

#endif /* INC_APP_TASKS_CAN_RX_TASK_H_ */
