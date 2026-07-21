/*
 * telecommandHandler_task.c
 *
 *  Created on: Jul 8, 2026
 *      Author: adaro
 */


#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/telecommandHandler_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* Task sync bit (if used) and task priority defined in app_config.h */
static void Telecommand_Handler(void *argument){
	Subscribe("Telecommand_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(TC_BIT);
	Message_t newMsg;
	while(1){
		TaskHealth_SetBit(TC_BIT);
		SleepUntil((QueueHandle_t)argument,&newMsg,portMAX_DELAY);
		TaskHealth_ClearBit(TC_BIT);

		//STEP 1: Authenticate
		//STEP 2: Decode Package
		//STEP 3: Decode PUS
		//STEP 4: Handle TC
		switch(newMsg.Data.mode){
		case SAFE:
			newMsg.Topic = CHANGE_SYSTEM_STATE;
			newMsg.Data.mode = SAFE;
			Publish(newMsg);
			break;
		case NOMINAL:
			newMsg.Topic = CHANGE_SYSTEM_STATE;
			newMsg.Data.mode = NOMINAL;
			Publish(newMsg);
			break;
		default:
			break;
		}

		TaskHealth_SetBit(TC_BIT);
	}
}

void TelecommandHandler_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(Telecommand_Handler,"Telecommand_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,TC_PR,xTaskStack,&xTaskBuffer);
}
