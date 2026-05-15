/*
 * subscriptions.c
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
#include "FreeRTOS.h"
#include "queue.h"
#include <App/app_config.h>
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>

static Subscription_t sub_table[MAX_SUBS];
static uint8_t sub_count = 0;

/* Subscription Tables ---------------------------------------------------------*/
static Topic_t DummyTaskGroup[] = {TOPIC_SYSTEM_STATE,TOPIC_EXAMPLE1};
static Topic_t TaskAGroup[]     = {TOPIC_SYSTEM_STATE,TOPIC_EXAMPLE1,TOPIC_EXAMPLE2};
static Topic_t TaskBGroup[]     = {TOPIC_SYSTEM_STATE,TOPIC_EXAMPLE2};

static SubEntries_t lookupTable[] = {
		{"Dummy_Task_Handler" , 2,DummyTaskGroup},
		{"TaskA_Handler"      , 3,TaskAGroup},
		{"TaskB_Handler"      , 2,TaskBGroup}
};

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
							printf("%s subscribed to %d successfully!\r\n",
									name, (int)lookupTable[i].topics[j]);
							fflush(stdout);
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
					printf("%s subscribed to %d successfully!\r\n",
							name, (int)lookupTable[i].topics[j]);
					fflush(stdout);
					continue;
				}
				else if(sub_count > MAX_SUBS && !found){
					printf("%s failed to subscribe to %d. Sub table full!\r\n",
							name, (int)lookupTable[i].topics[j]);
					fflush(stdout);
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

void Publish(Message_t newMsg){
	for(int i = 0; i < sub_count; i++){
		if(newMsg.Topic == sub_table[i].Topic){
			for(int j = 0; j < sub_table[i].SubscriberCount; j++){
				BaseType_t xStatus = xQueueSendToBack(sub_table[i].SubscriberQueues[j],&newMsg,50);
				if(xStatus != pdPASS){
					printf("%p queue is full!\r\n",(void*)sub_table[i].SubscriberQueues[j]);
					fflush(stdout);
				}
			}
		}
	}
}

