#ifndef RADIO_UNIFYING_H
#define RADIO_UNIFYING_H

// ADD to class (might build packet elsewhere and define less discretely)
/* Pre-defined pairing packets */
uint8_t pairing_packet_1[22] = {0x15, 0x5F, 0x01, 0x84, 0x5E, 0x3A, 0xA2, 0x57, 0x08, 0x10, 0x25, 0x04, 0x00, 0x01, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xEC};
uint8_t pairing_packet_1_bis[5] = {0x15, 0x40, 0x01, 0x84, 0x26};
uint8_t pairing_packet_2[22] = {0x00, 0x5F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x58, 0x8A, 0x51, 0xEA, 0x01, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t pairing_packet_2_bis[5] = {0x00, 0x40, 0x02, 0x01, 0xbd};
uint8_t pairing_packet_3[22] = {0x00, 0x5F, 0x03, 0x01, 0x00, 0x04, 0x4D, 0x35, 0x31, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB6};
uint8_t pairing_packet_3_bis[5] = {0x00, 0x5F, 0x03, 0x01, 0x0f};
uint8_t pairing_packet_4[10] = {0x00, 0x0F, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA};
/* End of pre-defined pairing packets */


// Radio frequencies used by unifying devices
uint32_t unifying_frequencies[25] = {5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 
								    44, 47, 50, 53, 56, 59, 62, 65, 68, 71, 74, 77};

#endif // RADIO_UNIFYING_H