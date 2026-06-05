/*
 * taskA_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include <App/Tasks/taskA_task.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>

static StackType_t xTaskAStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskABuffer;

static StaticQueue_t xTaskAQueueData;
static uint8_t ucTaskAQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskAQueue;

int triggerA;

static void TaskA_Handler(void *argument){
	Subscribe("TaskA_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(TASKA_BIT);
	Message_t newMsgRsc;
	while(1){
		SleepUntil((QueueHandle_t)argument,&newMsgRsc,500);

		if(triggerA == 1){
			triggerA = 0;
			Message_t newMsg;
			newMsg.Topic = CHANGE_SYSTEM_STATE;
			newMsg.Data.mode = NOMINAL;
			Publish(newMsg);
		}
		else if(triggerA == 2){
			triggerA = 0;
			Message_t newMsg;
			newMsg.Topic = CHANGE_SYSTEM_STATE;
			newMsg.Data.mode = SAFE;
			Publish(newMsg);
		}
		TaskHealth_SetBit(TASKA_BIT);
		vTaskDelay(100);
	}
}

void TaskA_Task_Init(void){
	xTaskAQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskAQueueStorageArea, &xTaskAQueueData);
	xTaskCreateStatic(TaskA_Handler,"TaskA_Handler",NORMAL_TASK_STACK_SIZE,(void*)xTaskAQueue,TASKA_PR,xTaskAStack,&xTaskABuffer);
}
