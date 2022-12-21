#include "radio_controls.h"

#include <drivers/clock_control.h>
#include <drivers/clock_control/nrf_clock_control.h>

#include <logging/log.h>
#include <nrf.h>

// initialize radio with default for unifying
// use unifying settings from structure and start radio on appropriate
// address

// have a struct that can be modified elsewhere and passed
// here to be set for radio
// This will require stopping and starting radio approprietly 
// such as making sure no events are being processed (ie. rx even)
// and perhaps disabling interrupts in the meantime

LOG_MODULE_REGISTER(radio_controls, CONFIG_ESB_PRX_APP_LOG_LEVEL); // how does this work?

RadioControls::RadioControls() {
	// initialize radio
	int err = clocks_start();
	if (err) {
		LOG_DBG("Clock start failed: %d", err);
		return;
	}

	err = esb_initialize();
	if (err) {
		LOG_DBG("ESB initialization failed: %d", err);
		return;
	}

	//LOG_INF("ESB RX mode started");
}

int RadioControls::clocks_start(void)
{
	int err;
	int res;
	struct onoff_manager *clk_mgr;
	struct onoff_client clk_cli;

	clk_mgr = z_nrf_clock_control_get_onoff(CLOCK_CONTROL_NRF_SUBSYS_HF);
	if (!clk_mgr) {
		LOG_DBG("Unable to get the Clock manager");
		return -ENXIO;
	}

	sys_notify_init_spinwait(&clk_cli.notify);

	err = onoff_request(clk_mgr, &clk_cli);
	if (err < 0) {
		LOG_DBG("Clock request failed: %d", err);
		return err;
	}

	do {
		err = sys_notify_fetch_result(&clk_cli.notify, &res);
		if (!err && res) {
			LOG_DBG("Clock could not be started: %d", res);
			return res;
		}
	} while (err);

	//LOG_DBG("HF clock started");
	return 0;
}

int RadioControls::esb_initialize(void)
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

	return 0;
}