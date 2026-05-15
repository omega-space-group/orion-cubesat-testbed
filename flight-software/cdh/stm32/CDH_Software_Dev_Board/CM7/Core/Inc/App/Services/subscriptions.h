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

void Subscribe(const char* name,QueueHandle_t queue);
void Publish(Message_t newMsg);

#endif /* INC_APP_SERVICES_SUBSCRIPTIONS_H_ */
