
#include "state_machine.h"
#include "tim.h"

int state = INIT_STATE;
float humidity = 0;
float max_humidity_allowed;

void Start_Timers();
void Stop_Timers();
void Set_All_PWM_To_Zero();
void Set_PWM(int TIM2_CH1, int TIM2_CH2, int TIM2_CH3, int TIM2_CH4, int TIM21_CH1, int TIM21_CH2, int TIM22_CH1, int TIM22_CH2);
void Switch_Direction();

void StateMachine_process ()
{
	switch(state)
	{

	case STANDBY_STATE:
		break;
	case INIT_STATE:
		Stop_Timers();
		state = STANDARD_STATE;
		break;
	case STANDARD_STATE:
		Start_Timers();
		if(humidity >= max_humidity_allowed)
		{
			state = CONTROLLED_STATE;
		}
		break;
	case CONTROLLED_STATE:
		//controller()
		break;
	case BATHROOM_STATE:
		break;
	default:
		break;
	}
}

void Start_Timers()
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_2);
}

void Set_All_PWM_To_Zero()
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_2, 0);
}

void Stop_Timers()
{
	Set_All_PWM_To_Zero();
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim21, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim22, TIM_CHANNEL_2);
}

void Set_PWM(int TIM2_CH1, int TIM2_CH2, int TIM2_CH3, int TIM2_CH4, int TIM21_CH1, int TIM21_CH2, int TIM22_CH1, int TIM22_CH2)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH3);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM2_CH4);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM21_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM21_CH2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM22_CH1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, TIM22_CH2);

}

void Switch_Direction()
{
	Set_All_PWM_To_Zero();
	//HAL_GPIO_TogglePin(GPIOx, GPIO_Pin)

}
