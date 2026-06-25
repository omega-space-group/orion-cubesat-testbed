/*
 * dummy_tasks.c
 *
 *  Created on: May 14, 2026
 *      Author: adaro
 */

#include <App/Tasks/dummy_task.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "fdcan.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>

static StackType_t xDummyTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xDummyTaskBuffer;

static StaticQueue_t xDummyTaskQueueData;
static uint8_t ucDummyTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xDummyTaskQueue;

static void Dummy_Task_Handler(void *argument){
	Subscribe("Dummy_Task_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(DUMMY_BIT);
	Message_t newMsg;
	while(1){
		SleepUntil((QueueHandle_t)argument,&newMsg,1000);
  		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
//	  	FDCAN_Tx();

	  	TaskHealth_SetBit(DUMMY_BIT);
	}
}


void DummyTask_Init(void) {
	xDummyTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucDummyTaskQueueStorageArea, &xDummyTaskQueueData);
	xTaskCreateStatic(Dummy_Task_Handler,"Dummy_Task_Handler",NORMAL_TASK_STACK_SIZE
			  ,(void*)xDummyTaskQueue,DUMMY_PR,xDummyTaskStack,&xDummyTaskBuffer);
}
