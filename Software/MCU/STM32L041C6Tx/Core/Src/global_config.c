#include "global_config.h"
#include "stdlib.h"


const uint16_t lptim_period = (LPTIM_FREQ * LPTIM_INTERRUPT_INTERVAL) -1;
const uint16_t min_pwm_val = MIN_PWM_VAL;

uint8_t listen_group_addresses[MAX_GROUP_LISTEN_ADDRESSES][3];

const float setpoint_humidity = SETPOINT_HUMIDITY;
const float max_humidity_allowed = MAX_HUMIDITY_ALLOWED;
float actual_humidity = 0;

const uint16_t max_error_integral = MAX_ERROR_INTEGRAL;
const uint16_t min_error_integral = MIN_ERROR_INTEGRAL;

const uint8_t switch_direction_interval = FAN_SWITCH_DIRECTION_INTERVAL / 10;
const uint8_t spin_out_time = FAN_SPIN_OUT_TIME / 10;
