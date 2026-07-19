/*
 * app_events.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file app_events.h
 * @brief This file contains all the function prototypes for the app_events.c file.
 * @details Implementation of all application events.
 */
#ifndef INC_APP_SERVICES_APP_EVENTS_H_
#define INC_APP_SERVICES_APP_EVENTS_H_

#include "FreeRTOS.h"
#include "event_groups.h"

typedef enum{
	TASKS_OK,
	TIME_EXPIRED
}EventWakeupReason_t;

void TaskSync_Init(void);
void TaskSync_SetAndWait(EventBits_t taskBit);
void TaskSync_WaitForAll(void);
void TaskHealth_Init(void);
void TaskHealth_SetBit(EventBits_t bit);
void TaskHealth_ClearBit(EventBits_t bit);
void TaskHealth_ClearAll(void);
EventBits_t TaskHealth_Read(void);

#endif /* INC_APP_SERVICES_APP_EVENTS_H_ */
