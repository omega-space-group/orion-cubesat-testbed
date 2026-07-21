/*
 * housekeeping_task.c
 *
 *  Created on: Jul 10, 2026
 *      Author: adaro
 */

/**
 * @file housekeeping_task.c
 * @brief Implementation of the telemetry data acquisition and log/downlink.
 */
#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/housekeeping_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* Task sync bit (if used) and task priority defined in app_config.h */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @param argument  Pointer to task's local queue handle
 * @note Pending implementation! However task sets its health bit periodically so it doesn't cause any problems.
 */
static void Housekeeping_Handler(void *argument){
	Subscribe("Housekeeping_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(HK_BIT);
	Message_t newMsg;
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(5000);
	xLastWakeTime = xTaskGetTickCount();

	while(1){
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		TaskHealth_SetBit(HK_BIT);

		//STEP 1: Request Telemmetry
		//STEP 2: Block until received or time expires
		//STEP 3: Package data
		//STEP 4: Log and/or downlink
	}
}

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void HousekeepingHandler_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(Housekeeping_Handler,"Housekeeping_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,HK_PR,xTaskStack,&xTaskBuffer);
}
