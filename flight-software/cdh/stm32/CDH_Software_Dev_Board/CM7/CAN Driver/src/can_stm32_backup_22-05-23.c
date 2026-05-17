/*
 * can_stm32.c
 *
 *  Created on: 20 Apr 2023
 *      Author: Xavier Hoyle
 */

#include "can_stm32.h"
#include "csp_if_can_stm32.h"
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <stdlib.h>

// CAN interface data, state, etc.
typedef struct {
	char name[CSP_IFLIST_NAME_MAX + 1];
	csp_iface_t iface;
	csp_can_interface_data_t ifdata;
	csp_thread_handle_t rx_thread;
} can_context_t;

//static struct can_context_t {
//	char name[CSP_IFLIST_NAME_MAX +1];
//	csp_iface_t iface;
//	csp_can_interface_data_t ifdata;
//	csp_thread_handle_t rx_thread;
//} test_ctx[1] = {
//		{
//				.iface = {
//						.name = "CAN",
//						.ifdata = &test_ctx[0].ifdata,
//				},
//		}
//};



/* bad definition of can context so that the CAN receive interrupt can use the CAN interface and driver data */
/* not sure how to improve this, but this means that only the last interface added will be used for CAN receive interrupts */
//can_context_t *ctx = calloc(1, sizeof(*ctx));

static int csp_can_tx_frame(void *driver_data, uint32_t id, const uint8_t *data, uint8_t dlc) {
	if (dlc > 8) {
		return CSP_ERR_INVAL;
	}

	CAN_TxHeaderTypeDef txHeader;
	txHeader.DLC = dlc;
	txHeader.IDE = CAN_ID_EXT;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.ExtId = id;
	txHeader.TransmitGlobalTime = DISABLE;


	uint32_t canMailBox;
	HAL_CAN_AddTxMessage((CAN_HandleTypeDef *)driver_data, &txHeader, data, &canMailBox);

	return CSP_ERR_NONE;
}

///* CAN Receive Interrupt*/
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
//	CAN_RxHeaderTypeDef rxHeader;
//	uint8_t rxData[8] = {0,0,0,0,0,0,0,0};
//
//	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
//
//	csp_can_rx_stm32(&ctx->iface, rxHeader.ExtId, rxData, rxHeader.DLC, 0);
//}

int csp_can_stm32_open_and_add_interface(const char *ifname, csp_iface_t **return_iface) {
	if (ifname == NULL) {
		ifname = CSP_IF_CAN_DEFAULT_NAME;
	}

	can_context_t *ctx = calloc(1, sizeof(*ctx));
	if (ctx == NULL) {
		return CSP_ERR_NOMEM;
	}

	strncpy(ctx->name, ifname, sizeof(ctx->name) - 1);
	ctx->iface.name = ctx->name;
	ctx->iface.interface_data = &ctx->ifdata;
	ctx->iface.driver_data = (*return_iface)->driver_data;
	ctx->ifdata.tx_func = csp_can_tx_frame;

	/* Add the rest of the Driver (use can_socketcan.c as a guide) here */



	/* End of new functionality */

	int res = csp_can_add_interface_stm32(&ctx->iface);
	if (res != CSP_ERR_NONE) {
		return res;
	}

	if (return_iface) {
		*return_iface = &ctx->iface;
	}

	return CSP_ERR_NONE;
}
