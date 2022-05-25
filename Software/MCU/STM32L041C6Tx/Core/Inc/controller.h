#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "stdint.h"


uint16_t pi_controller(uint8_t process_variable);
void start_timers();
void stop_timers();

void set_pwm(int TIM2_CH1, int TIM2_CH2, int TIM2_CH3, int TIM2_CH4, int TIM21_CH1, int TIM21_CH2, int TIM22_CH1, int TIM22_CH2);
void adjust_pwm_value(uint16_t new_duty);
void switch_direction();
void reset_all_pwm();
void toggle_all_gpio();



#endif
