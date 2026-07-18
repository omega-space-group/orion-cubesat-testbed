/*
 * app_config.h
 *
 *  Created on: May 14, 2026
 *      Author: adaro
 */
/**
 * @defgroup fdir_module Fault Detection, Isolation, and Recovery (FDIR)
 * @brief High-level flight safety, health monitoring, and watchdog tasks.
 */

/**
 * @defgroup telemetry_module Housekeeping & Telemetry
 * @brief Serializing, buffering, and transmitting space-to-ground health matrices.
 */

/**
 * @defgroup telecommand_module Ground Command Execution
 * @brief Handling of Telecommands
 */

/**
 * @defgroup sys_state_module Satellite Modes
 * @brief Run the System State FSM
 */

/**
 * @defgroup subsystems_module Subsystem Control
 * @brief Something
 */

/**
 * @file app_config.h
 * @brief Configuration file for current application.
 */
#ifndef INC_APP_APP_CONFIG_H_
#define INC_APP_APP_CONFIG_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "fdcan.h"

/**
 * @brief Normal task stack size
 */
#define NORMAL_TASK_STACK_SIZE 256*10

/**
 * @brief Large task stack size
 */
#define LARGE_TASK_STACK_SIZE  256*10

/**
 * @brief Master queue length
 */
#define MASTER_QUEUE_LENGTH    10

/**
 * @brief Local queue length
 */
#define LOCAL_QUEUE_LENGTH     5

/**
 * @name Task priorities
 * @details RTOS priorities go from high to low. Meaning highest priority task will execute before a lower priority task.
 *
 * This application uses preemption with time slicing, meaning a Round-Robin scheduling policy.
 * @note A new task's priority should be added here!
 * @warning PR = 24 is being used by the auto-generated default task in freertos.c which blocks indefinitely.
 * @{
 */
#define ROOT_PR        35 /**< &nbsp; */
#define DISPATCHER_PR  33 /**< &nbsp; */
#define MODE_PR        32 /**< &nbsp; */
#define CANRX_PR       31 /**< &nbsp; */
#define DUMMY_PR       29 /**< &nbsp; */
#define HM_PR          27 /**< &nbsp; */
#define SUBM_PR        26 /**< &nbsp; */
#define SUBC_PR        25 /**< &nbsp; */
#define TC_PR          24 /**< &nbsp; */
#define HK_PR          23 /**< &nbsp; */
/** @} */

/**
 * @name Task Health and Sync Bits
 * @note A new task's bit should be added at the end!
 * @{
 */
#define DISPATCHER_BIT (0 << 0) /**< &nbsp; */
#define DUMMY_BIT      (1 << 1) /**< &nbsp; */
#define MODE_BIT       (1 << 2) /**< &nbsp; */
#define HM_BIT         (1 << 3) /**< &nbsp; */
#define SUBM_BIT       (1 << 4) /**< &nbsp; */
#define SUBC_BIT       (1 << 5) /**< &nbsp; */
#define TC_BIT 		   (1 << 6) /**< &nbsp; */
#define HK_BIT 		   (1 << 7) /**< &nbsp; */
/** @} */

//move to app_events (?)
/**
 * @brief This define is used to group all periodic tasks together. More info in healthMonitor_task.c
 * @note Any new initialized PERIODIC tasks should be added!
 */
#define PERIODIC_TASKS (DUMMY_BIT | HM_BIT | SUBM_BIT | HK_BIT)

/**
 * @brief This define is used for sync purposes. More info in dispatcher_task.c
 * @note Any new initialized tasks should be added!
 */
#define ALL_TASKS_READY (DUMMY_BIT | MODE_BIT | HM_BIT | SUBM_BIT | SUBC_BIT | TC_BIT | HK_BIT)

/**
 * @brief This define is used for health monitoring purposes in healthMonitor_task.c
 * @note Any new initialized tasks should be added!
 */
#define ALL_TASKS_OK (DISPATCHER_BIT | DUMMY_BIT | MODE_BIT | HM_BIT | SUBM_BIT | SUBC_BIT | TC_BIT | HK_BIT)

/**
 * @brief Maximum number of subscribers (i.e. tasks that have subscribed to at least one topic) allowed.
 */
#define MAX_SUBS 10

/**
 * @brief Maximum number of subscribers per topic allowed.
 */
#define MAX_SUBSCRIBERS_PER_TOPIC 10

/**
 * @brief All available system states. More info in modeManager.c
 * @note Any extra states should be added here!
 */
typedef enum{
	SAFE      = 1,/**< Safe Mode */
	LOW_POWER = 2,/**< Low Power Mode */
	NOMINAL   = 3,/**< Nominal Mode */
	PAYLOAD   = 4 /**< Payload Mode */
}State_t;

/**
 * @brief All available TOPICS.
 * @note Any extra topics should be added here!
 */
typedef enum{
	SYSTEM_STATE        = 1,/**< Current System State/Mode */
	CHANGE_SYSTEM_STATE = 2,/**< System State Change Request */
	TELECOMMAND,            /**< Telecommand Received */
	EPS_MSG,                /**< EPS Message */
	EPS_TM,                 /**< EPS Telemetry */
	COMMS_MSG,              /**< COMMS Message */
	COMMS_TM,               /**< COMMS Telemetry */
	ADCS_MSG,               /**< ADCS Message */
	ADCS_TM,                /**< ADCS Telemetry */
	PL_MSG,                 /**< Payload Message */
	PL_TM,                  /**< Payload Telemetry */
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
