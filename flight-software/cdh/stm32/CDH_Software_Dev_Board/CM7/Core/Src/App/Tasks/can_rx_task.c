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

#include "usbd_cdc_if.h"
#include <stdio.h>

static StackType_t xTaskStack[NORMAL_TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

static CAN_RxPacket CAN_Rx;

static StaticQueue_t xxCAN_RXQueueData;
static uint8_t ucxCAN_RXQueueStorageArea[LOCAL_QUEUE_LENGTH * sizeof(CAN_Rx)];
static QueueHandle_t xCAN_RXQueue;

const char *subsystems[] = {
        "COMMS",
        "EPS",
        "PAYLOAD",
        "ADCS",
    };

void Subsystem_HB_Parser(CAN_RxPacket packet);
int Subsystem_HB_Print(uint8_t data, const char *subsystem);

Message_t newMsg;
static int status[4] = {0};

static void CAN_RX_Handler(void *argument){
	while(1){
		if(xQueueReceive((QueueHandle_t)argument,&CAN_Rx,portMAX_DELAY) == pdPASS){
			Subsystem_HB_Parser(CAN_Rx);
//			if(CAN_Rx.Header.Identifier == 0x103 || 0x204 || 0x304 || 0x404){
//
//			}
		}
	}
}

void CAN_RX_Task_Init(void){
	xCAN_RXQueue = xQueueCreateStatic(LOCAL_QUEUE_LENGTH, sizeof(CAN_Rx), ucxCAN_RXQueueStorageArea, &xxCAN_RXQueueData);
	xTaskCreateStatic(CAN_RX_Handler,"CAN_RX_Handler",NORMAL_TASK_STACK_SIZE,(void*)xCAN_RXQueue,CANRX_PR,xTaskStack,&xTaskBuffer);
}

QueueHandle_t CAN_RX_Task_GetQueue(void) {
    return xCAN_RXQueue;
}

void Subsystem_HB_Parser(CAN_RxPacket packet){
	switch(packet.Header.Identifier){
	//COMMS HB
	case 0x100:
		newMsg.Topic = TELECOMMAND;
		newMsg.Data.mode = SAFE;
		Publish(newMsg);
		break;
	case 0x101:
		newMsg.Topic = TELECOMMAND;
		newMsg.Data.mode = NOMINAL;
		Publish(newMsg);
		break;
	case 0x103:
		SetSubsystemStatus(COMMS, 1);
		Subsystem_HB_Print(*packet.Data, subsystems[0]);
		break;
	case 0x104:
		printf("SYSTEM COMMAND ACK: COMMS in SAFE MODE\r\n");
		fflush(stdout);
		break;
	case 0x105:
		printf("SYSTEM COMMAND ACK: COMMS in NOMINAL MODE\r\n");
		fflush(stdout);
		break;
	//EPS HB
	case 0x201:
		printf("SYSTEM COMMAND ACK: EPS in SAFE MODE\r\n");
		fflush(stdout);
		break;
	case 0x202:
		printf("SYSTEM COMMAND ACK: EPS in NOMINAL MODE\r\n");
		fflush(stdout);
		break;
	case 0x204:
		SetSubsystemStatus(EPS, 1);
		Subsystem_HB_Print(*packet.Data, subsystems[1]);
		break;
	case 0x203:
        newMsg.Topic = EPS_MSG;
        memcpy(&newMsg.Data.canPacket,&packet, sizeof(packet));
        Publish(newMsg);
		break;
	//ADCS HB
	case 0x301:
		printf("SYSTEM COMMAND ACK: ADCS in SAFE MODE\r\n");
		fflush(stdout);
		break;
	case 0x302:
		printf("SYSTEM COMMAND ACK: ADCS in NOMINAL MODE\r\n");
		fflush(stdout);
		break;
	case 0x303:
        newMsg.Topic = ADCS_MSG;
        memcpy(&newMsg.Data.canPacket,&packet, sizeof(packet));
        Publish(newMsg);
		break;
	case 0x304:
		SetSubsystemStatus(ADCS, 1);
		Subsystem_HB_Print(*packet.Data, subsystems[3]);
		break;
	//PL HB
	case 0x401:
		printf("SYSTEM COMMAND ACK: PAYLOAD in NOMINAL MODE\r\n");
		fflush(stdout);
		break;
	case 0x402:
		printf("SYSTEM COMMAND ACK: PAYLOAD in SAFE MODE\r\n");
		fflush(stdout);
		break;
	case 0x403:
        newMsg.Topic = PL_MSG;
        memcpy(&newMsg.Data.canPacket,&packet, sizeof(packet));
        Publish(newMsg);
		break;
	case 0x404:
		SetSubsystemStatus(PL, 1);
		Subsystem_HB_Print(*packet.Data, subsystems[2]);
		break;
	default:
		break;
	}
}

int Subsystem_HB_Print(uint8_t data, const char *subsystem){
	//Depending on the subsystem STATUS take action accordingly
	switch(data){
	case 1:
		//system in BOOT
		printf("%s in BOOT\r\n", subsystem);
		fflush(stdout);
		return 0;
	case 2:
		//system OK
		printf("%s OK\r\n", subsystem);
		fflush(stdout);
		return 1;
	case 3:
		//system ERROR
		printf("%s CRITICAL ERROR\r\n", subsystem);
		fflush(stdout);
		return 2;
	}
	return -1;
}

int GetSubsystemStatus(subsystemName sub){
	return status[sub];
}

void SetSubsystemStatus(subsystemName sub, int stat){
	status[sub] = stat;
}
