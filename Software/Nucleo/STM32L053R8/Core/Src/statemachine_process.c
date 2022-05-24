
#include "statemachine_process.h"
#include "tim.h"
#include "lptim.h"
#include "controller.h"
#include "global_config.h"
#include "usart.h"
#include "gpio.h"





float humidity = 0;
float max_humidity_allowed;

//statemachine_process_state = STANDBY_STATE;


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
	HAL_UART_Receive_IT(&huart2, &knx_controlbyte, sizeof(knx_controlbyte));
	flag_controlbyte_receive_started = FLAG_TRUE;
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
		statemachine_process_state = SET_STATE;
	}
}


void statemachine_controlled_state(){

	pi_controller(80);
	if(humidity <= max_humidity_allowed){
		statemachine_process_state = STANDARD_STATE;
	}
}


void start_timers(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_2);
	HAL_LPTIM_Counter_Start_IT(&hlptim1, lptim_period);
}

void reset_all_pwm(){
	set_pwm(0, 0, 0, 0, 0, 0, 0, 0);
}

void stop_timers(){
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_2);
}

void set_pwm(int TIM2_CH1, int TIM2_CH2, int TIM2_CH3, int TIM2_CH4, int TIM21_CH1, int TIM21_CH2, int TIM22_CH1, int TIM22_CH2){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH3);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH4);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM21_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM21_CH2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM22_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM22_CH2);
}

void switch_direction(){
	reset_all_pwm();
	HAL_Delay(1000);
	toggle_all_gpio();

}

void toggle_all_gpio(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
	//einer fehlt wegen nucleo board
}



