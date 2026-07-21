/*
 * stateManager_task.c
 *
 *  Created on: May 16, 2026
 *      Author: adaro
 */
/**
 * @file modeManager_task.c
 * @brief Implementation of the System State/Mode FSM.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "fdcan.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Services/app_events.h>
#include <App/Tasks/modeManager_task.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

State_t CurrentMode = SAFE;

/* Task priority and Task sync bit (if used) defined in app_config.h */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details Mode Manager Logic:
 * 1. Subscribe to topics defined in subscriptions.c
 * 2. Set your own sync bit and block until all other tasks have finished initialization before continuing.
 * 3. Enter infinite loop and set health bit (since task is not periodic).
 * 4. Block indefinitely until a new message is pushed in the local queue.
 * 5. Wake up and clear the health bit and begin execution.
 * 6. Run FSM and change current state if change is valid.
 * 7. Publish change internally via Publish() and send a message over FDCAN to the rest of the system.
 * 8. Set health bit to 1 again and finish.
 *
 * @note On boot system enters SAFE mode.
 * @note Task runs every 100ms!
 * @param argument  Pointer to task's local queue handle
 */
static void ModeManager_Handler(void *argument){
	CAN_TxPacket CAN1_Tx;
	Message_t newMsg;
	Subscribe("ModeManager_Handler",(QueueHandle_t)argument);
	TaskSync_SetAndWait(MODE_BIT);
	Message_t newMsgRsc;
	while(1){
		TaskHealth_SetBit(MODE_BIT);
		SleepUntil((QueueHandle_t)argument,&newMsgRsc,portMAX_DELAY);
		TaskHealth_ClearBit(MODE_BIT);
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
			if(NewMode == SAFE){
				CAN1_Tx.Header.Identifier = 0x002;
				printf("SYSTEM COMMAND: ENTER SAFE MODE\r\n");
				fflush(stdout);
			}
			else if(NewMode == NOMINAL) {
				CAN1_Tx.Header.Identifier = 0x003;
				CAN1_Tx.Header.Identifier = 0x002;
				printf("SYSTEM COMMAND: ENTER NOMINAL MODE\r\n");
				fflush(stdout);
			}

			CAN1_Tx.Header.DataLength = FDCAN_DLC_BYTES_0;
			FDCAN_Tx(CAN1_Tx);

			newMsg.Topic = SYSTEM_STATE;
			newMsg.Data.mode = NewMode;
			Publish(newMsg);
			break;
		}
		default:
			break;
		}

		TaskHealth_SetBit(MODE_BIT);
		vTaskDelay(100);
	}
}

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void ModeManager_Task_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(ModeManager_Handler,"ModeManager_Handler",NORMAL_TASK_STACK_SIZE,(void*)xTaskQueue,MODE_PR,xTaskStack,&xTaskBuffer);
}
