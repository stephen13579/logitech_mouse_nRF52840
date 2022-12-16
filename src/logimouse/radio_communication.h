#ifndef RADIO_COMMUNICATION_H
#define RADIO_COMMUNICATION_H

#define RADIO_DEFAULT_CHANNEL_HOP_INTERVAL_MS 30 // used for scanning mode

#include <esb.h>

/*
 * Callback for radio events.
 * Will respond to TX and RX events including detected failures. 
 */
void radio_event_handler(struct esb_evt const *event);

static struct esb_payload rx_payload;

#endif // RADIO_COMMUNICATION_H