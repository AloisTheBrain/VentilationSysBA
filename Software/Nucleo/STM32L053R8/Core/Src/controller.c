#include "controller.h"
#include "tim.h"

#include "global_config.h"



int dynamic_error[21] = {0,-2,-4,-6,-8,-10,-12,-14,-16,-18,-20,-22,-24,-26,-28,-30,-32,-34,-36,-38,-40};



float error = 0;
float error_integral = 0;
float pi_output = 0;



int pulse_width1 = 0;
int pulse_width2 = 0;
int pulse_width3 = 0;
int pulse_width4 = 0;

controller_param_t pi_param = {
		.kp = KP,
		.ki = KI,
		.kd = KD,
};







//float pid_controller(float soll_wert, float ist_wert, pid.Kp, pid.Ki, pid.Kd);

float pi_controller(float ist_wert)
{




	error = set_humidity - ist_wert;
	error_integral += error;

	//Integralfehler begrenzen (Anti-Windup)
	if(error_integral > max_error_integral){
		error_integral = max_error_integral;
	}
	else if(error_integral < min_error_integral ){
		error_integral = min_error_integral;
	}

	pi_output = ((pi_param.kp * error) + (pi_param.ki * error_integral)) * (-1);

	//Stellgröße (PWM) begrenzen
	if(pi_output > htim2.Init.Period){
		pi_output = htim2.Init.Period;
	}
	else if(pi_output < min_pwm_val){
		pi_output = min_pwm_val;
	}
	return pi_output;
}
/*
float pid_controller(float soll_wert, float ist_wert, pid.Kp, pid.Ki, pid.Kd, float max_pwm, float *out1)
{
	float error = 0;
	float output = 0;

	error = soll_wert - ist_wert;
	error_integral += error;
	if(error_integral  > max_pwm) { error_integreal = max_pwm};
	else if(error_integral < 0) {error_integreal = 0};
	error_derivative = error - error_previous;
	error_previous = error;
	output = (Kp*error) + (Ki*error_integral) + (Kd*error_derivative);
	if(output > max_pwm){ output = max_pwm};
	else if (output < 0) {output = 0};
	else *out = output;
}*/


// Pro Lüftergruppe 1 Regler
// Adjust pwm funktion für alle


void adjust_PWM()
{
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, &out1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse_width1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pulse_width2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pulse_width2);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, pulse_width3);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, pulse_width3);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, pulse_width4);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_2, pulse_width4);
}



