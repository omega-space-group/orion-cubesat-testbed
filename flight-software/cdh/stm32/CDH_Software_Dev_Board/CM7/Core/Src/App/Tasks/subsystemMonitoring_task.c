/*
 * subsystemMonitoring_task.c
 *
 *  Created on: Jun 23, 2026
 *      Author: adaro
 */

#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/app_events.h>
#include <App/Tasks/subsystemMonitoring_task.h>
#include <App/Tasks/can_rx_task.h>

#include "usbd_cdc_if.h"
#include <stdio.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

/* 0: comms, 1: eps, 2: pl, 3: adcs */
static uint8_t comms_cnt, eps_cnt, pl_cnt, adcs_cnt = 0;

/* --- SYS-OBC-REL-04 ---*/

/* Task sync bit (if used) and task priority defined in app_config.h */
static void SubsystemMonitor_Handler(void *argument){
	TaskSync_SetAndWait(SUBM_BIT);
	while(1){
		vTaskDelay(2000);

		if(GetSubsystemStatus(COMMS)){
			SetSubsystemStatus(COMMS,0);
		}
		else{
			comms_cnt++;
			if(comms_cnt >= 3){
				printf("COMMS COMMAND: RESET\r\n");
				fflush(stdout);
				comms_cnt = 0;
			}
		}

		if(GetSubsystemStatus(EPS)){
			SetSubsystemStatus(EPS,0);
		}
		else{
			eps_cnt++;
			if(eps_cnt >= 3){
				printf("EPS COMMAND: RESET\r\n");
				fflush(stdout);
				eps_cnt = 0;
			}
		}

		if(GetSubsystemStatus(ADCS)){
			SetSubsystemStatus(ADCS,0);
		}
		else{
			adcs_cnt++;
			if(adcs_cnt >= 3){
				printf("ADCS COMMAND: RESET\r\n");
				fflush(stdout);
				adcs_cnt = 0;
			}
		}

		if(GetSubsystemStatus(PL)){
			SetSubsystemStatus(PL,0);
		}
		else{
			pl_cnt++;
			if(pl_cnt >= 3){
				printf("PAYLOAD COMMAND: RESET\r\n");
				fflush(stdout);
				pl_cnt = 0;
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


