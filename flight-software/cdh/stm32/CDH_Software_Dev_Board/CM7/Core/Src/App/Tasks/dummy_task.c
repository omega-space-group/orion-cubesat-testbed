/*
 * dummy_tasks.c
 *
 *  Created on: May 14, 2026
 *      Author: adaro
 */

#include <App/Tasks/dummy_task.h>
#include "FreeRTOS.h"
#include "queue.h"

#include <App/app_config.h>

static StackType_t xDummyTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xDummyTaskBuffer;

static StaticQueue_t xDummyTaskQueueData;
static uint8_t ucDummyTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xDummyTaskQueue;

static void DummyTask_Entry(void *argument) {
    while(1) {
        vTaskDelay(1000);
    }
}

void DummyTask_Init(void) {
	xDummyTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucDummyTaskQueueStorageArea, &xDummyTaskQueueData);
    xTaskCreateStatic(DummyTask_Entry,"DummyTask_Entry",256,(void*)xDummyTaskQueue, 32,xDummyTaskStack,     &xDummyTaskBuffer);
}

QueueHandle_t DummyTask_GetQueue(void) {
    return xDummyTaskQueue;
}

