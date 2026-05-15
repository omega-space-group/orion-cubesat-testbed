/*
 * dummy_task.h
 *
 *  Created on: May 14, 2026
 *      Author: adaro
 */

#ifndef INC_APP_TASKS_DUMMY_TASK_H_
#define INC_APP_TASKS_DUMMY_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"

void DummyTask_Init(void);
QueueHandle_t DummyTask_GetQueue(void);

#endif /* INC_APP_TASKS_DUMMY_TASK_H_ */
