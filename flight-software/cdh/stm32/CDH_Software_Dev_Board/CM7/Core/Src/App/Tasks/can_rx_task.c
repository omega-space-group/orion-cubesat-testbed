/*
 * can_rx_task.c
 *
 *  Created on: Jun 5, 2026
 *      Author: adaro
 */
#include <App/Tasks/can_rx_task.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "fdcan.h"

#include <App/app_config.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static CAN_RxPacket CAN_Rx;

static StaticQueue_t xxCAN_RXQueueData;
static uint8_t ucxCAN_RXQueueStorageArea[LOCAL_QUEUE_LENGTH * sizeof(CAN_Rx)];
static QueueHandle_t xCAN_RXQueue;


int cnt = 0;

static void CAN_RX_Handler(void *argument){
	while(1){
		if(xQueueReceive((QueueHandle_t)argument,&CAN_Rx,portMAX_DELAY) == pdPASS){
			switch(CAN_Rx.Header.Identifier){
			case 0x300:
				cnt++;
				break;
			default:
				break;
			}
		}
	}
}

void CAN_RX_Task_Init(void){
	xCAN_RXQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, MSG_SIZE, ucxCAN_RXQueueStorageArea, &xxCAN_RXQueueData);
	xTaskCreateStatic(CAN_RX_Handler,"CAN_RX_Handler",NORMAL_TASK_STACK_SIZE,(void*)xCAN_RXQueue,CANRX_PR,xTaskStack,&xTaskBuffer);
}

QueueHandle_t CAN_RX_Task_GetQueue(void) {
    return xCAN_RXQueue;
}
