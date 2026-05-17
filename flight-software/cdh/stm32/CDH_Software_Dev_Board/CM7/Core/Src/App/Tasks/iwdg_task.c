/*
 * iwdg_task.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include "FreeRTOS.h"
#include "task.h"
#include "iwdg.h"
#include <App/app_config.h>

static StackType_t xIWDGTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xIWDGTaskBuffer;

/* IWDG Task responsible for kicking internal watchdog. IWDG currently set to trigger every 500ms */
//do we need this if we get an external watchdog?
static void IWDG_Task_Handler(void *argument){
	while(1){
		HAL_IWDG_Refresh(&hiwdg1);
		vTaskDelay(300);
	}
}

void IWDG_Task_Init(void){
	  xTaskCreateStatic(IWDG_Task_Handler,"IWDG_Task_Handler",NORMAL_TASK_STACK_SIZE,NULL,34,xIWDGTaskStack,&xIWDGTaskBuffer);
}
