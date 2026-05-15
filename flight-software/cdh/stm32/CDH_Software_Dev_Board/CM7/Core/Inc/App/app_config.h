/*
 * app_config.h
 *
 *  Created on: May 14, 2026
 *      Author: adaro
 */

#ifndef INC_APP_APP_CONFIG_H_
#define INC_APP_APP_CONFIG_H_

#include "FreeRTOS.h"
#include "queue.h"

#define NORMAL_TASK_STACK_SIZE 256
#define LARGE_TASK_STACK_SIZE  256*8

#define MASTER_QUEUE_LENGTH    10
#define LOCAL_QUEUE_LENGTH     5

#define ROOT_PR       35
#define DISPATCHER_PR 33
#define DUMMY_PR      32
#define TASKA_PR      31
#define TASKB_PR      30

#define DUMMY_BIT    (1 << 0)
#define TASKA_BIT    (1 << 1)
#define TASKB_BIT    (1 << 2)

//move to app_events (?)
#define ALL_TASKS_READY (DUMMY_BIT | TASKA_BIT | TASKB_BIT)

#define MAX_SUBS 4
#define MAX_SUBSCRIBERS_PER_TOPIC 5

typedef enum{
	SAFE      = 1,
	LOW_POWER = 2,
	NOMINAL   = 3,
	PAYLOAD   = 4
}State_t;

typedef enum{
	TOPIC_SYSTEM_STATE = 1,
	TOPIC_EXAMPLE1     = 2,
	TOPIC_EXAMPLE2     = 3,
	TOPIC_EXAMPLE3     = 4
}Topic_t;

/* Subscription Table Definition */
typedef struct {
    Topic_t Topic;
    uint8_t SubscriberCount;
    QueueHandle_t SubscriberQueues[MAX_SUBSCRIBERS_PER_TOPIC];
} Subscription_t;

typedef struct{
	Topic_t Topic;
	uint32_t Data;
} Message_t;

#define MSG_SIZE sizeof(Message_t)

/* Tasks - Topics Lookup Table */
typedef struct{
	const char* name;
	uint8_t length;
	Topic_t* topics;
} SubEntries_t;


#endif /* INC_APP_APP_CONFIG_H_ */
