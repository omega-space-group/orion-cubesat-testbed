/*
 * root_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include <App/Tasks/root_task.h>
#include "FreeRTOS.h"
#include "task.h"
#include "fdcan.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

#include <App/app_config.h>
#include <App/Services/app_events.h>
#include <App/Tasks/dummy_task.h>
#include <App/Tasks/taskA_task.h>
#include <App/Tasks/taskB_task.h>
#include <App/Tasks/dispatcher_task.h>

static StackType_t xRootTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xRootTaskBuffer;

/* Root Task responsible for starting peripherals and spawn tasks */
void Root_Task_Handler(void *argument){
	printf("\r\n--- SYSTEM BOOT START ---\r\n");
	fflush(stdout);
	FDCAN_Start();

	TaskSync_Init();

	Dispatcher_Task_Init();
	DummyTask_Init();
	TaskA_Task_Init();
	TaskB_Init();
	/* Infinite loop */
	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

void Root_Task_Init(void) {
	  xTaskCreateStatic(Root_Task_Handler,"Root_Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,ROOT_PR,xRootTaskStack,&xRootTaskBuffer);
}
