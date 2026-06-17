/*
 * healthMonitor_task.c
 *
 *  Created on: May 18, 2026
 *      Author: adaro
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/healthMonitor_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

int cntHealth,problem = 0;
/* Task sync bit (if used) and task priority defined in app_config.h */
static void HealthMonitor_Handler(void *argument){
	Subscribe("HealthMonitor_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(HM_BIT);
	/* Infinite loop */
	while(1){
		vTaskDelay(5000);
		if(TaskHealth_Read() == ALL_TASKS_OK){
			cntHealth++;
		}
		else{
			problem++;
		}
		TaskHealth_ClearAll();
	}
}

void HealthMonitor_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(HealthMonitor_Handler,"HealthMonitor_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,HM_PR,xTaskStack,&xTaskBuffer);
}
