#include "interrupt_timer.h"

// includes from nrf connect dev env
#include <irq.h>
#include <nrfx_timer.h>
#include <esb.h>
#include <logging/log.h>

// includes from application
#include "radio_unifying.h"
#include "radio_communication.h"

LOG_MODULE_REGISTER(test, CONFIG_ESB_PRX_APP_LOG_LEVEL); // what is this? 

// Interrupt constructor
Interrupt::Interrupt()
{
	m_timer_counter = 0;
}

void Interrupt::timer_init(void)
{
	NRF_TIMER1->TASKS_STOP = 1; // Stop timer
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER1->BITMODE = (TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos);
	NRF_TIMER1->PRESCALER = NRF_TIMER_FREQ_62500Hz << TIMER_PRESCALER_PRESCALER_Pos; // 1us resolution
	nrf_timer_frequency_t frequency = NRF_TIMER_FREQ_62500Hz;
	NRF_TIMER1->TASKS_CLEAR = 1; // Clear timer
	uint32_t tick_time_value = nrf_timer_ms_to_ticks(RADIO_DEFAULT_CHANNEL_HOP_INTERVAL_MS, frequency);
	NRF_TIMER1->CC[0] = tick_time_value;	
	LOG_DBG("tick_time_value: %d", tick_time_value);
	NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;		   // compare to CC[0] value
	NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos); // reset CC[0] value after interrupt
	NVIC_EnableIRQ(TIMER1_IRQn);
	IRQ_DIRECT_CONNECT(TIMER1_IRQn, 0, interrupt_handler, 0); // connect interrupt (related to RTOS from zephyer)
	NRF_TIMER1->TASKS_START = 1;										 // Start TIMER
}

static void interrupt_handler()
{
	if (NRF_TIMER1->EVENTS_COMPARE[0] != 0)
	{
		int err;
		NRF_TIMER1->EVENTS_COMPARE[0] = 0;
		//err = esb_stop_rx();
		if (m_timer_counter >200)
		{
			// change to next frequency to begin listening
			LOG_DBG("ISR test");
			m_timer_counter = 0;
		}
		// set new radio frequency
		//err = esb_set_rf_channel(unifying_frequencies[m_timer_counter]);
		if (err)
		{
			// LOG_ERR("RF channel set failed: %d", err);
		}

		// start radio
		//err = esb_start_rx();
		if (err)
		{
			// LOG_ERR("ESB start failed: %d", err);
		}

		m_timer_counter++;
	}
}

void Interrupt::reset_timestamp_timer(void)
{
	NRF_TIMER1->TASKS_STOP = 1;
	NRF_TIMER1->TASKS_CLEAR = 1;
	m_timer_counter = 0;
	NRF_TIMER1->TASKS_START = 1;
}