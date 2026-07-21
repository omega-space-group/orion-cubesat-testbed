/*
 * iwdg_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file iwdg_task.c
 * @brief Implementation of the internal watchdog handler.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "iwdg.h"
#include <App/app_config.h>

static StackType_t xIWDGTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xIWDGTaskBuffer;

/* IWDG Task responsible for kicking internal watchdog. IWDG currently set to trigger every 300ms */
//do we need this if we get an external watchdog?
/**
 * @brief Task's Handler where main logic is executed inside an infinite loop.
 * @details Task kicks the watchdog every 300ms.
 * @note Task runs every 300ms.
 * @warning IWDG will be replaced by an external watchdog IC on the custom OBC.
 */
static void IWDG_Task_Handler(void *argument){
	while(1){
		HAL_IWDG_Refresh(&hiwdg1);
		vTaskDelay(300);
	}
}

/**
 * @brief Task's initialization function.
 * @note Check app_config.h for stack configurations
 * @warning IWDG Task has no local queue since its not meant to interact with the application.
 */
void IWDG_Task_Init(void){
	  xTaskCreateStatic(IWDG_Task_Handler,"IWDG_Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,34,xIWDGTaskStack,&xIWDGTaskBuffer);
}
