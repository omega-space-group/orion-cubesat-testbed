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
#include "fdcan.h"

#define NORMAL_TASK_STACK_SIZE 256*10
#define LARGE_TASK_STACK_SIZE  256*10

#define MASTER_QUEUE_LENGTH    10
#define LOCAL_QUEUE_LENGTH     5

#define ROOT_PR       35
#define DISPATCHER_PR 33
#define MODE_PR       32
#define CANRX_PR      31
#define DUMMY_PR      29
#define HM_PR         27
#define SUBM_PR       26


//Keep in mind PR = 24 is being used by the auto-generated default task in freertos.c

#define DISPATCHER_BIT (0 << 0)
#define DUMMY_BIT      (1 << 1)
#define MODE_BIT       (1 << 2)
#define HM_BIT         (1 << 3)
#define SUBM_BIT       (1 << 4)

//move to app_events (?)
#define ALL_TASKS_READY (DUMMY_BIT | MODE_BIT | HM_BIT | SUBM_BIT)
#define ALL_TASKS_OK (DISPATCHER_BIT | DUMMY_BIT | MODE_BIT | HM_BIT | SUBM_BIT)

#define MAX_SUBS 10
#define MAX_SUBSCRIBERS_PER_TOPIC 10

typedef enum{
	SAFE      = 1,
	LOW_POWER = 2,
	NOMINAL   = 3,
	PAYLOAD   = 4
}State_t;

typedef enum{
	SYSTEM_STATE        = 1,
	CHANGE_SYSTEM_STATE = 2,
	SUBSYSTEM_STATUS    = 3,
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
		CAN_RxPacket canPacket;
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
