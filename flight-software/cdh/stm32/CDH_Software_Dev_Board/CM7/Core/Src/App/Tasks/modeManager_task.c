/*
 * stateManager_task.c
 *
 *  Created on: May 16, 2026
 *      Author: adaro
 */

#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/modeManager_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* Task priority and Task sync bit (if used) defined in app_config.h */
static void ModeManager_Handler(void *argument){
	Subscribe("ModeManager_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(MODE_BIT);
	Message_t newMsgRsc;
	State_t CurrentMode = SAFE;
	while(1){
		if(SleepUntil((QueueHandle_t)argument,&newMsgRsc,500) == NEW_MSG){
			switch((int)newMsgRsc.Topic){
			case CHANGE_SYSTEM_STATE:{
				State_t NewMode = newMsgRsc.Data.mode;
				switch (CurrentMode) {
				        case SAFE:
				            if (NewMode == NOMINAL) CurrentMode = NOMINAL;
				            else if (NewMode == LOW_POWER) CurrentMode = LOW_POWER;
				            break;

				        case NOMINAL:
				            if (NewMode == SAFE) CurrentMode = SAFE;
				            else if (NewMode == PAYLOAD) CurrentMode = PAYLOAD;
				            else if (NewMode == LOW_POWER) CurrentMode = LOW_POWER;
				            break;

				        case LOW_POWER:
				            if (NewMode == SAFE) CurrentMode = SAFE;
				            else if (NewMode == NOMINAL) CurrentMode = NOMINAL;
				            break;

				        case PAYLOAD:
				            if (NewMode == NOMINAL) CurrentMode = NOMINAL;
				            else if (NewMode == SAFE) CurrentMode = SAFE;
				            break;

				        default:
				        	NewMode = CurrentMode;
				            break;
				    	}
		        Message_t newMsg;
		        newMsg.Topic = SYSTEM_STATE;
		        newMsg.Data.mode = NewMode;
		        Publish(newMsg);
		        break;
			}
			default:
				break;
			}
		}
		vTaskDelay(100);
	}
}

void ModeManager_Task_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(ModeManager_Handler,"ModeManager_Handler",NORMAL_TASK_STACK_SIZE,(void*)xTaskQueue,MODE_PR,xTaskStack,&xTaskBuffer);
}
