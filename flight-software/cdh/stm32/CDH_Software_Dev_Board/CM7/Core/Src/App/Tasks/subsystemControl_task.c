/*
 * subsystemControl_task.c
 *
 *  Created on: Jun 26, 2026
 *      Author: adaro
 */
/**
 * @file subsystemControl_task.c
 * @brief Implementation of the subsystem control logic.
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

/* Task sync bit (if used) and task priority defined in app_config.h */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details Subsystem Control Logic:
 * 1. Subscribe to topics defined in subscriptions.c
 * 2. Set your own sync bit and block until all other tasks have finished initialization before continuing.
 * 3. Enter infinite loop and set health bit (since task is not periodic).
 * 4. Block indefinitely until a new message is pushed in the local queue.
 * 5. Wake up and clear the health bit and begin execution.
 * 6. Decode the message.
 * 7. If ERROR MSG and same ERROR MSG has occurred over 3 times, send reset command on the relative subsystem.
 * 8. Set health bit to 1 again and finish.
 * @param argument  Pointer to task's local queue handle
 */
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

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void SubsystemControl_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(SubsystemControl_Handler,"SubsystemControl_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,SUBC_PR,xTaskStack,&xTaskBuffer);
}

