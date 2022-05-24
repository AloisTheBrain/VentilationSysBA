
#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_


void start_timers();
void stop_timers();

void set_pwm(int TIM2_CH1, int TIM2_CH2, int TIM2_CH3, int TIM2_CH4, int TIM21_CH1, int TIM21_CH2, int TIM22_CH1, int TIM22_CH2);
void switch_direction();
void reset_all_pwm();
void toggle_all_gpio();

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
