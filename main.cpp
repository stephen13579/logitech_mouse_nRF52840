/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <drivers/clock_control.h>
#include <drivers/clock_control/nrf_clock_control.h>
#include <drivers/gpio.h>
#include <irq.h>
#include <logging/log.h>
#include <nrf.h>
#include <esb.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include <nrfx_timer.h>
 


LOG_MODULE_REGISTER(esb_prx, CONFIG_ESB_PRX_APP_LOG_LEVEL);

#define RADIO_DEFAULT_CHANNEL_HOP_INTERVAL_MS 30 // use 30ms

uint32_t m_timer_counter = 0;

static const struct device *led_port;
static struct esb_payload rx_payload;
static struct esb_payload tx_payload = ESB_CREATE_PAYLOAD(0,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17);

uint32_t unifying_frequencies[25] = {5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 
								    44, 47, 50, 53, 56, 59, 62, 65, 68, 71, 74, 77};

/* Pre-defined pairing packets */
uint8_t pairing_packet_1[22] = {0x15, 0x5F, 0x01, 0x84, 0x5E, 0x3A, 0xA2, 0x57, 0x08, 0x10, 0x25, 0x04, 0x00, 0x01, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xEC};
uint8_t pairing_packet_1_bis[5] = {0x15, 0x40, 0x01, 0x84, 0x26};
uint8_t pairing_packet_2[22] = {0x00, 0x5F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x58, 0x8A, 0x51, 0xEA, 0x01, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t pairing_packet_2_bis[5] = {0x00, 0x40, 0x02, 0x01, 0xbd};
uint8_t pairing_packet_3[22] = {0x00, 0x5F, 0x03, 0x01, 0x00, 0x04, 0x4D, 0x35, 0x31, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB6};
uint8_t pairing_packet_3_bis[5] = {0x00, 0x5F, 0x03, 0x01, 0x0f};
uint8_t pairing_packet_4[10] = {0x00, 0x0F, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA};
/* Enf of pre-defined pairing packets */

void interrupt_handler() {
	if(NRF_TIMER1->EVENTS_COMPARE[0] != 0)
    {
		int err; 
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;
		err = esb_stop_rx();
		if (m_timer_counter > 24) 
		{
			// change to next frequency to begin listening
			m_timer_counter = 0;
		}
		// set new radio frequency 
		err = esb_set_rf_channel(unifying_frequencies[m_timer_counter]);
		if (err) 
		{
			LOG_ERR("RF channel set failed: %d", err);
		}

		// start radio
		err = esb_start_rx();
		if (err) 
		{
			LOG_ERR("ESB start failed: %d", err);
		}

		m_timer_counter++;
    }
}

static void timer_init(void) 
{
	NRF_TIMER1->TASKS_STOP = 1; // Stop timer
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER1->BITMODE = (TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos);
	NRF_TIMER1->PRESCALER = 8 << TIMER_PRESCALER_PRESCALER_Pos; // 1us resolution
	NRF_TIMER1->TASKS_CLEAR = 1; // Clear timer
	uint32_t tick_time_value = nrf_timer_ms_to_ticks(RADIO_DEFAULT_CHANNEL_HOP_INTERVAL_MS, NRF_TIMER1->PRESCALER);
	NRF_TIMER1->CC[0] = tick_time_value; // using compare register CC[0]
	NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos; // compare to CC[0] value
	NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos); // reset CC[0] value after interrupt
	NVIC_EnableIRQ(TIMER1_IRQn);
	IRQ_DIRECT_CONNECT(TIMER1_IRQn, 0, interrupt_handler, 0); // connect interrupt (related to RTOS from zephyer)
	NRF_TIMER1->TASKS_START = 1; // Start TIMER 
}

static int leds_init(void)
{
	led_port = device_get_binding(DT_GPIO_LABEL(DT_ALIAS(led0), gpios));
	if (!led_port) {
		LOG_ERR("Could not bind to LED port %s",
			DT_GPIO_LABEL(DT_ALIAS(led0), gpios));
		return -EIO;
	}

	const uint8_t pins[] = {DT_GPIO_PIN(DT_ALIAS(led0), gpios),
			     DT_GPIO_PIN(DT_ALIAS(led1), gpios),
			     DT_GPIO_PIN(DT_ALIAS(led2), gpios),
			     DT_GPIO_PIN(DT_ALIAS(led3), gpios)};

	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
		int err = gpio_pin_configure(led_port, pins[i], GPIO_OUTPUT);

		if (err) {
			LOG_ERR("Unable to configure LED%u, err %d.", i, err);
			led_port = NULL;
			return err;
		}
	}

	return 0;
}

static void leds_update(uint8_t value)
{
	bool led0_status = !(value % 8 > 0 && value % 8 <= 4);
	bool led1_status = !(value % 8 > 1 && value % 8 <= 5);
	bool led2_status = !(value % 8 > 2 && value % 8 <= 6);
	bool led3_status = !(value % 8 > 3);

	gpio_port_pins_t mask =
		1 << DT_GPIO_PIN(DT_ALIAS(led0), gpios) |
		1 << DT_GPIO_PIN(DT_ALIAS(led1), gpios) |
		1 << DT_GPIO_PIN(DT_ALIAS(led2), gpios) |
		1 << DT_GPIO_PIN(DT_ALIAS(led3), gpios);

	gpio_port_value_t val =
		led0_status << DT_GPIO_PIN(DT_ALIAS(led0), gpios) |
		led1_status << DT_GPIO_PIN(DT_ALIAS(led1), gpios) |
		led2_status << DT_GPIO_PIN(DT_ALIAS(led2), gpios) |
		led3_status << DT_GPIO_PIN(DT_ALIAS(led3), gpios);

	if (led_port != NULL) {
		gpio_port_set_masked_raw(led_port, mask, val);
	}
}

void event_handler(struct esb_evt const *event)
{
	switch (event->evt_id) {
	case ESB_EVENT_TX_SUCCESS:
		LOG_DBG("TX SUCCESS EVENT");
		break;
	case ESB_EVENT_TX_FAILED:
		LOG_DBG("TX FAILED EVENT");
		break;
	case ESB_EVENT_RX_RECEIVED:
		if (esb_read_rx_payload(&rx_payload) == 0) {
			LOG_DBG("Packet received, len %d : "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x",
				rx_payload.length, rx_payload.data[0],
				rx_payload.data[1], rx_payload.data[2],
				rx_payload.data[3], rx_payload.data[4],
				rx_payload.data[5], rx_payload.data[6],
				rx_payload.data[7], rx_payload.data[8],
				rx_payload.data[9], rx_payload.data[10],
				rx_payload.data[11], rx_payload.data[12],
				rx_payload.data[13], rx_payload.data[14],
				rx_payload.data[15], rx_payload.data[16], 
				rx_payload.data[17], rx_payload.data[18],
				rx_payload.data[19], rx_payload.data[20],
				rx_payload.data[21]);
		} else {
			LOG_ERR("Error while reading rx packet");
		}
		break;
	}
}

int clocks_start(void)
{
	int err;
	int res;
	struct onoff_manager *clk_mgr;
	struct onoff_client clk_cli;

	clk_mgr = z_nrf_clock_control_get_onoff(CLOCK_CONTROL_NRF_SUBSYS_HF);
	if (!clk_mgr) {
		LOG_ERR("Unable to get the Clock manager");
		return -ENXIO;
	}

	sys_notify_init_spinwait(&clk_cli.notify);

	err = onoff_request(clk_mgr, &clk_cli);
	if (err < 0) {
		LOG_ERR("Clock request failed: %d", err);
		return err;
	}

	do {
		err = sys_notify_fetch_result(&clk_cli.notify, &res);
		if (!err && res) {
			LOG_ERR("Clock could not be started: %d", res);
			return res;
		}
	} while (err);

	LOG_DBG("HF clock started");
	return 0;
}

int esb_initialize(void)
{
	int err;
	/* These are arbitrary default addresses. In end user products
	 * different addresses should be used for each set of devices.
	 */
	// 0x BB 0A DC A5 75 unifying pairing address [5]
	uint8_t base_addr_0[4] = {0xA5, 0xDC, 0x0A, 0xBB};
	uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};

	// uint8_t base[4] = { 0 };
    // uint8_t prefix = 0;
	// helper_addr_to_base_and_prefix({0, 0, 0, 0}, 0, rf_address, 5);
	/*
	void helper_addr_to_base_and_prefix(uint8_t *out_base_addr, uint8_t *out_prefix, uint8_t const *in_addr, uint8_t in_addr_len) {
	    int pos = 0;
	    for (int i=5-2; i >= 0; i--) {
        	out_base_addr[pos++] = {0xBB, 0x0A, 0xDC, 0xA5, 0x75}[i];
    	}
    	*out_prefix = in_addr[in_addr_len-1];
    }
	// base address = A5, DC, 0A, BB
	// prefix = 75
	*/

	uint8_t addr_prefix[8] = {0x75, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8};

	struct esb_config config = ESB_DEFAULT_CONFIG;

	// config changes from default
	config.retransmit_delay = 250; 
	config.crc = ESB_CRC_16BIT;
	config.protocol = ESB_PROTOCOL_ESB_DPL;
	config.bitrate = ESB_BITRATE_2MBPS;
	config.mode = ESB_MODE_PRX;
	config.event_handler = event_handler;
	config.selective_auto_ack = false;

	err = esb_init(&config);
	if (err) {
		return err;
	}

	err = esb_set_base_address_0(base_addr_0);
	if (err) {
		return err;
	}

	err = esb_set_base_address_1(base_addr_1);
	if (err) {
		return err;
	}

	err = esb_set_prefixes(addr_prefix, ARRAY_SIZE(addr_prefix));
	if (err) {
		return err;
	}

	err = esb_set_rf_channel(unifying_frequencies[0]); // set rf channel to first element in array of unifying
	if (err) {
		return err;
	}

	return 0;
}

// 
static void reset_timestamp_timer(void)
{
    NRF_TIMER1->TASKS_STOP = 1;
    NRF_TIMER1->TASKS_CLEAR = 1;
    m_timer_counter = 0;
    NRF_TIMER1->TASKS_START = 1;
}

// start the esb in recieve mode
uint32_t app_sniffer_start_rx(void)
{
    reset_timestamp_timer();
    uint32_t err_code = esb_start_rx();
    return err_code;
}

// stop esb operations
uint32_t app_sniffer_stop_rx(void)
{
    uint32_t err_code = esb_stop_rx();
    return err_code;
}

// MAIN
void main(void)
{
	int err;

	LOG_INF("Enhanced ShockBurst scannner for unifying frequencies");

	err = clocks_start();
	if (err) {
		return;
	}

	timer_init(); // initialize interrupt that scans RF channels

	leds_init();

	err = esb_initialize();
	if (err) {
		LOG_ERR("ESB initialization failed, err %d", err);
		return;
	}

	LOG_INF("Initialization complete");

	err = esb_write_payload(&tx_payload);
	if (err) {
		LOG_ERR("Write payload, err %d", err);
		return;
	}

	LOG_INF("Setting up for packet receiption");

	err = esb_start_rx();
	if (err) {
		LOG_ERR("RX setup failed, err %d", err);
		return;
	}

	while(1) {
		__WFE(); // wake from event (esb event or counter event)
	}

	return;
}
