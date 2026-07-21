/*
 * telecommandHandler_task.c
 *
 *  Created on: Jul 8, 2026
 *      Author: adaro
 */

/**
 * @file telecommandHandler_task.c
 * @brief Implementation of the parsing and handling of Telecommands received from ground.
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
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details Telecommand Handling Logic:
 * 1. Subscribe to topics defined in subscriptions.c
 * 2. Set your own sync bit and block until all other tasks have finished initialization before continuing.
 * 3. Enter infinite loop and set health bit (since task is not periodic).
 * 4. Block indefinitely until a new message is pushed in the local queue.
 * 5. Wake up and clear the health bit and begin execution.
 * 6. Parse the command (Authenticate -> Decode (Package then PUS))
 * 7. Execute TC if valid.
 * 8. Set health bit to 1 again and finish.
 * @note For testing purposes, the handler responds only to TC to change state to SAFE or NOMINAL at the moment.
 * @param argument  Pointer to task's local queue handle
 */
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

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void TelecommandHandler_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(Telecommand_Handler,"Telecommand_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,TC_PR,xTaskStack,&xTaskBuffer);
}
