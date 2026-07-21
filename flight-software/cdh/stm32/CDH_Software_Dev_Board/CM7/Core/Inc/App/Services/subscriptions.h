/*
 * subscriptions.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file subscriptions.h
 * @brief This file contains all the function prototypes for the subscriptions.c file.
 * @details Implementation of the Subscriber/Publisher logic.
 */
#ifndef INC_APP_SERVICES_SUBSCRIPTIONS_H_
#define INC_APP_SERVICES_SUBSCRIPTIONS_H_

#include "FreeRTOS.h"
#include "queue.h"
#include <App/app_config.h>

/**
 * @brief Wake up reason used in SleepUntil()
 */
typedef enum{
	NEW_MSG,/**< New Message Received */
	TIMEOUT /**< Set Time Expired */
}WakeupReason_t;

void Subscribe(const char* name,QueueHandle_t queue);
void Publish(Message_t newMsg);
WakeupReason_t SleepUntil(QueueHandle_t queue, Message_t *newMsgRsc, TickType_t xTicksToWait);
void DispatcherSend(Message_t newMsg);

#endif /* INC_APP_SERVICES_SUBSCRIPTIONS_H_ */
