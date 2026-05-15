/*
 * taskB_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include <App/Tasks/taskB_task.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/dispatcher_task.h>

static StackType_t xTaskBStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBBuffer;

static StaticQueue_t xTaskBQueueData;
static uint8_t ucTaskBQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskBQueue;

int triggerB;

void TaskB_Handler(void *argument){
	Subscribe("TaskB_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(TASKB_BIT);
	Message_t newMsgRsc;
	while(1){
		BaseType_t xStatus = xQueueReceive((QueueHandle_t)argument,&newMsgRsc,500);
		if(xStatus == pdPASS){
			printf("%p: Got a new message\r\n",(void*)(QueueHandle_t)argument);
			fflush(stdout);
			//woke up from a msg
		}
		else{
			//timer elapsed
		}

		if(triggerB == 1){
			triggerB = 0;
			Message_t newMsg;
			newMsg.Topic = TOPIC_SYSTEM_STATE;
			newMsg.Data = 5;
			xStatus = xQueueSendToBack(DispatcherTask_GetQueue(),&newMsg,200);
			if(xStatus != pdPASS){
				printf("Dispatcher queue is full!\r\n");
				fflush(stdout);
			}
		}
		else if(triggerB == 2){
			triggerB = 0;
			Message_t newMsg;
			newMsg.Topic = TOPIC_EXAMPLE1;
			newMsg.Data = 15;
			xStatus = xQueueSendToBack(DispatcherTask_GetQueue(),&newMsg,200);
			if(xStatus != pdPASS){
				printf("Dispatcher queue is full!\r\n");
				fflush(stdout);
			}
		}
		vTaskDelay(100);
	}
}

void TaskB_Init(void){
	xTaskBQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskBQueueStorageArea, &xTaskBQueueData);
	xTaskCreateStatic(TaskB_Handler,"TaskB_Handler",NORMAL_TASK_STACK_SIZE,(void*)xTaskBQueue,TASKB_PR,xTaskBStack,&xTaskBBuffer);
}
