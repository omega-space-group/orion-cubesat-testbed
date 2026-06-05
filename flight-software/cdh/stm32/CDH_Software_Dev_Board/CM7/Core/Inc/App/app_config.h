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

#define NORMAL_TASK_STACK_SIZE 256*8
#define LARGE_TASK_STACK_SIZE  256*8

#define MASTER_QUEUE_LENGTH    10
#define LOCAL_QUEUE_LENGTH     5

#define ROOT_PR       35
#define DISPATCHER_PR 33
#define MODE_PR       32
#define CANRX_PR      31
#define HM_PR         30
#define DUMMY_PR      29
#define TASKA_PR      28
#define TASKB_PR      27
//Keep in mind PR = 24 is being used by the auto-generated default task

#define DUMMY_BIT (1 << 0)
#define TASKA_BIT (1 << 1)
#define TASKB_BIT (1 << 2)
#define MODE_BIT  (1 << 3)
#define HM_BIT    (1 << 4)
#define DISPATCHER_BIT (1 << 5)

//move to app_events (?)
#define ALL_TASKS_READY (DUMMY_BIT | TASKA_BIT | TASKB_BIT | MODE_BIT | HM_BIT)
#define ALL_TASKS_OK (ALL_TASKS_READY | DISPATCHER_BIT)

#define MAX_SUBS 4
#define MAX_SUBSCRIBERS_PER_TOPIC 5

typedef enum{
	SAFE      = 1,
	LOW_POWER = 2,
	NOMINAL   = 3,
	PAYLOAD   = 4
}State_t;

typedef enum{
	SYSTEM_STATE        = 1,
	EXAMPLE1            = 2,
	EXAMPLE2            = 3,
	EXAMPLE3            = 4,
	CHANGE_SYSTEM_STATE = 5
}Topic_t;

/* Subscription Table Definition */
typedef struct {
    Topic_t Topic;
    uint8_t SubscriberCount;
    QueueHandle_t SubscriberQueues[MAX_SUBSCRIBERS_PER_TOPIC];
} Subscription_t;

typedef struct{
	Topic_t Topic;
	union{
		State_t mode;
		uint32_t rawData;
	}Data;
} Message_t;

#define MSG_SIZE sizeof(Message_t)

/* Tasks - Topics Lookup Table */
typedef struct{
	const char* name;
	uint8_t length;
	Topic_t* topics;
} SubEntries_t;


#endif /* INC_APP_APP_CONFIG_H_ */
