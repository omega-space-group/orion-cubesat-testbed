/*
 * healthMonitor_task.c
 *
 *  Created on: May 18, 2026
 *      Author: adaro
 */
/**
 * @file healthMonitor_task.c
 * @brief Implementation of the background execution check and task accounting.
 */
#include "FreeRTOS.h"
#include "task.h"

#include <App/app_config.h>
#include <App/Services/app_events.h>
#include <App/Tasks/healthMonitor_task.h>

/* Private Static Memory Blocks */
static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;

int cntHealth = 0;
int problem = 0;

/* Task sync bit (if used) and task priority defined in app_config.h */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details The idea here is to have a mask (xTaskHealthEvent) with the number of bits equal to the number of tasks initialized.
 * Each task has its own place on the mask, which is given by the Health and Sync bit (see app_config.h).
 *
 * -> Every time a task finishes an execution, it sets its bit to 1.
 * -> If the task is not periodic and blocks indefinitely then the task sets its bit to 1 before it sleeps to prevent any false
 * triggers. When the task is woken up, it clears its bit, executes its main loop and sets its bit again after
 * finishing execution.
 *
 *
 * Health Monitoring Task logic:
 * 1. Set your own sync bit and block until all other tasks have finished initialization before continuing.
 * 2. Periodically:
 * - Check if all tasks are making progress by checking that the mask is equal to ALL_TASKS_OK. That would mean that
 * all tasks have set their bit and finished execution at least ones, so no starvation or hangs!
 * - If check okay then increment a local health variable. Else, increment a different local variable.
 * - Lastly, clear the mask to reset the process.
 * @note Task runs every 5sec!
 * @param argument  Pointer to task's local queue handle
 */
static void HealthMonitor_Handler(void *argument){
	TaskSync_SetAndWait(HM_BIT);
	/* Infinite loop */
	while(1){
		vTaskDelay(5000);
		TaskHealth_SetBit(HM_BIT);
		if(TaskHealth_Read() == ALL_TASKS_OK){
			cntHealth++;
		}
		else{
			problem++;
		}
		TaskHealth_ClearAll();
	}
}

/**
 * @brief Task's initialization function.
 * @details First local queue is created. Then the task itself is created and the queue handle is passed as a task parameter.
 * @note Check app_config.h for stack configurations
 */
void HealthMonitor_Init(void) {
	xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
	xTaskCreateStatic(HealthMonitor_Handler,"HealthMonitor_Handler",NORMAL_TASK_STACK_SIZE
			,(void*)xTaskQueue,HM_PR,xTaskStack,&xTaskBuffer);
}
