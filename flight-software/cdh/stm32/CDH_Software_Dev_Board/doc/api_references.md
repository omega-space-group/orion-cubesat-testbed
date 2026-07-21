@page api_references Available API Reference

### 1. Subscribe
@code{.c}
void Subscribe(const char* name, QueueHandle_t queue);
@endcode

#### Discription: 
Subscribes a task to topics based on the Subscription Table (@ref sub_table)

#### Notes:
* Calling this function does not wake up a task. It is just a normal function call inside the task.
* You should call this during the setup phase of the task, outside of the infinite loop, so it runs only once. 
* After subscribing, all tasks must call TaskSync_SetAndWait() to sync up before moving forward.

For more details, check out the @ref task_creation "Task Creation and Initialization Manual".

@note Missing return values to show if execution succeeded.

---

### 2. Publish
@code{.c}
void Publish(Message_t newMsg);
@endcode

#### Discription:
Publishes a message via the software bus

#### Notes:
* Calling this does not wake up a task directly and acts like a normal function call. However, calling this will wake up the dispatcher task right away. This triggers a context switch, meaning the current task stops running and the dispatcher takes over (because in the original firmware, the dispatcher task has the highest priority).

---

### 3. Sleep
@code{.c}
WakeupReason_t SleepUntil(QueueHandle_t queue, Message_t *newMsgRsc, TickType_t xTicksToWait);
@endcode

#### Discription:
Task blocks (sleeps) until a new message is received in its local queue

#### Notes:  
* If you set a timeout value, the task wakes up either when a message arrives or when the time runs out. 
* To make the task block and wait forever without timing out, use portMAX_DELAY. 

For more information on how this works under the hood, look up xQueueReceive() in the FreeRTOS_Reference_Manual_V10.0.0.pdf.

---

### 4. Task Sync
@code{.c}
void TaskSync_Init(void);
@endcode

#### Discription:
Initialization of task synchronization event group

@code{.c}
void TaskSync_SetAndWait(EventBits_t taskBit);
@endcode

#### Discription:
Acts as a Thread Sync Barrier

#### Notes: 
* Task sets its own bit to 1 in the event mask and blocks until every other task has done so too to continue.

@code{.c}
void TaskSync_WaitForAll(void);
@endcode

#### Discription:
Acts as a Thread Sync Barrier

#### Notes: 
* Block until event mask becomes 1, which means all tasks are synced, and then continue.

---

### 5. Task     Health Check
@code{.c}
void TaskHealth_Init(void);
@endcode

#### Discription:
Initializes task health check event group

@code{.c}
void TaskHealth_SetBit(EventBits_t bit);
@endcode

#### Discription:
Sets task bit to health check event group

@code{.c}
void TaskHealth_ClearBit(EventBits_t bit);
@endcode

#### Discription:
Clears task bit to health check event group

@code{.c}
void TaskHealth_ClearAll(void);
@endcode

#### Discription:
Clears health check event mask

@code{.c}
EventBits_t TaskHealth_Read(void);
@endcode

#### Discription:
Returns the value of the health check event mask

---
#### For more information on how events work, look up the EventGroup section in the FreeRTOS_Reference_Manual_V10.0.0.pdf.
