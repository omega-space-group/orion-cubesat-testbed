
 @page task_creation Task Creation and Initialization
 
 
 Every new task is defined inside a pair of taskName_task.c/taskName_task.h files inside the Src/Core/App/Tasks and Inc/Core/App/Tasks directories respectively.

 ## Source File Template:
 
 @code{.c}
 #include "FreeRTOS.h"
 #include "task.h"
  
 #include <App/app_config.h>
 #include <App/Services/subscriptions.h>
 #include <App/Services/app_events.h>
 #include <App/Tasks/taskName_task.h>
  
 static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
 static StaticTask_t xTaskBuffer;
  
 static StaticQueue_t xTaskQueueData;
 static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
 static QueueHandle_t xTaskQueue;
 @endcode
 
 ### -> Handler Template for periodic tasks:
 @code{.c}
 static void TaskName_Handler(void *argument){
    TaskSync_SetAndWait(TASK_BIT);
    while(1){
        vTaskDelay(period_in_ms);
 
        //Task logic goes here...
 
        TaskHealth_SetBit(TASK_BIT);
    }
 }
 @endcode
 
 ### -> Handler Template for non-periodic tasks:
 @code{.c}
 static void TaskName_Handler(void *argument){
    Subscribe("TaskName_Handler",(QueueHandle_t)argument);
    TaskSync_SetAndWait(TASK_BIT);
    Message_t newMsgRsc;
    while(1){
        TaskHealth_SetBit(TASK_BIT);
        SleepUntil((QueueHandle_t)argument,&newMsgRsc,portMAX_DELAY);
        TaskHealth_ClearBit(TASK_BIT);
 
        //Task logic goes here...
 
        TaskHealth_SetBit(TASK_BIT);
    }
 
@endcode
@note New task subscriptions need to be manually added in the Subscription Table in subscriptions.c before calling Subscribe()

### -> Task initialization without a local queue:
@code{.c}
void TaskName_Init(void) {
    xTaskCreateStatic(TaskName_Handler,"TaskName_Handler",NORMAL_TASK_STACK_SIZE,NULL,TASK_PR,xTaskStack,&xTaskBuffer);
}
@endcode

### -> Task initialization with a local queue:
@code{.c}
#include "queue.h"
 
static StaticQueue_t xTaskQueueData;
static uint8_t ucTaskQueueStorageArea[LOCAL_QUEUE_LENGTH * MSG_SIZE];
static QueueHandle_t xTaskQueue;
 
void TaskNameHandler_Init(void) {
    xTaskQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucTaskQueueStorageArea, &xTaskQueueData);
    xTaskCreateStatic(TaskNameHandler_Init,"TaskNameHandler_Init",NORMAL_TASK_STACK_SIZE
           ,(void*)xTaskQueue,TASK_PR,xTaskStack,&xTaskBuffer);
}
@endcode

### -> Optional function to get global access to local queue:
@code{.c}
static QueueHandle_t xLocal_Queue;
 
QueueHandle_t TaskName_GetQueue(void) {
    return xLocal_Queue;
}
@endcode

@note TASK_PR, TASK_BIT should be defined inside app_config.h

## Header File Template:

@code{.c}
#include "FreeRTOS.h"
#include "queue.h"
 
void TaskName_Init(void);
QueueHandle_t TaskName_GetQueue(void); //If used
@endcode

@note Finally to get the task scheduled for execution you need to call TaskNameHandler_Init(void) inside the Root_Task_Handler(void *argument) before the infite loop.

## Header File Template:
