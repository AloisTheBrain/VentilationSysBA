#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <stdint.h>


typedef struct
{
	float Kp; //Proportional
	float Ki; //Integral
	float Kd; //Derivative
}controller_param;



#endif
