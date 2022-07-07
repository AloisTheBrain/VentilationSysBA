#include "knx_receive_telegram.h"
#include "global_config.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"



uint8_t knx_controlbytes[2];
uint8_t listen_group_address_counter = 0;

//Überprüft das übergebenen Byte auf den Wert 188
bool is_knx_controlbyte(uint8_t byte){
  return ((byte | 0b00101100) == 0b10111100 );
}

//Extrahiert die Länge der Payload aus dem übergebenen Addressbuffer
uint8_t get_payload_length(uint8_t *address_buffer){
  uint8_t length = (address_buffer[4] & 0b00001111) + 1;
  return length;
}

/*Zielbit des Adressenfelds wird geprüft
Zielbit = 1 => Gruppenaddresse
Zielbit = 0 => physikalische Adresse*/

bool is_target_group(uint8_t *address_buffer){
  return address_buffer[4] & 0b10000000;
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

//Umwandlung von IEEE-Format in physikalischen Wert
float get_2byte_float_value(uint8_t *payload_buffer){
	  uint8_t exponent = (payload_buffer[2] & 0b01111000) >> 3;
	  uint16_t mantisse = ((payload_buffer[2] & 0b00000111) << 8) | (payload_buffer[3]);

	  /*falls negatives vorzeichen(eigentlich nie)

	  if (payload_buffer[2] & 0b10000000) {
	    return ((-2048 + mantisse) * 0.01) * pow(2.0, exponent);
	  }
	  */
	  return (mantisse * 0.01) * pow(2.0, exponent);
}

//Prüft ob der übergebene Buffer ein Kontrolbyte enthält
bool check_for_controlbyte(uint8_t *buffer, uint8_t size){
	bool rtrn_buffer[size];
	bool rtrn = false;
	for(int i = 0; i < size; i++){
		rtrn_buffer[i] = ((buffer[i] | 0b00101100) == 0b10111100);
		rtrn = rtrn || rtrn_buffer[i];
	}
	return rtrn;
}

//Vergleicht die übergebene Gruppenadresse mit den angelegten Gruppenadressen
bool check_interest(uint8_t *address_buffer){
	uint8_t maingroup = get_target_maingroup(address_buffer);
	uint8_t middlegroup = get_target_middlegroup(address_buffer);
	uint8_t subgroup = get_target_subgroup(address_buffer);
	bool interested = is_target_group(address_buffer) && is_listening_to_group_address(maingroup, middlegroup, subgroup); //&&main middle und subgroup mit am anfang im code initilaisierten listening adessen vergleichen
	return interested;
}

//zerlegt die übergebene char Adresse in int-Werte und fügt sie zu den angelegten Adressen hinzu
void add_listen_group_address(char *address){
	char *token;
	token = strtok(address, "/");
	for (int i = 0; i < 3; i++){
		listen_group_addresses[listen_group_address_counter][i] = atoi(token);
		token = strtok(NULL, "/");
	}
	listen_group_address_counter++;
}

//Hilfsfunktion für check_interest(uint8_t *address_buffer)
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


