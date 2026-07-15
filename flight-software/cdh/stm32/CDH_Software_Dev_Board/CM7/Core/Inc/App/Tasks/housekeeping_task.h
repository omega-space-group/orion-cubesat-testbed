/*
 * housekeeping_task.h
 *
 *  Created on: Jul 10, 2026
 *      Author: adaro
 */
/**
 * @file housekeeping_task.h
 * @brief This file contains all the function prototypes for the housekeeping_task.c file
 * @details This module manages the data request and acquisition from itself and the subsystems
 * and also the logging and downlink when possible.
 * @ingroup telemetry_module
 */
#ifndef INC_APP_TASKS_HOUSEKEEPING_TASK_H_
#define INC_APP_TASKS_HOUSEKEEPING_TASK_H_

void HousekeepingHandler_Init(void);

#endif /* INC_APP_TASKS_HOUSEKEEPING_TASK_H_ */
