

#ifndef INC_KNX_RECEIVE_TELEGRAM_H_
#define INC_KNX_RECEIVE_TELEGRAM_H_
#include "stdint.h"
#include <stdbool.h>

bool is_knx_controlbyte(uint8_t byte);
uint8_t get_payload_length(uint8_t *payload_buffer);
bool is_target_group(uint8_t *adress_buffer);
uint8_t get_target_maingroup(uint8_t *adress_buffer);
uint8_t get_target_middlegroup(uint8_t *adress_buffer);
uint8_t get_target_subgroup(uint8_t *adress_buffer);
bool check_interest(uint8_t *adress_buffer);
void clear_flags(void);
void add_listen_group_address(char *address);
bool is_listening_to_group_address(int main, int middle, int sub);


extern uint8_t listen_group_addresses[10][3];

#endif /* INC_KNX_RECEIVE_TELEGRAM_H_ */
