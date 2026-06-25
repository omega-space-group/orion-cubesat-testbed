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

static StaticEventGroup_t xTaskHealthEventGroup;
static EventGroupHandle_t xTaskHealthEvent;

void TaskSync_Init(void) {
	xSysSyncEvent = xEventGroupCreateStatic(&xSysSyncEventGroup);
}

void TaskSync_SetAndWait(EventBits_t taskBit) {
	xEventGroupSync(xSysSyncEvent,taskBit,ALL_TASKS_READY,portMAX_DELAY);
}

void TaskSync_WaitForAll(void) {
	xEventGroupWaitBits(xSysSyncEvent,ALL_TASKS_READY,pdTRUE,pdTRUE,portMAX_DELAY);
}

void TaskHealth_Init(void) {
	xTaskHealthEvent = xEventGroupCreateStatic(&xTaskHealthEventGroup);
}

void TaskHealth_SetBit(EventBits_t bit){
	xEventGroupSetBits(xTaskHealthEvent,bit);
}

void TaskHealth_ClearBit(EventBits_t bit){
	xEventGroupClearBits(xTaskHealthEvent,bit);
}

void TaskHealth_ClearAll(void){
	xEventGroupClearBits(xTaskHealthEvent,ALL_TASKS_READY);
}

EventBits_t TaskHealth_Read(void){
	return xEventGroupGetBits(xTaskHealthEvent);
}
