/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

// #include <drivers/gpio.h>
#include <zephyr.h>
#include <logging/log.h>
#include "interrupt_timer.h"
#include "radio_controls.h"
#include "radio_unifying.h"
// #include <zephyr/types.h>
 
LOG_MODULE_REGISTER(esb_prx, CONFIG_ESB_PRX_APP_LOG_LEVEL); // what is this? 

void main(void) {
	Interrupt isr; // initialize logimouse
	isr.timer_init(); // initialize timer

	// create radio object
	RadioControls radio;

	while(1) {
		__WFE(); // wake from event (esb event or counter event)
		// transmit pairing packet 1 and log the response
		// log pairing packet
		LOG_DBG("Pairing packet 1 byte 0: 0x%02x", RadioUnifying::pairing_packet_1[0]);
		k_sleep(K_MSEC(100)); // sleep for 100 ms

	}

	return;
}
