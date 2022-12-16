#ifndef HELPER_H__
#define HELPER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void helper_addr_to_base_and_prefix(uint8_t *out_base_addr, uint8_t *out_prefix, uint8_t const *in_addr, uint8_t in_addr_len);
void helper_base_and_prefix_to_addr(uint8_t *out_addr, uint8_t const *in_base_addr, uint8_t in_prefix, uint8_t in_addr_len);

#endif