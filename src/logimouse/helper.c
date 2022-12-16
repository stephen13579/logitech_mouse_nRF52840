#include "helper.h"

#include <logging/log.h>

void helper_addr_to_base_and_prefix(uint8_t *out_base_addr, uint8_t *out_prefix, uint8_t const *in_addr, uint8_t in_addr_len) {
    int pos = 0;
    for (int i=in_addr_len-2; i >= 0; i--) {
        out_base_addr[pos++] = in_addr[i];
    }
    *out_prefix = in_addr[in_addr_len-1];
    
}

void helper_base_and_prefix_to_addr(uint8_t *out_addr, uint8_t const *in_base_addr, uint8_t in_prefix, uint8_t in_addr_len) {
    int pos = 0;
    for (int i=in_addr_len-2; i >= 0; i--) {
        out_addr[i] = in_base_addr[pos++];
    }
    out_addr[in_addr_len-1] = in_prefix;

}