/*
 * subscriptions.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file subscriptions.c
 * @brief Implementation of the Subscriber/Publisher logic.
 */
#include "FreeRTOS.h"
#include "queue.h"
#include <App/app_config.h>
#include <App/Services/subscriptions.h>
#include <App/Tasks/dispatcher_task.h>

#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>

static Subscription_t sub_table[MAX_SUBS];
static uint8_t sub_count = 0;

/**
 * @anchor sub_table
 * @name Subscription Mapping Table
 * @brief Current routing matrix for task topic groups.
 *
 * | Task Group / Subscriber | Subscribed Topics |
 * | :--- | :--- |
 * | `DummyTaskGroup` | • `SYSTEM_STATE` |
 * | `ModeManagerGroup` | • `CHANGE_SYSTEM_STATE` |
 * | `SubsystemControlGroup` | • `EPS_MSG`<br>• `COMMS_MSG`<br>• `ADCS_MSG`<br>• `PL_MSG` |
 * | `TelecommandHandlerGroup` | • `TELECOMMAND` |
 * | `HousekeepingGroup` | • `EPS_TM`<br>• `COMMS_TM`<br>• `ADCS_TM`<br>• `PL_TM` |
 *
 */
/* Subscription Tables ---------------------------------------------------------*/
static Topic_t DummyTaskGroup[]          = {SYSTEM_STATE};
static Topic_t ModeManagerGroup[]        = {CHANGE_SYSTEM_STATE};
static Topic_t SubsystemControlGroup[]   = {EPS_MSG,COMMS_MSG,ADCS_MSG,PL_MSG};
static Topic_t TelecommandHandlerGroup[] = {TELECOMMAND};
static Topic_t HousekeepingGroup[]       = {EPS_TM,COMMS_TM,ADCS_TM,PL_TM};

static SubEntries_t lookupTable[] = {
		{"Dummy_Task_Handler"       , 1, DummyTaskGroup},
		{"ModeManager_Handler"      , 1, ModeManagerGroup},
		{"SubsystemControl_Handler" , 4, SubsystemControlGroup},
		{"Telecommand_Handler"      , 1, TelecommandHandlerGroup},
		{"Housekeeping_Handler"     , 4, HousekeepingGroup}
};

/**
 * @brief Subscribes a task to predefined topics
 * @details Task with handle name 'name' and local queue handle 'queue' subscribes to topics defined in the subscription table
 * @param name  The task handler name as a string
 * @param queue  The task's local queue handle (passed via the task's argument)
 */
void Subscribe(const char* name,QueueHandle_t queue){
	/*
	 * 1. Check sub table to see if anyone has already subscribed to this topic
	 * 2. If so, add this queue to topic's list of queues
	 * 3. If not, add topic and queue to sub table
	 * */
	int lookupTableLength = sizeof(lookupTable)/sizeof(lookupTable[0]);
	int found = 0;

	for(int i = 0; i < lookupTableLength; i++){
		if (strcmp(name, lookupTable[i].name) == 0){
			for(int j = 0; j < lookupTable[i].length; j++){
				for(int k = 0; k < sub_count; k++){
					if(sub_table[k].Topic == lookupTable[i].topics[j] ){
						found = 1;
						if(sub_table[k].SubscriberCount < MAX_SUBSCRIBERS_PER_TOPIC){
							sub_table[k].SubscriberQueues[sub_table[k].SubscriberCount++] = queue;
//							printf("%s subscribed to %d successfully!\r\n",
//									name, (int)lookupTable[i].topics[j]);
//							fflush(stdout);
							break;
						}
						printf("%s failed to subscribe to %d. Topic's subscriber queue full!\r\n",
								name, (int)lookupTable[i].topics[j]);
						fflush(stdout);
						break;
					}
				}
				if(sub_count < MAX_SUBS && !found){
					Subscription_t newSub;
					newSub.SubscriberCount = 0;
					newSub.SubscriberQueues[newSub.SubscriberCount++] = queue;
					newSub.Topic = lookupTable[i].topics[j];

					sub_table[sub_count++] = newSub;
//					printf("%s subscribed to %d successfully!\r\n",
//							name, (int)lookupTable[i].topics[j]);
//					fflush(stdout);
					continue;
				}
				else if(sub_count > MAX_SUBS && !found){
//					printf("%s failed to subscribe to %d. Sub table full!\r\n",
//							name, (int)lookupTable[i].topics[j]);
//					fflush(stdout);
				}
				else{
					found = 0;
				}
				continue;
			}
			break;
		}
	}
	return;
}

/**
 * @brief Publishes a message via the software bus
 * @details Message 'newMsg' is being pushed to dispatcher master queue which effectively publishes the message
 * to anyone interested
 * @param newMsg  The message you want to publish
 */
void Publish(Message_t newMsg){
	xQueueSendToBack(DispatcherTask_GetQueue(),&newMsg,200);
}

/**
 * @brief Task blocks (sleeps) until a new message is received in its local queue
 * @param queue  The task's own local queue
 * @param newMsgRsc  The structure where the incoming message will be stored
 * @param xTicksToWait   How long the task can stay blocked waiting for a message
 * @return NEW_MSG : if new msg is received, TIMEOUT : if time (xTicksToWait) expired
 */
WakeupReason_t SleepUntil(QueueHandle_t queue, Message_t *newMsgRsc, TickType_t xTicksToWait){
	if (xQueueReceive(queue,newMsgRsc,xTicksToWait) == pdPASS) {
//		printf("%p: Got a new message\r\n",(void*)queue);
//		fflush(stdout);
	    return NEW_MSG;
	}
	return TIMEOUT;
}

void DispatcherSend(Message_t newMsg){
	for(int i = 0; i < sub_count; i++){
		if(newMsg.Topic == sub_table[i].Topic){
			for(int j = 0; j < sub_table[i].SubscriberCount; j++){
				BaseType_t xStatus = xQueueSendToBack(sub_table[i].SubscriberQueues[j],&newMsg,50);
				if(xStatus != pdPASS){
//					printf("%p queue is full!\r\n",(void*)sub_table[i].SubscriberQueues[j]);
//					fflush(stdout);
				}
			}
		}
	}
}

