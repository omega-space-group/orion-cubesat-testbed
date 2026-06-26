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
#include <string.h>

#include <App/app_config.h>
#include <App/Services/subscriptions.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static CAN_RxPacket CAN_Rx;

static StaticQueue_t xxCAN_RXQueueData;
static uint8_t ucxCAN_RXQueueStorageArea[LOCAL_QUEUE_LENGTH * sizeof(CAN_Rx)];
static QueueHandle_t xCAN_RXQueue;


volatile int cnt1 = 0,cnt2 = 0;
Message_t newMsg;
static void CAN_RX_Handler(void *argument){

	while(1){
//		if(xQueueReceive((QueueHandle_t)argument,&CAN_Rx,portMAX_DELAY) == pdPASS){
		xQueueReceive((QueueHandle_t)argument,&CAN_Rx,portMAX_DELAY);
			switch(CAN_Rx.Header.Identifier){
			//COMMS HB
			case 0x103:
				cnt1++;
//				newMsg.Topic = SUBSYSTEM_STATUS;
//				newMsg.Data.canPacket.Header.Identifier = CAN_Rx.Header.Identifier;
//				memcpy(newMsg.Data.canPacket.Data, CAN_Rx.Data, sizeof(CAN_Rx.Data));
//				Publish(newMsg);
				break;
			//EPS HB
			case 0x205:
				cnt2++;
//				newMsg.Topic = SUBSYSTEM_STATUS;
//				newMsg.Data.canPacket.Header.Identifier = CAN_Rx.Header.Identifier;
//				memcpy(newMsg.Data.canPacket.Data, CAN_Rx.Data, sizeof(CAN_Rx.Data));
//				Publish(newMsg);
				break;
			//ADCS HB
			case 0x305:
//				newMsg.Topic = SUBSYSTEM_STATUS;
//				newMsg.Data.canPacket.Header.Identifier = CAN_Rx.Header.Identifier;
//				memcpy(newMsg.Data.canPacket.Data, CAN_Rx.Data, sizeof(CAN_Rx.Data));
//				Publish(newMsg);
				break;
			//PL HB
			case 0x405:
//				newMsg.Topic = SUBSYSTEM_STATUS;
//				newMsg.Data.canPacket.Header.Identifier = CAN_Rx.Header.Identifier;
//				memcpy(newMsg.Data.canPacket.Data, CAN_Rx.Data, sizeof(CAN_Rx.Data));
//				Publish(newMsg);
				break;
			default:
				break;
			}
//		}
	}
}

void CAN_RX_Task_Init(void){
	xCAN_RXQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, sizeof(CAN_Rx), ucxCAN_RXQueueStorageArea, &xxCAN_RXQueueData);
	xTaskCreateStatic(CAN_RX_Handler,"CAN_RX_Handler",NORMAL_TASK_STACK_SIZE,(void*)xCAN_RXQueue,CANRX_PR,xTaskStack,&xTaskBuffer);
}

QueueHandle_t CAN_RX_Task_GetQueue(void) {
    return xCAN_RXQueue;
}
