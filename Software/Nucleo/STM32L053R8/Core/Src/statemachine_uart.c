#include "statemachine_uart.h"
#include "knx_receive_telegram.h"
#include "stdlib.h"
#include "usart.h"
#include "global_config.h"

//statemachine_uart_state = CONTROLBYTE_STATE;
uint8_t payload_length = 0;
uint8_t interested_byte = 		0b00010001;
uint8_t not_interested_byte = 	0b00010000;

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
	//einmaliger start uart am anfang des progammes nicht vergessen
	if(flag_controlbyte_receive_started == FLAG_FALSE && flag_data_processed == FLAG_TRUE){
		HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));
		flag_controlbyte_receive_started = FLAG_TRUE;
	}
	//auf knx message prüfen
	if(is_knx_controlbyte(knx_controlbyte)){
		statemachine_uart_state = ADDRESS_STATE;
	}
	else{
		HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));
	}
}


//Richtiges controllbyte wurde empfangen
void statemachine_address_state(){

	if (flag_address_receive_started == FLAG_FALSE){
		HAL_UART_Receive_IT(&huart2, buffer_knx_address, sizeof(buffer_knx_address));
		flag_address_receive_started = FLAG_TRUE;
	}
	else{
		statemachine_uart_state = PAYLOAD_STATE;
	}

}

void statemachine_payload_state(){

	payload_length = get_payload_length(buffer_knx_address);

	if(flag_payload_receive_started == FLAG_FALSE){
		buffer_knx_payload = malloc(sizeof(uint8_t) * payload_length);			//speicher nach auswertung freigeben
		HAL_UART_Receive_IT(&huart2, buffer_knx_payload, sizeof(buffer_knx_payload));
		flag_payload_receive_started = FLAG_TRUE;
	}
	else{
		statemachine_uart_state = CHECKSUM_STATE;
	}
}

void statemachine_checksum_state(){
	if(flag_checksum_receive_started == FLAG_FALSE){
		HAL_UART_Receive_IT(&huart2, &knx_checksum_byte, sizeof(knx_checksum_byte));
		flag_checksum_receive_started = FLAG_TRUE;
	}
	else{
		statemachine_uart_state = ACK_STATE;
	}
}

void statemachine_ack_state(){
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
}

void process_data(){
	if(flag_uart_reception_complete == FLAG_TRUE){
		// to do aus buffer_knx_payload feuchtigkeit extrahieren
		flag_data_processed = FLAG_TRUE;
		flag_uart_reception_complete = FLAG_FALSE;
		free(buffer_knx_payload);
	}
}

