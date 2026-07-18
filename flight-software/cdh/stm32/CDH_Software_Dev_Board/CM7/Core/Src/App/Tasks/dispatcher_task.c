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

/**
 * @brief  Main task loop for the system dispatcher.
 * @details Task's logic:
 * 1. Block until all other tasks have finished initialization.
 * 2. Enter infinite loop and set health bit (since task is not periodic).
 * 2. Block indefinitely until a new message is pushed in the master queue.
 * 3. Wake up and clear the health bit and begin execution.
 * 4. Forward the received message to every subscriber task.
 * 6. Set health bit to 1 again and finish.
 * * @param  argument:  Pointer to the Master Queue handle.
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

/**
 * @brief Task's initialization function.
 * @details First master queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void Dispatcher_Task_Init(void){
	xMasterQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucMasterQueueStorageArea, &xMasterQueueData);
	xTaskCreateStatic(Dispatcher_Task_Handler,"Dispatcher_Task_Handler",LARGE_TASK_STACK_SIZE
			,(void*)xMasterQueue,DISPATCHER_PR,xDispatcherTaskStack,&xDispatcherTaskBuffer);
}

/**
 * @brief Global access to master queue
 * @return Queue handle
 */
QueueHandle_t DispatcherTask_GetQueue(void) {
    return xMasterQueue;
}
