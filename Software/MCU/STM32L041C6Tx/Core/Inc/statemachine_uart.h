#include "global_config.h"

#ifndef INC_STATEMACHINE_UART_H_
#define INC_STATEMACHINE_UART_H_

#define INTERESTED_BYTE 0b00010001
#define NOT_INTERESTED_BYTE 0b0001000

extern uint8_t knx_controlbytes[2];

void statemachine_uart(void);
void statemachine_first_controlbyte_state(void);
void statemachine_second_controlbyte_state(void);
void statemachine_address_state(void);
void statemachine_payload_state(void);
void statemachine_ack_state(void);
void extract_data(void);

typedef enum{
	FIRST_CONTROLBYTE_STATE = 0,
	ADDRESS_STATE = 1,
	PAYLOAD_STATE = 2,
	SECOND_CONTROLBYTE_STATE = 3,
	ACK_STATE = 4,
} UART_STATEMACHINE_STATES;

#endif /* INC_STATEMACHINE_UART_H_ */
