#include "global_config.h"
#include "statemachine_process.h"
#include "statemachine_uart.h"
#include "stddef.h"



const uint16_t lptim_period = LPTIM_FREQ * SWITCH_DIRECTION_INTERVAL;

uint8_t rx_uart_buffer[UART_MSG_LENGTH];

const uint16_t min_pwm_val = MIN_PWM_VAL;


uint8_t statemachine_process_state = STANDBY_STATE;
uint8_t statemachine_uart_state = CONTROLBYTE_STATE;

bool flag_lptim_interrupt = FLAG_FALSE;
bool flag_controlbyte_receive_started = FLAG_FALSE;
bool flag_address_receive_started = FLAG_FALSE;
bool flag_payload_receive_started = FLAG_FALSE;
bool flag_checksum_receive_started = FLAG_FALSE;

bool flag_data_processed = FLAG_FALSE;
bool flag_uart_reception_complete = FLAG_FALSE;


uint8_t knx_controlbyte;
uint8_t knx_checksum_byte;
uint8_t buffer_knx_address[5];
uint8_t buffer_knx_header[6];
uint8_t *buffer_knx_payload = NULL;



const float set_humidity = SET_HUMIDITY;

const uint16_t max_error_integral = MAX_ERROR_INTEGRAL;
const uint16_t min_error_integral = MIN_ERROR_INTEGRAL;
