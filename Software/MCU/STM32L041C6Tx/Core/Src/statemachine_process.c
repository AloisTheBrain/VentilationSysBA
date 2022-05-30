
#include "statemachine_process.h"
#include "tim.h"
#include "lptim.h"
#include "controller.h"
#include "global_config.h"
#include "usart.h"
#include "gpio.h"
#include "knx_receive_telegram.h"





float humidity = 0;
float max_humidity_allowed = 50;

uint8_t statemachine_process_state = STANDBY_STATE;


void statemachine_process ()
{
	switch(statemachine_process_state)
	{

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



void statemachine_standby_state(){
	reset_all_pwm();
	stop_timers();
	statemachine_process_state = INIT_STATE;
}


void statemachine_init_state(){
	flag_controlbyte_receive_started = FLAG_TRUE;
	HAL_UART_Receive_IT(&huart2, knx_controlbytes, sizeof(knx_controlbytes));

	start_timers();
	reset_all_pwm();
	statemachine_process_state = SET_STATE;
}


void statemachine_set_state(){
	set_pwm(min_pwm_val, min_pwm_val, min_pwm_val, min_pwm_val, min_pwm_val, min_pwm_val, min_pwm_val, min_pwm_val);
	statemachine_process_state = STANDARD_STATE;
}


void statemachine_standard_state(){
	if(humidity >= max_humidity_allowed){
		statemachine_process_state = CONTROLLED_STATE;
	}
	else if(flag_lptim_interrupt == FLAG_TRUE){
		switch_direction();
		flag_lptim_interrupt = FLAG_FALSE;
		HAL_LPTIM_Counter_Start_IT(&hlptim1, lptim_period);
		statemachine_process_state = SET_STATE;
	}
}


void statemachine_controlled_state(){

	uint16_t new_dutycycle = pi_controller(actual_humidity);
	adjust_pwm_value(new_dutycycle);
	if(flag_lptim_interrupt == FLAG_TRUE){
		switch_direction();
		flag_lptim_interrupt = FLAG_FALSE;
		HAL_LPTIM_Counter_Start_IT(&hlptim1, lptim_period);

	}
	if(humidity <= max_humidity_allowed){
		statemachine_process_state = STANDARD_STATE;
	}

}




