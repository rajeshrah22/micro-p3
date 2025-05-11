#include <stdint.h>
#include <stdfix.h>
#include "pid.h"

/* Assume k = k_p = k_i = k_d. No time to calibrate */
#define	K_PID		((long _Accum)0.01K)
#define D_T		((long _Accum)0.1K)

static void pid_update(uint16_t in_a_x);

uint8_t buffer[MPU_DATA_BUF_SIZE];
long _Accum c_err_dt = 0;
long _Accum prev_err = 0;
long _Accum out = 0;


long _Accum pid_controller() {
	uint16_t res = 0;

	// read ACCELL X AXIS
	i2c1_read_MPU6050(IC_MPU6050_AD0_VDD_ADDRESS, 59, buffer, 2);
	res |= (buffer[0] << 8) | buffer[1];
	pid_update(res);

	return out;
}

/*
 *      figure out how to read a_x
 *	error = 0 - a_x
 *	goal: a_x = 0
 */
static void pid_update(uint16_t in_a_x) {
	uint8_t error;
	long _Accum p;
	long _Accum i;
	long _Accum d;
	long _Accum a_x = (long _Accum)in_a_x;

	error = 0K - a_x;
	c_err_dt += error * D_T;

	p = K_PID * error;
	i = K_PID * c_err_dt;
	d = K_PID * (prev_err - error);

	out = p + i + d;

	prev_err = error;
}
