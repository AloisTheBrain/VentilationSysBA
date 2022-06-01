#include "knx_receive_telegram.h"
#include "global_config.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"



uint8_t knx_controlbytes[2];
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

float get_2byte_float_value(uint8_t *payload_buffer){
	  uint8_t exponent = (payload_buffer[2] & 0b01111000) >> 3;
	  uint8_t mantisse = ((payload_buffer[2] & 0b00000111) << 8) | (payload_buffer[3]);

	  //falls negatives vorzeichen(eigentlich nie)
	  if (payload_buffer[2] & 0b10000000) {
	    return ((-2048 + mantisse) * 0.01) * pow(2.0, exponent);
	  }

	  return (mantisse * 0.01) * pow(2.0, exponent);
}

bool check_for_controlbyte(uint8_t *buffer, uint8_t size){
	bool rtrn_buffer[size];
	bool rtrn = false;
	for(int i = 0; 0 < size; i++){
		rtrn_buffer[i] = ((buffer[i] | 0b00101100) == 0b10111100);
		rtrn = rtrn || rtrn_buffer[i];
	}
	return rtrn;
}

bool check_interest(uint8_t *address_buffer){
	uint8_t maingroup = get_target_maingroup(address_buffer);
	uint8_t middlegroup = get_target_middlegroup(address_buffer);
	uint8_t subgroup = get_target_subgroup(address_buffer);
	bool interested = is_target_group(address_buffer) && is_listening_to_group_address(maingroup, middlegroup, subgroup); //&&main middle und subgroup mit am anfang im code initilaisierten listening adessen vergleichen
	return interested;
}



void add_listen_group_address(char *address){

	/*
	char substring1[3];
	char substring2[3];
	char substring3[4];
	char *maingroup;
	char *middlegroup;
	char *subgroup;
	*/
	char *token;
	token = strtok(address, "/");
	for (int i = 0; i < 3; i++){
		listen_group_addresses[listen_group_address_counter][i] = atoi(token);
		token = strtok(NULL, "/");
	}

	listen_group_address_counter++;
	/*
	strncpy(substring1, &address[0], 2);
	substring1[2] = '\0';
	strncpy(substring1, &address[3], 2);
	substring2[1] = '\0';
	strncpy(substring1, &address[6], 3);
	substring3[1] = '\0';



	listen_group_addresses[listen_group_address_counter][0] = atoi(substring1);
	listen_group_addresses[listen_group_address_counter][1] = atoi(substring2);
	listen_group_addresses[listen_group_address_counter][2] = atoi(substring3);
	*/

}

bool is_listening_to_group_address(uint8_t maingroup, uint8_t middlegroup, uint8_t subgroup){
  for (int i = 0; i < listen_group_address_counter; i++){
    if ((listen_group_addresses[i][0] == maingroup)
         && (listen_group_addresses[i][1] == middlegroup)
         && (listen_group_addresses[i][2] == subgroup)){
      return true;
    }
  }
  return false;
}


