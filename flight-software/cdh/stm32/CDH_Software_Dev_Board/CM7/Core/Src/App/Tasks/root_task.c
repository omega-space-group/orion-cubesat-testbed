/*
 * root_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file root_task.c
 * @brief Implementation of the Root Task responsible for starting all peripherals and spawn (initialize) every task.
 */
#include <App/Tasks/root_task.h>
#include "FreeRTOS.h"
#include "task.h"
#include "fdcan.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

#include <App/app_config.h>
#include <App/Services/app_events.h>
#include <App/Tasks/iwdg_task.h>
#include <App/Tasks/dummy_task.h>
#include <App/Tasks/dispatcher_task.h>
#include <App/Tasks/modeManager_task.h>
#include <App/Tasks/healthMonitor_task.h>
#include <App/Tasks/subsystemMonitoring_task.h>
#include <App/Tasks/subsystemControl_task.h>
#include <App/Tasks/can_rx_task.h>
#include <App/Tasks/telecommandHandler_task.h>
#include <App/Tasks/housekeeping_task.h>

static StackType_t xRootTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xRootTaskBuffer;

/* Root Task responsible for starting peripherals and spawn tasks */
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details
 * 1. All peripherals are started.
 * 2. Task sync event is initialized.
 * 3. Task health event is initialized.
 * 4. Each task is initialized.
 * 5. Task blocks forever.
 * @warning To create a task.c/.h file with an init() and a handler() function does NOT mean the task will execute. Only tasks that
 * are initialized in the Root Task are scheduled by the scheduler for execution.
 */
static void Root_Task_Handler(void *argument){
//	printf("\r\n--- SYSTEM BOOT START ---\r\n");
//	fflush(stdout);
	FDCAN_Start();

	TaskSync_Init();
	TaskHealth_Init();

	IWDG_Task_Init();
	Dispatcher_Task_Init();
	DummyTask_Init();
	ModeManager_Task_Init();
	HealthMonitor_Init();
	SubsystemMonitor_Init();
	SubsystemControl_Init();
	CAN_RX_Task_Init();
	TelecommandHandler_Init();
	HousekeepingHandler_Init();
	/* Infinite loop */
	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

/**
 * @brief Task's initialization function.
 * @note Check app_config.h for stack configurations
 * @warning Root Task has no local queue since its only meant to execute once and block forever.
 */
void Root_Task_Init(void) {
	  xTaskCreateStatic(Root_Task_Handler,"Root_Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,ROOT_PR,xRootTaskStack,&xRootTaskBuffer);
}
