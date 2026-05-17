/*
 * can_stm32.h
 *
 *  Created on: 20 Apr 2023
 *      Author: Xavier Hoyle
 */

#ifndef INCLUDE_CSP_DRIVERS_CAN_STM32_H_
#define INCLUDE_CSP_DRIVERS_CAN_STM32_H_

#include "main.h"
#include "../../CAN Interface/include/csp_if_can_stm32.h"

/**
   Open stm32 CAN and add CSP interface.

   @param[in] ifname CSP interface name, use #CSP_IF_CAN_DEFAULT_NAME for default name.
   @param[out] return_iface the added interface.
   @return The added interface, or NULL in case of failure.
*/
int csp_can_stm32_open_and_add_interface(const char * ifname, csp_iface_t ** return_iface, int id);



#endif /* INCLUDE_CSP_DRIVERS_CAN_STM32_H_ */
