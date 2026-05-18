/*
 * task_template.c
 *
 *  Created on: May 17, 2026
 *      Author: adaro
 */

#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/modeManager_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* Task sync bit (if used) and task priority defined in app_config.h */
static void _Task_Handler(void *argument){
	Subscribe("_Task_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(0);
	/* Infinite loop */
	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

void _Task_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(_Task_Handler,"_Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,0,xTaskStack,&xTaskBuffer);
}
