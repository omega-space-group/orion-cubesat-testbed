/*
 * app_events.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */

#ifndef INC_APP_SERVICES_APP_EVENTS_H_
#define INC_APP_SERVICES_APP_EVENTS_H_

#include "FreeRTOS.h"
#include "event_groups.h"

void TaskSync_Init(void);
void TaskSync_SetAndWait(EventBits_t taskBit);
void TaskSync_WaitForAll(void);

#endif /* INC_APP_SERVICES_APP_EVENTS_H_ */
