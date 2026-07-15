/*
 * dispatcher_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */

/**
 * @file dispatcher_task.c
 * @brief Implementation of the dispatcher and master queue logic.
 */
#include <App/Tasks/dispatcher_task.h>
#include "FreeRTOS.h"
#include "queue.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>

static StackType_t xDispatcherTaskStack[LARGE_TASK_STACK_SIZE];
static StaticTask_t xDispatcherTaskBuffer;

static StaticQueue_t xMasterQueueData;
static uint8_t ucMasterQueueStorageArea[MASTER_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xMasterQueue;

/*
 * @brief  Main task loop for the system dispatcher.
 * @note   This is an internal task handler managed strictly by FreeRTOS.
 * * @details This task orchestrates system events by implementing the following pipeline:
 * 1. Blocks indefinitely until a new message arrives via the master queue.
 * 2. Temporarily clears the health monitoring status during execution.
 * 3. Forwards the received `Message_t` packet to sub-tables via DispatcherSend().
 * 4. Sets the health bit high again before reverting back to a blocked state.
 * * @param  argument: A pointer to the Master Queue handle (`QueueHandle_t`).
 */
static void Dispatcher_Task_Handler(void *argument){
	TaskSync_WaitForAll();
	Message_t newMsg;
	while(1){
		TaskHealth_SetBit(DISPATCHER_BIT);
		SleepUntil((QueueHandle_t)argument,&newMsg,portMAX_DELAY);
		TaskHealth_ClearBit(DISPATCHER_BIT);
		DispatcherSend(newMsg);

		TaskHealth_SetBit(DISPATCHER_BIT);
	}
}

void Dispatcher_Task_Init(void){
	xMasterQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucMasterQueueStorageArea, &xMasterQueueData);
	xTaskCreateStatic(Dispatcher_Task_Handler,"Dispatcher_Task_Handler",LARGE_TASK_STACK_SIZE
			,(void*)xMasterQueue,DISPATCHER_PR,xDispatcherTaskStack,&xDispatcherTaskBuffer);
}

QueueHandle_t DispatcherTask_GetQueue(void) {
    return xMasterQueue;
}
