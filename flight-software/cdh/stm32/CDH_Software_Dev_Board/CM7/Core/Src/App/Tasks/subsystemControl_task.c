/*
 * subsystemControl_task.c
 *
 *  Created on: Jun 26, 2026
 *      Author: adaro
 */
/**
 * @file subsystemControl_task.c
 * @brief .
 */
#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/subsystemControl_task.h>

#include "usbd_cdc_if.h"
#include <stdio.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

static uint8_t eps_cnt, pl_cnt, adcs_cnt = 0;

/* --- SYS-OBC-REL-04 ---*/

/* Task sync bit (if used) and task priority defined in app_config.h */
static void SubsystemControl_Handler(void *argument){
	Subscribe("SubsystemControl_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(SUBC_BIT);
	Message_t newMsg;
	while(1){
		TaskHealth_SetBit(SUBC_BIT);
		SleepUntil((QueueHandle_t)argument,&newMsg,portMAX_DELAY);
		TaskHealth_ClearBit(SUBC_BIT);

		switch(newMsg.Data.canPacket.Header.Identifier){
		case 0x203:
			eps_cnt++;
			if(eps_cnt > 3){
				eps_cnt = 0;
				printf("SYSTEM COMMAND: ENTER SAFE MODE\r\n");
				fflush(stdout);
				newMsg.Topic = CHANGE_SYSTEM_STATE;
				newMsg.Data.mode = SAFE;
				Publish(newMsg);
			}
			break;
		case 0x303:
			adcs_cnt++;
			if(adcs_cnt > 3){
				adcs_cnt = 0;
				printf("SYSTEM COMMAND: ENTER SAFE MODE\r\n");
				fflush(stdout);
				newMsg.Topic = CHANGE_SYSTEM_STATE;
				newMsg.Data.mode = SAFE;
				Publish(newMsg);
			}
			break;
		case 0x403:
			pl_cnt++;
			if(pl_cnt > 3){
				pl_cnt = 0;
				printf("SYSTEM COMMAND: ENTER SAFE MODE\r\n");
				fflush(stdout);
				newMsg.Topic = CHANGE_SYSTEM_STATE;
				newMsg.Data.mode = SAFE;
				Publish(newMsg);
			}
			break;
		}
		TaskHealth_SetBit(SUBC_BIT);
	}
}

void SubsystemControl_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(SubsystemControl_Handler,"SubsystemControl_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,SUBC_PR,xTaskStack,&xTaskBuffer);
}

