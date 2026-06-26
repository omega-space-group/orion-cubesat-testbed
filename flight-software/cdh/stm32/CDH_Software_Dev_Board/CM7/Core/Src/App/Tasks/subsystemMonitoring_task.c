/*
 * subsystemMonitoring_task.c
 *
 *  Created on: Jun 23, 2026
 *      Author: adaro
 */

#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/subsystemMonitoring_task.h>

#include "usbd_cdc_if.h"
#include <stdio.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* 0: comms, 1: eps, 2: pl, 3: adcs */
uint8_t comms_cnt, eps_cnt, pl_cnt, adcs_cnt = 0;

/* --- SYS-OBC-REL-04 ---*/

/* Task sync bit (if used) and task priority defined in app_config.h */
static void SubsystemMonitor_Handler(void *argument){
	Subscribe("SubsystemMonitor_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(SUBM_BIT);
	Message_t newMsg;
	while(1){
		SleepUntil((QueueHandle_t)argument,&newMsg,portMAX_DELAY);

		switch(newMsg.Data.canPacket.Header.Identifier){
		case 0x203:
			eps_cnt++;
			if(eps_cnt > 3){
				//subsystem hasn't sent a HB in over 15s
				eps_cnt = 0;
				printf("SYSTEM ENTER SAFE MODE COMMAND\r\n");
				fflush(stdout);
				newMsg.Topic = CHANGE_SYSTEM_STATE;
				newMsg.Data.mode = SAFE;
				Publish(newMsg);
			}
			break;
		}
		TaskHealth_SetBit(SUBM_BIT);
	}
}

void SubsystemMonitor_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(SubsystemMonitor_Handler,"SubsystemMonitor_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,SUBM_PR,xTaskStack,&xTaskBuffer);
}


