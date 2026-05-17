/*
 * stateManager_task.c
 *
 *  Created on: May 16, 2026
 *      Author: adaro
 */

#include <App/Tasks/stateManager_task.h>
#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* Task priority and Task sync bit (if used) defined in app_config.h */
static void Task_Handler(void *argument){
	Subscribe("Task_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(STATE_BIT);
	/* Infinite loop */
	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

void StateManager_Task_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(Task_Handler,"Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,STATE_PR,xTaskStack,&xTaskBuffer);
}
