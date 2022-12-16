#ifndef RADIO_CONTROLS_H
#define RADIO_CONTROLS_H

#include <esb.h>

class RadioControls {
    private:
        esb_config config;

        // config changes from default that need to be added by mouse
        // and need to set RF 
    	//config.retransmit_delay = 250; // lower retransmit delay from default of 600 
    	//config.event_handler = event_handler; // add radio event handler

        int clocks_start(void); // start HF clock (needed for ESB to work)
        int esb_initialize(void); // initialize esb radio
        // want a structure for esb initialize values to be passed in

        // rx and tx payloads
        static struct esb_payload rx_payload;
        static struct esb_payload tx_payload;

    public: 
        RadioControls(); // initialize radio on nrf device
        int esb_set_rf_channel(uint32_t channel); // set rf channel to first element in array of unifying
        // could be a helper function like set_rf(uint32 channel); and implementation file calls esb.h defined one

}; // namespace controls::radio

#endif // RADIO_CONTROLS_H