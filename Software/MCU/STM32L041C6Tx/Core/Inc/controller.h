#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "stdint.h"


uint16_t pi_controller(float process_variable);
void start_all_timers();
void stop_all_timers();
void set_all_pwm(uint16_t pwm_value);
void set_pwm_not_controlgroup(uint16_t pwm_value);
void adjust_pwm_value(uint16_t new_duty);
void reset_all_pwm(void);
void toggle_all_gpios(void);
void toggle_gpios_not_controlgroup(void);
void reset_error_integral(void);
void reset_pwm_not_controlgroup(void);



#endif
