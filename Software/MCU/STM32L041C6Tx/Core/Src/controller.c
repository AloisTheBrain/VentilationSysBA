#include "controller.h"
#include "tim.h"
#include "lptim.h"
#include "gpio.h"
#include "global_config.h"



int dynamic_error[21] = {0,-2,-4,-6,-8,-10,-12,-14,-16,-18,-20,-22,-24,-26,-28,-30,-32,-34,-36,-38,-40};


controller_param_t pi_param = {
		.kp = KP,
		.ki = KI * DT_SAMPLE_TIME,
		.kd = KD / DT_SAMPLE_TIME,
};


uint16_t pi_controller(uint8_t process_variable)
{

	float error_integral = 0;
	//uint16_t bias = min_pwm_val;
	float error = setpoint_humidity - process_variable;
	error_integral += error;

	//Integralfehler begrenzen (Anti-Windup)
	if(error_integral > max_error_integral){
		error_integral = max_error_integral;
	}
	else if(error_integral < min_error_integral ){
		error_integral = min_error_integral;
	}

	uint16_t pi_output = ((pi_param.kp * error) + (pi_param.ki * error_integral)) * (-1);
	//pi_output += bias;
	//Stellgröße (PWM) begrenzen
	if(pi_output > htim2.Init.Period){
		pi_output = htim2.Init.Period;
	}
	else if(pi_output < min_pwm_val){
		pi_output = min_pwm_val;
	}
	return pi_output;
}



void adjust_pwm_value(uint16_t new_duty){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, new_duty);
	//falls weitere Lüfter auf geregelten wert anspringen sollen
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, new_duty);
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, new_duty);
}

void start_all_timers(void){
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

void stop_all_timers(void){
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_2);
	HAL_LPTIM_Counter_Stop_IT(&hlptim1);
}

void reset_all_pwm(void){
	set_all_pwm(0);
}


void set_pwm_not_controlgroup(uint16_t TIM2_CH4, uint16_t TIM21_CH1, uint16_t TIM21_CH2, uint16_t TIM22_CH1, uint16_t TIM22_CH2){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, TIM2_CH4);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, TIM21_CH1);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, TIM21_CH2);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, TIM22_CH1);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_2, TIM22_CH2);
}

void set_all_pwm(uint16_t pwm_value){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, pwm_value);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_2, pwm_value);
}

void switch_directions_not_controlgroup(void){
	set_pwm_not_controlgroup(0, 0, 0, 0, 0);
	HAL_Delay(1000);
	toggle_gpio_not_controlgroup();
}

void switch_all_directions(void){
	reset_all_pwm();
	HAL_Delay(1000);
	toggle_all_gpio();
}

void toggle_all_gpio(void){
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);		//Lüfter 1 &htim2 , ersten drei sind in der controlgroup
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);		//Lüfter 5 &htim21
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);		//Lüfter 7 &htim22
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
}

void toggle_gpio_not_controlgroup(void){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
}



