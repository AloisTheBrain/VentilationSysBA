
#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "stdint.h"


extern uint8_t statemachine_process_state;

void statemachine_standby_state();
void statemachine_init_state();
void statemachine_set_state();
void statemachine_standard_state();
void statemachine_controlled_state();
void statemachine_process ();

typedef enum{
	STANDBY_STATE = 0,
	INIT_STATE = 1,
	STANDARD_STATE = 2,
	SET_STATE = 3,
	CONTROLLED_STATE = 4,
} STATEMACHINE_STATES;



#endif /* INC_STATE_MACHINE_H_ */
