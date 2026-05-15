/*
 * dispatcher_task.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */

#ifndef INC_APP_TASKS_DISPATCHER_TASK_H_
#define INC_APP_TASKS_DISPATCHER_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"

void Dispatcher_Task_Init(void);
QueueHandle_t DispatcherTask_GetQueue(void);

#endif /* INC_APP_TASKS_DISPATCHER_TASK_H_ */
