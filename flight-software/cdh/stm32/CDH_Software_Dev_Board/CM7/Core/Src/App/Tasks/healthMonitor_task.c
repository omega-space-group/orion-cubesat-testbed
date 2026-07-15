/*
 * healthMonitor_task.c
 *
 *  Created on: May 18, 2026
 *      Author: adaro
 */
/**
 * @file healthMonitor_task.c
 * @brief Implementation of the background execution check and task accounting.
 */
#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/app_events.h>
#include <App/Tasks/healthMonitor_task.h>

/* Private Static Memory Blocks */
static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

int cntHealth = 0;
int problem = 0;

/* Task sync bit (if used) and task priority defined in app_config.h */
/*
 * @brief  Main execution thread loop for the Health Monitor system.
 * @details Implements a 5-second periodic execution loop. It synchronizes with the system
 * via the `HM_BIT` flag, asserts its own health state, checks the collective
 * status of all flight subsystems, and accounts for missing status logs.
 * * @param  argument: A generic parameter pointer containing the initialization queue handle.
 * * @note   This function runs as a static thread and is blocked for 5000ms intervals.
 * @see    TaskHealth_Read(), TaskHealth_ClearAll()
 */
static void HealthMonitor_Handler(void *argument){
	TaskSync_SetAndWait(HM_BIT);
	/* Infinite loop */
	while(1){
		vTaskDelay(5000);
		TaskHealth_SetBit(HM_BIT);
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
