/*
 * dispatcher_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
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
 * 1. Block until someone wakes you up (all events should trigger the dispatcher only)
 * 2. Check reason for waking up
 * 3. If event than set event group's respective event bit (this action will automatically wake up all "subscribed tasks") (not sure yet)
 * 4. If need for data exchange than get data, look through sub table and push on respective queues
 * */
static void Dispatcher_Task_Handler(void *argument){
	TaskSync_WaitForAll();
	Message_t newMsg;
	while(1){
		SleepUntil((QueueHandle_t)argument,&newMsg,portMAX_DELAY);
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
