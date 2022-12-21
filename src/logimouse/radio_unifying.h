#ifndef RADIO_UNIFYING_H
#define RADIO_UNIFYING_H

#include <stdint.h>

class RadioUnifying {
	private:

	public:
		// (might build packet elsewhere and define less discretely)
		/* Pre-defined pairing packets */
		const static uint8_t pairing_packet_1[22];
		const static uint8_t pairing_packet_1_bis[5];
		const static uint8_t pairing_packet_2[22];
		const static uint8_t pairing_packet_2_bis[5];
		const static uint8_t pairing_packet_3[22];
		const static uint8_t pairing_packet_3_bis[5];
		const static uint8_t pairing_packet_4[10];
		/* End of pre-defined pairing packets */

		// Radio frequencies used by unifying devices
		const static uint32_t unifying_frequencies[25];


};

#endif // RADIO_UNIFYING_H