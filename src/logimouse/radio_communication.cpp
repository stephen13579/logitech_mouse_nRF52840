#include "radio_communication.h"

#include <esb.h>



// responsible for sending and recieving data to logitech RF dongle

// will build packets for transmision but not have anything specifically to do
// with logitech (ie build paclets as told not taylored to logitech)

// will have scanning mode to change RF frequency (or does dongle do this for us?)
// and we do not need to worry but just pick an RF and switch when told?
// might remove this funcitonality later

void radio_event_handler(struct esb_evt const *event)
{
	switch (event->evt_id) {
	case ESB_EVENT_TX_SUCCESS:
		//LOG_DBG("TX SUCCESS EVENT");
		break;
	case ESB_EVENT_TX_FAILED:
		//LOG_DBG("TX FAILED EVENT");
		break;
	case ESB_EVENT_RX_RECEIVED:
		if (esb_read_rx_payload(&rx_payload) == 0) {
			/*
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
				*/
		} else {
			//LOG_ERR("Error while reading rx packet");
		}
		break;
	}
}