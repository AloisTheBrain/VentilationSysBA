
#ifndef INC_GLOBAL_CONFIG_H_
#define INC_GLOBAL_CONFIG_H_

#include "stdint.h"
#include "stdbool.h"

// lptim_freq = (lptim_clock/prescaler) = 0.037 MHz / 124 = 298 Hz  Ändern nur über CUbeMX oder lptim.c
#define LPTIM_FREQ 298

// Lptim Interrupt-Intervall in Sekunden
#define LPTIM_INTERRUPT_INTERVAL 10

//Lüfterrichtungswechsel-Intervall in Sekunden (nur vielfache von 10 möglich)
#define FAN_SWITCH_DIRECTION_INTERVAL 40

//Ausdrehzeit der Lüfter (nur vielfache von 10 möglich)
#define FAN_SPIN_OUT_TIME 20

//gewünschter Feuchtigkeitswert
#define SETPOINT_HUMIDITY 50

//maximal erlaubter Feuchtigkeitswert; Bei Erreichen des Wert wird in den geregelten Modus übergegangen
#define MAX_HUMIDITY_ALLOWED 70

//Anti-Windup-Grenzen (hier nur negative Grenze relevant)
#define MAX_ERROR_INTEGRAL 1000
#define MIN_ERROR_INTEGRAL -1000

//Maximale Anzahl an Adressen auf die reagiert wird; kann beliebig erhöht werden
#define MAX_GROUP_LISTEN_ADDRESSES 30

//#define PWM_FREQ 8000					//to do: soll cube generierten code überschreiben //Ändern derzeit nur über CubeMx oder tim.c
#define MIN_PWM_VAL 45

#define KNX_TELEGRAM_MAX_LENGTH 23

#define KP 1
#define KI 100
#define KD 0
#define DT_SAMPLE_TIME 0.02

extern const uint8_t switch_direction_interval;
extern const uint8_t spin_out_time;
extern uint8_t listen_group_addresses[MAX_GROUP_LISTEN_ADDRESSES][3];

extern const uint16_t min_pwm_val;
extern const uint16_t lptim_period;

extern const uint16_t max_error_integral;
extern const uint16_t min_error_integral;

extern const float setpoint_humidity;
extern const float max_humidity_allowed;
extern float actual_humidity;

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
