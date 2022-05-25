
#ifndef INC_GLOBAL_CONFIG_H_
#define INC_GLOBAL_CONFIG_H_

#include "stdint.h"
#include "stdbool.h"

#define LPTIM_FREQ 298 					  // lptim_freq = (lptim_clock/prescaler) = 0.037 MHz / 124 = 298 Hz  //Ändern nur über CUbeMX oder lptim.c
#define SWITCH_DIRECTION_INTERVAL 60     // Lütferrichtungswechsel in sekunden


#define KNX_TELEGRAM_MAX_LENGTH 23

#define PWM_FREQ 8000
#define MIN_PWM_VAL 1000

#define KP 1
#define KI 100
#define KD 0
#define DT_SAMPLE_TIME 0.02

#define SETPOINT_HUMIDITY 50

#define MAX_ERROR_INTEGRAL 1000
#define MIN_ERROR_INTEGRAL -1000

#define MAX_GROUP_LISTEN_ADDRESSES 10	//für erzeugung von array, kann beliebig erhöht werden



extern uint8_t listen_group_addresses[MAX_GROUP_LISTEN_ADDRESSES][3];

extern const uint16_t lptim_period;

extern const float setpoint_humidity;
extern float actual_humidity;



extern const uint16_t min_pwm_val;




extern bool flag_lptim_interrupt;

extern bool flag_controlbyte_receive_started;
extern bool flag_address_receive_started;
extern bool flag_payload_receive_started;
extern bool flag_checksum_receive_started;

extern bool flag_data_processed;
extern bool flag_uart_reception_complete;

extern bool flag_entered_uart_callback;



extern const uint16_t max_error_integral;
extern const uint16_t min_error_integral;

typedef struct{
	uint8_t kp;
	uint8_t ki;
	uint8_t kd;
	uint8_t dt;
} controller_param_t;

typedef enum{
	FLAG_FALSE = 0,
	FLAG_TRUE  = 1,
} FLAG_STATUS;





#endif /* INC_GLOBAL_CONFIG_H_ */
