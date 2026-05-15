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
#include <App/Tasks/dispatcher_task.h>

static StackType_t xTaskAStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskABuffer;

static StaticQueue_t xTaskAQueueData;
static uint8_t ucTaskAQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskAQueue;

int triggerA;

void TaskA_Handler(void *argument){
	Subscribe("TaskA_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(TASKA_BIT);
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

		if(triggerA == 1){
			triggerA = 0;
			Message_t newMsg;
			newMsg.Topic = TOPIC_SYSTEM_STATE;
			newMsg.Data = 32;
			xStatus = xQueueSendToBack(DispatcherTask_GetQueue(),&newMsg,200);
			if(xStatus != pdPASS){
				printf("Dispatcher queue is full!\r\n");
				fflush(stdout);
			}
		}
		else if(triggerA == 2){
			triggerA = 0;
			Message_t newMsg;
			newMsg.Topic = TOPIC_EXAMPLE2;
			newMsg.Data = 27;
			xStatus = xQueueSendToBack(DispatcherTask_GetQueue(),&newMsg,200);
			if(xStatus != pdPASS){
				printf("Dispatcher queue is full!\r\n");
				fflush(stdout);
			}
		}
		vTaskDelay(100);
	}
}

void TaskA_Task_Init(void){
	xTaskAQueue     = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskAQueueStorageArea, &xTaskAQueueData);
	xTaskCreateStatic(TaskA_Handler,"TaskA_Handler",NORMAL_TASK_STACK_SIZE,(void*)xTaskAQueue,TASKA_PR,xTaskAStack,&xTaskABuffer);
}
