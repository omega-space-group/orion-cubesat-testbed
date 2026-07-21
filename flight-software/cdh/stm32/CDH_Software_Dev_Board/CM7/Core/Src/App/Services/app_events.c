/*
 * app_events.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file app_events.c
 * @brief Implementation of all application events.
 */
#include "FreeRTOS.h"
#include "event_groups.h"
#include <App/Services/app_events.h>
#include <App/app_config.h>

static StaticEventGroup_t xSysSyncEventGroup;
static EventGroupHandle_t xSysSyncEvent;

static StaticEventGroup_t xTaskHealthEventGroup;
static EventGroupHandle_t xTaskHealthEvent;

/**
 * @brief Initialization of task synchronization event group
 */
void TaskSync_Init(void) {
	xSysSyncEvent = xEventGroupCreateStatic(&xSysSyncEventGroup);
}

/**
 * @brief Acts as a Thread Sync Barrier
 * @details Task sets its own bit to 1 in the event mask and blocks until every other task has done so too to continue.
 * @param taskBit Task bit defined in app_config.h
 */
void TaskSync_SetAndWait(EventBits_t taskBit) {
	xEventGroupSync(xSysSyncEvent,taskBit,ALL_TASKS_READY,portMAX_DELAY);
}

/**
 * @brief Acts as a Thread Sync Barrier
 * @details Block until event mask becomes 1, which means all tasks are synced, and then continue.
 * @note At the moment, this is only used by the dispatcher task thats needs to have the overview.
 */
void TaskSync_WaitForAll(void) {
	xEventGroupWaitBits(xSysSyncEvent,ALL_TASKS_READY,pdTRUE,pdTRUE,portMAX_DELAY);
}

/**
 * @brief Initializes task health check event group
 */
void TaskHealth_Init(void) {
	xTaskHealthEvent = xEventGroupCreateStatic(&xTaskHealthEventGroup);
}

/**
 * @brief Sets task bit to health check event group
 * @param bit Task bit defined in app_config.h
 */
void TaskHealth_SetBit(EventBits_t bit){
	xEventGroupSetBits(xTaskHealthEvent,bit);
}

/**
 * @brief Clears task bit to health check event group
 * @param bit Task bit defined in app_config.h
 */
void TaskHealth_ClearBit(EventBits_t bit){
	xEventGroupClearBits(xTaskHealthEvent,bit);
}

/**
 * @brief Clears health check event mask
 */
void TaskHealth_ClearAll(void){
	xEventGroupClearBits(xTaskHealthEvent,PERIODIC_TASKS);
}

/**
 * @brief Returns the value of the health check event mask
 * @return EventBits_t
 */
EventBits_t TaskHealth_Read(void){
	return xEventGroupGetBits(xTaskHealthEvent);
}
