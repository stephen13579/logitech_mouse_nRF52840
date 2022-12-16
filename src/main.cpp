/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

// #include <drivers/gpio.h>
// #include <logging/log.h>
#include <zephyr.h>
#include <logging/log.h>
#include "interrupt_timer.h"
// #include <zephyr/types.h>
 
LOG_MODULE_REGISTER(esb_prx, CONFIG_ESB_PRX_APP_LOG_LEVEL); // what is this? 

void main(void) {
	Interrupt isr; // initialize logimouse
	isr.timer_init(); // initialize timer

	while(1) {
		__WFE(); // wake from event (esb event or counter event)
		
	}

	return;
}
