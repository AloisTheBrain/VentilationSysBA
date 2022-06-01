#include "statemachine_uart.h"
#include "knx_receive_telegram.h"
#include "stdlib.h"
#include "usart.h"
#include "global_config.h"


uint8_t payload_length = 0;
uint8_t interested_byte = 		0b00010001;
uint8_t not_interested_byte = 	0b00010000;


uint8_t knx_checksum_byte;
uint8_t buffer_knx_address[5];
uint8_t buffer_knx_header[6];
uint8_t *buffer_knx_payload = NULL;

uint8_t statemachine_uart_state = CONTROLBYTE_STATE;

bool flag_knx_message_interested = FLAG_FALSE;
bool flag_data_processed = FLAG_TRUE;
bool flag_uart_reception_complete = FLAG_FALSE;


uint8_t buffer_val1 = 0;
uint8_t buffer_val2 = 0;
uint8_t buffer_val3 = 0;
uint8_t buffer_val4 = 0;
uint8_t buffer_val5 = 0;



void statemachine_uart(void){
	switch(statemachine_uart_state){

	case CONTROLBYTE_STATE:
		statemachine_controlbyte_state();
		break;

	case ADDRESS_STATE:
		statemachine_address_state();
		break;

	case PAYLOAD_STATE:
		statemachine_payload_state();
		break;

	case ACK_STATE:
		statemachine_ack_state();
		break;

	default:
		break;
	}
}

void statemachine_controlbyte_state(void){
	if(is_knx_controlbyte(knx_controlbytes[0]) && is_knx_controlbyte(knx_controlbytes[1]) && flag_data_processed == FLAG_TRUE){
		HAL_UART_Receive_IT(&huart2, buffer_knx_address, sizeof(buffer_knx_address));
		statemachine_uart_state = ADDRESS_STATE;
	}
	else{
		HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));
	}
}


//Richtiges controllbyte wurde empfangen
void statemachine_address_state(void){
	if(check_for_controlbyte(buffer_knx_address, sizeof(buffer_knx_address))){
		statemachine_uart_state = CONTROLBYTE_STATE;
		HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));
	}
	else{
		payload_length = get_payload_length(buffer_knx_address);
		buffer_knx_payload = (uint8_t *) malloc( sizeof(uint8_t) * payload_length );			//speicher nach auswertung freigeben
		statemachine_uart_state = PAYLOAD_STATE;
		HAL_UART_Receive_IT(&huart2, buffer_knx_payload, payload_length);
	}
}

void statemachine_payload_state(void){
	buffer_val1 = buffer_knx_payload[0];
	buffer_val2 = buffer_knx_payload[1];
	buffer_val3 = buffer_knx_payload[2];
	buffer_val4 = buffer_knx_payload[3];
	buffer_val5 = buffer_knx_payload[4];
	if(check_for_controlbyte(buffer_knx_payload, sizeof(buffer_knx_payload))){
		statemachine_uart_state = CONTROLBYTE_STATE;
		HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));
	}
	else{
		statemachine_uart_state = ACK_STATE;
		HAL_UART_Receive_IT(&huart2, &knx_checksum_byte, sizeof(knx_checksum_byte));

	}

}

void statemachine_ack_state(void){

	if(is_knx_controlbyte(knx_checksum_byte)){
			statemachine_uart_state = CONTROLBYTE_STATE;
			HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));
		}
	else if(check_interest(buffer_knx_address)){
		HAL_UART_Transmit_IT(&huart2, &interested_byte, sizeof(interested_byte));
		flag_knx_message_interested = FLAG_TRUE;
	}
	else{
		HAL_UART_Transmit_IT(&huart2, &not_interested_byte, sizeof(not_interested_byte));
		flag_knx_message_interested = FLAG_FALSE;
	}


	flag_uart_reception_complete = FLAG_TRUE;
	flag_data_processed = FLAG_FALSE;
	statemachine_uart_state = CONTROLBYTE_STATE;
	HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));


}



void extract_data(void){
	if(flag_uart_reception_complete == FLAG_TRUE){
		if(flag_knx_message_interested == FLAG_TRUE){}
		actual_humidity = get_2byte_float_value(buffer_knx_payload);
		free(buffer_knx_payload);
		flag_data_processed = FLAG_TRUE;
		flag_uart_reception_complete = FLAG_FALSE;
	  }
}


