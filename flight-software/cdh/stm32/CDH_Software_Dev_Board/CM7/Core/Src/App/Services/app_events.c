/*
 * app_events.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include "FreeRTOS.h"
#include "event_groups.h"
#include <App/Services/app_events.h>
#include <App/app_config.h>

static StaticEventGroup_t xSysSyncEventGroup;
static EventGroupHandle_t xSysSyncEvent;

void TaskSync_Init(void) {
	xSysSyncEvent = xEventGroupCreateStatic(&xSysSyncEventGroup);
}

void TaskSync_SetAndWait(EventBits_t taskBit) {
	xEventGroupSync(xSysSyncEvent,taskBit,ALL_TASKS_READY,portMAX_DELAY);
}

void TaskSync_WaitForAll(void) {
	xEventGroupWaitBits(xSysSyncEvent,ALL_TASKS_READY,pdTRUE,pdTRUE,portMAX_DELAY);
}

void TaskHealth_SetBit(EventBits_t bit){
	xEventGroupSetBits(xSysSyncEvent,bit);
}

void TaskHealth_ClearAll(void){
	xEventGroupClearBits(xSysSyncEvent,0xFF);
}

EventBits_t TaskHealth_Read(void){
	return xEventGroupGetBits(xSysSyncEvent);
}

EventWakeupReason_t TaskHealth_Check(void){
	if((int)xEventGroupWaitBits(xSysSyncEvent,ALL_TASKS_OK,pdTRUE,pdTRUE,1000) == ALL_TASKS_OK){
		return TASKS_OK;
	}
	else{
		return TIME_EXPIRED;
	}
}
