#include "controller.h"
#include "tim.h"

controller_param pi;
controller_param pid = {0, 0, 0};

float Kp = 0;
float Ki = 0;
float Kd = 0;
int pulse_width1 = 0;
int pulse_width2 = 0;
int pulse_width3 = 0;
int pulse_width4 = 0;

float pi_controller(float soll_wert, float ist_wert, float Kp, float Ki, float Kd, float max_pwm);
//float pid_controller(float soll_wert, float ist_wert, pid.Kp, pid.Ki, pid.Kd);

float pi_controller(float soll_wert, float ist_wert, float Kp, float Ki, float Kd, float max_pwm)
{
	float error = 0;
	float error_integral = 0;
	float error_previous = 0;
	float output = 0;

	error = soll_wert - ist_wert;
	error_integral += error;

	error_previous = error;
	output = (Kp*error) + (Ki*error_integral);
	if(output = 0){};
	return output;
}
/*
float pid_controller(float soll_wert, float ist_wert, pid.Kp, pid.Ki, pid.Kd, float max_pwm, float *out1)
{
	float error = 0;
	float output = 0;

	error = soll_wert - ist_wert;
	error_integral += error;
	if(error_integral > max_pwm) { error_integreal = max_pwm};
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


void switch_direction()
{
	//in task mit höchster priorität und 30s timeout
	//pi_controller mit ist wert auf null und dann gpio toggle


}
