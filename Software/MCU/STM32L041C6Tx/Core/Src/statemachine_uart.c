#include "statemachine_uart.h"
#include "knx_receive_telegram.h"
#include "stdlib.h"
#include "usart.h"
#include "global_config.h"

//statemachine_uart_state = CONTROLBYTE_STATE;
uint8_t payload_length = 0;
uint8_t interested_byte = 		0b00010001;
uint8_t not_interested_byte = 	0b00010000;

uint8_t statemachine_uart_state = CONTROLBYTE_STATE;

uint8_t get_payload_lenght(uint8_t *buffer_knx_address);



void statemachine_uart()
{
	switch(statemachine_uart_state)
	{

	case CONTROLBYTE_STATE:
		statemachine_controlbyte_state();
		break;

	case ADDRESS_STATE:
		statemachine_address_state();
		break;

	case PAYLOAD_STATE:
		statemachine_payload_state();
		break;

	case CHECKSUM_STATE:
		statemachine_checksum_state();
		break;

	case ACK_STATE:
		statemachine_ack_state();
		break;

	default:
		break;
	}
}

void statemachine_controlbyte_state(){
	if(is_knx_controlbyte(knx_controlbyte) && flag_data_processed == FLAG_TRUE){
		statemachine_uart_state = ADDRESS_STATE;
		HAL_UART_Receive_IT(&huart2, buffer_knx_address, sizeof(buffer_knx_address));
	}
	else{
		HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));
	}

}


//Richtiges controllbyte wurde empfangen
void statemachine_address_state(){

	payload_length = get_payload_length(buffer_knx_address);
	buffer_knx_payload = malloc(sizeof(uint8_t) * payload_length);			//speicher nach auswertung freigeben
	statemachine_uart_state = PAYLOAD_STATE;
	HAL_UART_Receive_IT(&huart2, buffer_knx_payload, sizeof(buffer_knx_payload));

}

void statemachine_payload_state(){

	statemachine_uart_state = CHECKSUM_STATE;
	HAL_UART_Receive_IT(&huart2, &knx_checksum_byte, sizeof(knx_checksum_byte));
}

void statemachine_checksum_state(){

	if(check_interest(buffer_knx_address)){
		HAL_UART_Transmit_IT(&huart2, &interested_byte, sizeof(interested_byte));
		flag_uart_reception_complete = FLAG_TRUE;
		flag_data_processed = FLAG_FALSE;
	}
	else{
		HAL_UART_Transmit_IT(&huart2, &not_interested_byte, sizeof(not_interested_byte));
	}


	clear_flags();
	statemachine_uart_state = CONTROLBYTE_STATE;
	HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));

}

void statemachine_ack_state(){


}

void extract_data(void){
	if(flag_uart_reception_complete == FLAG_TRUE){
		// to do aus buffer_knx_payload feuchtigkeit extrahieren
		float humidity = 0;
		flag_data_processed = FLAG_TRUE;
		flag_uart_reception_complete = FLAG_FALSE;
		actual_humidity = humidity;
		free(buffer_knx_payload);
	  }
}


