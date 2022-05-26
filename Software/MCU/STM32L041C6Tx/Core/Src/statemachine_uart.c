#include "statemachine_uart.h"
#include "knx_receive_telegram.h"
#include "stdlib.h"
#include "usart.h"
#include "global_config.h"

//statemachine_uart_state = CONTROLBYTE_STATE;
uint8_t payload_length = 0;
uint8_t interested_byte = 		0b00010001;
uint8_t not_interested_byte = 	0b00010000;

static uint8_t statemachine_uart_state = CONTROLBYTE_STATE;
bool flag_knx_message_interested = FLAG_FALSE;

uint8_t get_payload_lenght(uint8_t *buffer_knx_address);

uint8_t buffer_val1 = 0;
uint8_t buffer_val2 = 0;
uint8_t buffer_val3 = 0;
uint8_t buffer_val4 = 0;
uint8_t buffer_val5 = 0;



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
		HAL_UART_Receive_IT(&huart2, buffer_knx_address, sizeof(buffer_knx_address));
		statemachine_uart_state = ADDRESS_STATE;
	}
	else{
		HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));
	}

}


//Richtiges controllbyte wurde empfangen
void statemachine_address_state(){
	payload_length = get_payload_length(buffer_knx_address);
	buffer_knx_payload = (uint8_t *) malloc(sizeof(uint8_t) * payload_length );			//speicher nach auswertung freigeben
	HAL_UART_Receive_IT(&huart2, buffer_knx_payload, payload_length);
	statemachine_uart_state = PAYLOAD_STATE;
}

void statemachine_payload_state(){
	buffer_val1 = buffer_knx_payload[0];
	buffer_val2 = buffer_knx_payload[1];
	buffer_val3 = buffer_knx_payload[2];
	buffer_val4 = buffer_knx_payload[3];
	buffer_val5 = buffer_knx_payload[4];
	HAL_UART_Receive_IT(&huart2, &knx_checksum_byte, sizeof(knx_checksum_byte));
	statemachine_uart_state = CHECKSUM_STATE;

}

void statemachine_checksum_state(){
	flag_entered_uart_callback = FLAG_TRUE;
	if(check_interest(buffer_knx_address)){
		HAL_UART_Transmit_IT(&huart2, &interested_byte, sizeof(interested_byte));
		flag_knx_message_interested = FLAG_TRUE;
	}
	else{
		HAL_UART_Transmit_IT(&huart2, &not_interested_byte, sizeof(not_interested_byte));
		flag_knx_message_interested = FLAG_FALSE;
	}
	HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));

	clear_flags();

	flag_uart_reception_complete = FLAG_TRUE;
	flag_data_processed = FLAG_FALSE;
	statemachine_uart_state = CONTROLBYTE_STATE;


}

void statemachine_ack_state(){


}

void extract_data(void){
	if(flag_uart_reception_complete == FLAG_TRUE){
		if(flag_knx_message_interested == FLAG_TRUE){}
		// to do aus buffer_knx_payload feuchtigkeit extrahieren
		float humidity = 0;
		//HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));

		actual_humidity = humidity;
		free(buffer_knx_payload);
		flag_data_processed = FLAG_TRUE;
		flag_uart_reception_complete = FLAG_FALSE;
	  }
}


