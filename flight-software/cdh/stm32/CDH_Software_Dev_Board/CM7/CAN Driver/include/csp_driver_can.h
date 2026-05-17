/*
 * csp_driver_can.h
 *
 *  Created on: 19 May 2023
 *      Author: Xavier Hoyle
 */

#ifndef INCLUDE_CSP_DRIVER_CAN_H_
#define INCLUDE_CSP_DRIVER_CAN_H_

#include <csp/csp_types.h>


void CAN_0_rx_callback(struct can_async_descriptor *const descr);

void CAN_0_irq_callback(struct _can_async_device *dev, enum can_async_interrupt_type type);

int csp_can_tx_frame(void *driver_data, uint32_t id, const uint8_t * data, uint8_t dlc);

csp_iface_t * csp_driver_can_init(int addr, int netmask, int id, can_mode_e mode, uint32_t bitrate);


#endif /* INCLUDE_CSP_DRIVER_CAN_H_ */
