#include "global_config.h"
#include "stdlib.h"





const uint16_t lptim_period = LPTIM_FREQ * SWITCH_DIRECTION_INTERVAL;



const uint16_t min_pwm_val = MIN_PWM_VAL;


bool flag_lptim_interrupt = FLAG_FALSE;
bool flag_controlbyte_receive_started = FLAG_FALSE;
bool flag_address_receive_started = FLAG_FALSE;
bool flag_payload_receive_started = FLAG_FALSE;
bool flag_checksum_receive_started = FLAG_FALSE;

bool flag_data_processed = FLAG_TRUE;
bool flag_uart_reception_complete = FLAG_FALSE;

bool flag_entered_uart_callback = FLAG_FALSE;



uint8_t listen_group_addresses[MAX_GROUP_LISTEN_ADDRESSES][3];

const float setpoint_humidity = SETPOINT_HUMIDITY;
float actual_humidity = 0;

const uint16_t max_error_integral = MAX_ERROR_INTEGRAL;
const uint16_t min_error_integral = MIN_ERROR_INTEGRAL;
