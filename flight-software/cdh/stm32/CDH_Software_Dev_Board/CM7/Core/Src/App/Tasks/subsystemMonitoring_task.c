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
static uint8_t cnt[4] = {0};
static uint8_t flag[4] = {0};

void Subsystem_Check(uint8_t data);

/* --- SYS-OBC-REL-04 ---*/

/* Task sync bit (if used) and task priority defined in app_config.h */
static void SubsystemMonitor_Handler(void *argument){
	Subscribe("SubsystemMonitor_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(SUBM_BIT);
	Message_t newMsg;
	while(1){
		if(SleepUntil((QueueHandle_t)argument,&newMsg,5000) == NEW_MSG){
			if(newMsg.Topic == SUBSYSTEM_STATUS){
				switch(newMsg.Data.canPacket.Header.Identifier){
				case 0x103:
//					printf("COMMS HB Received\r\n");
//					fflush(stdout);
					Subsystem_Check(newMsg.Data.canPacket.Data[0]);
					flag[0] = 1;
					break;
				case 0x204:
//					printf("EPS HB Received\r\n");
//					fflush(stdout);
					Subsystem_Check(newMsg.Data.canPacket.Data[0]);
					flag[1] = 1;
					break;
				case 0x304:
//					printf("PL HB Received\r\n");
//					fflush(stdout);
					Subsystem_Check(newMsg.Data.canPacket.Data[0]);
					flag[3] = 1;
					break;
				case 0x404:
//					printf("ADCS HB Received\r\n");
//					fflush(stdout);
					Subsystem_Check(newMsg.Data.canPacket.Data[0]);
					flag[2] = 1;
					break;
				default:
					break;
				}
			}
		}
		else{
			for(int i = 0; i<3; i++){
				if(flag[i]){
					flag[i] = 0;
				}
				else{
					cnt[i]++;
					if(cnt[i] > 3){
						//subsystem hasn't sent a HB in over 15s
						printf("Subsystem Restart CMD\r\n");
						fflush(stdout);
					}
				}
			}
		}
		TaskHealth_SetBit(SUBM_BIT);
	}
}

void SubsystemMonitor_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(SubsystemMonitor_Handler,"SubsystemMonitor_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,SUBM_PR,xTaskStack,&xTaskBuffer);
}

void Subsystem_Check(uint8_t data){
	//Depending on the subsystem STATUS take action accordingly
	switch(data){
	case 0:
		//system in BOOT
		break;
	case 1:
		//system OK
		break;
	case 2:
		//system ERROR
//		printf("Subsystem CRITICAL ERROR!\r\n");
//		fflush(stdout);
		break;
	}
}
