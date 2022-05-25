#include "knx_receive_telegram.h"
#include "global_config.h"
#include "string.h"
#include "stdlib.h"



uint8_t knx_controlbyte;
uint8_t knx_checksum_byte;
uint8_t buffer_knx_address[5];
uint8_t buffer_knx_header[6];
uint8_t *buffer_knx_payload = NULL;



uint8_t listen_group_address_counter = 0;

bool is_knx_controlbyte(uint8_t byte){
  return ((byte | 0b00101100) == 0b10111100 ); // Ignore repeat flag and priority flag
}

uint8_t get_payload_length(uint8_t *address_buffer){
  uint8_t length = (address_buffer[4] & 0b00001111) + 1;
  return length;
}

bool is_target_group(uint8_t *address_buffer){
  return address_buffer[4] & 0b10000000; 	//Letztes bit von zielbyte auslesen, wenn 1 dann gruppenadresse
}

uint8_t get_target_maingroup(uint8_t *address_buffer){
  return ((address_buffer[2] & 0b01111000) >> 3);
}

uint8_t get_target_middlegroup(uint8_t *address_buffer){
  return (address_buffer[2] & 0b00000111);
}

uint8_t get_target_subgroup(uint8_t *address_buffer){
  return address_buffer[3];
}

bool check_interest(uint8_t *address_buffer){
	uint8_t maingroup = get_target_maingroup(address_buffer);
	uint8_t middlegroup = get_target_middlegroup(address_buffer);
	uint8_t subgroup = get_target_subgroup(address_buffer);
	bool interested = is_target_group(address_buffer) && is_listening_to_group_address(maingroup, middlegroup, subgroup); //&&main middle und subgroup mit am anfang im code initilaisierten listening adessen vergleichen
	return interested;
}

void clear_flags(void){
	flag_controlbyte_receive_started 	= FLAG_FALSE;
	flag_address_receive_started 		= FLAG_FALSE;
	flag_payload_receive_started 		= FLAG_FALSE;
	flag_checksum_receive_started		= FLAG_FALSE;
}

void add_listen_group_address(char *address){

	// adressen format:  "15/0/0"
	char substring1[3];
	char substring2[1];

	strncpy(substring1, &address[0], 2);
	substring1[2] = '\0';
	substring2[0] = address[3];
	int substring3 = address[5] + '0';
	listen_group_addresses[listen_group_address_counter][0] = atoi(substring1);
	listen_group_addresses[listen_group_address_counter][1] = atoi(substring2);
	listen_group_addresses[listen_group_address_counter][2] = substring3;
	listen_group_address_counter++;
}

bool is_listening_to_group_address(int maingroup, int middlegroup, int subgroup){
  for (int i = 0; i < listen_group_address_counter; i++){
    if ((listen_group_addresses[i][0] == maingroup)
         && (listen_group_addresses[i][1] == middlegroup)
         && (listen_group_addresses[i][2] == subgroup)){
      return true;
    }
  }
  return false;
}


