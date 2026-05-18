/*
 * subscriptions.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */

#ifndef INC_APP_SERVICES_SUBSCRIPTIONS_H_
#define INC_APP_SERVICES_SUBSCRIPTIONS_H_

#include "FreeRTOS.h"
#include "queue.h"
#include <App/app_config.h>

typedef enum{
	NEW_MSG,
	TIMEOUT
}WakeupReason_t;

void Subscribe(const char* name,QueueHandle_t queue);
void Publish(Message_t newMsg);
WakeupReason_t SleepUntil(QueueHandle_t queue, Message_t *newMsgRsc, TickType_t xTicksToWait);
void DispatcherSend(Message_t newMsg);

#endif /* INC_APP_SERVICES_SUBSCRIPTIONS_H_ */
