/*
 * can_rx_task.h
 *
 *  Created on: Jun 5, 2026
 *      Author: adaro
 */

#ifndef INC_APP_TASKS_CAN_RX_TASK_H_
#define INC_APP_TASKS_CAN_RX_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"

void CAN_RX_Task_Init(void);
QueueHandle_t CAN_RX_Task_GetQueue(void);

#endif /* INC_APP_TASKS_CAN_RX_TASK_H_ */
