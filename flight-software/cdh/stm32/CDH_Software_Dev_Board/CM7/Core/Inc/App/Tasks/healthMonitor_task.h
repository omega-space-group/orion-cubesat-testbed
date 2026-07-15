/*
 * healthMonitor_task.h
 *
 *  Created on: May 18, 2026
 *      Author: adaro
 */
/**
 * @file healthMonitor_task.h
 * @brief Public interface for the OBC Health Monitoring System.
 * @details This module manages the background health reporting, watchdog tracking,
 * and error accounting for all active flight tasks.
 * @ingroup fdir_module
 */

#ifndef INC_APP_TASKS_HEALTHMONITOR_TASK_H_
#define INC_APP_TASKS_HEALTHMONITOR_TASK_H_

void HealthMonitor_Init(void);

#endif /* INC_APP_TASKS_HEALTHMONITOR_TASK_H_ */
