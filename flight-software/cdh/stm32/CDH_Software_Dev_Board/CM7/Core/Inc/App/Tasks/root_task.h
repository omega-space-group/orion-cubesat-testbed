/*
 * root_task.h
 *
 *  Created on: May 15, 2026
 *      Author: adaro
 */
/**
 * @file root_task.h
 * @brief This file contains all the function prototypes for the root_task.c file.
 * @details Implementation of the Root Task responsible for starting all peripherals and spawn (initialize) every task.
 * For more details look into Root_Task_Handler().
 */
#ifndef INC_APP_TASKS_ROOT_TASK_H_
#define INC_APP_TASKS_ROOT_TASK_H_

void Root_Task_Init(void);

#endif /* INC_APP_TASKS_ROOT_TASK_H_ */
