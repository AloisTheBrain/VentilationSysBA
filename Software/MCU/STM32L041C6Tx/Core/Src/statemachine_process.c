
#include "statemachine_process.h"
#include "tim.h"
#include "lptim.h"
#include "controller.h"
#include "global_config.h"
#include "usart.h"
#include "gpio.h"
#include "knx_receive_telegram.h"


uint8_t statemachine_process_state = STANDBY_STATE;
bool flag_lptim_interrupt = FLAG_FALSE;


void statemachine_process ()
{
	switch(statemachine_process_state){

	case STANDBY_STATE:
		statemachine_standby_state();
		break;

	case INIT_STATE:
		statemachine_init_state();
		break;

	case SET_STATE:
		statemachine_set_state();
		break;

	case STANDARD_STATE:
		statemachine_standard_state();
		break;

	case CONTROLLED_STATE:
		statemachine_controlled_state();
		break;

	default:
		break;
	}
}



void statemachine_standby_state(void){
	reset_all_pwm();
	stop_all_timers();
	statemachine_process_state = INIT_STATE;
}


void statemachine_init_state(void){
	HAL_UART_Receive_IT(&huart2, &knx_controlbytes[0], sizeof(uint8_t));
	start_all_timers();
	reset_all_pwm();
	statemachine_process_state = SET_STATE;
}


void statemachine_set_state(void){
	set_all_pwm(min_pwm_val);
	statemachine_process_state = STANDARD_STATE;
}


void statemachine_standard_state(void){
	if(actual_humidity >= max_humidity_allowed){
		reset_error_integral();
		statemachine_process_state = CONTROLLED_STATE;
	}
	else if(flag_lptim_interrupt == FLAG_TRUE){
		switch_all_directions();
		flag_lptim_interrupt = FLAG_FALSE;
		HAL_LPTIM_Counter_Start_IT(&hlptim1, lptim_period);
		statemachine_process_state = SET_STATE;
	}
}


void statemachine_controlled_state(void){

	uint16_t new_dutycycle = pi_controller(actual_humidity);
	adjust_pwm_value(new_dutycycle);

	if(actual_humidity <= setpoint_humidity){
		statemachine_process_state = SET_STATE;
	}
	else if(flag_lptim_interrupt == FLAG_TRUE){
		switch_directions_not_controlgroup();
		flag_lptim_interrupt = FLAG_FALSE;
		HAL_LPTIM_Counter_Start_IT(&hlptim1, lptim_period);
	}
}

void set_flag_lptim_interrupt(void){
	flag_lptim_interrupt = FLAG_TRUE;
}



