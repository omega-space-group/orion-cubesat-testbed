/*
 * subsystemMonitoring_task.c
 *
 *  Created on: Jun 23, 2026
 *      Author: adaro
 */
/**
 * @file subsystemMonitoring_task.c
 * @brief Implementation of the subsystem monitoring logic.
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

/* Task sync bit (if used) and task priority defined in app_config.h */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details The idea is to have every subsystem broadcast a Heartbeat MSG via FDCAN periodically. This message is caught
 * by the OBC (CAN_RX_Handler()) and is recognized as a HB MSG. The status of every subsystem is locally stored inside
 * can_rx_tasl.c and updated on every HB MSG. The Subsystem Monitor Task periodically checks the status of every subsystem
 * and resets it effectively forcing the subsystem to toggle it if its alive and indicate an ERROR if the status is not
 * toggled on the next check. If this occurs over 3 times, a subsystem reset command is send over FDCAN to the subsystem.
 *
 * Subsystem Monitoring Task logic:
 * 1. Set your own sync bit and block until all other tasks have finished initialization before continuing.
 * 2. Periodically and for every subsystem separately:
 * - Check Status
 * - If Status 1, HB has been received so reset it and continue.
 * - If Status 0, assume no new HB and increase error counter. If counter > 3 reset subsystem.
 * - Lastly, set your own health bit and finish.
 * @note Heartbeat MSG structure could/should change to include more information.
 * @param argument  Pointer to task's local queue handle
 */
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

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void SubsystemMonitor_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(SubsystemMonitor_Handler,"SubsystemMonitor_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,SUBM_PR,xTaskStack,&xTaskBuffer);
}


