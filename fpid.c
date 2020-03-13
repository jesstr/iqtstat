#include <stdint.h>
#include <stdio.h>

#include "fpid.h"
#include "options.h"

#define DBG if(0)

#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))


/* PID init */
void fpid_init(double kP, double kI, double kD, fpid_data_t *pid)
{
    pid->kP = kP;
    pid->kI = kI;
    pid->kD = kD;

    pid->sum_error = 0;
    pid->last_value = 0;

    pid->max_error = PID_ERROR_LIMIT;
    pid->max_sumerror = PID_SUMERROR_LIMIT;
}

/* PID algorithm single iteration */
double fpid_controller(double reference, double measured, fpid_data_t *pid)
{
    double p_term, d_term, i_term;
    double error = reference - measured;

    /* Proportional (P) component calculation */
    if (error > pid->max_error) {
        p_term = PID_ERROR_LIMIT;
    } else if (error < -pid->max_error) {
        p_term = -(PID_ERROR_LIMIT);
    } else {
        p_term = pid->kP * error;
    }

    /* Integrative (I) component calculation */
    double temp = pid->sum_error + error;
    if (temp > pid->max_sumerror) {
        i_term = PID_SUMERROR_LIMIT;
        pid->sum_error = pid->max_sumerror;
    } else if (temp < -pid->max_sumerror) {
        i_term = -(PID_SUMERROR_LIMIT);
        pid->sum_error = -pid->max_sumerror;
    } else {
        pid->sum_error = temp;
        i_term = pid->kI * pid->sum_error;
    }

    /* Derivative (D) component calculation */
    d_term = pid->kD * (pid->last_value - measured);

    pid->last_value = measured;

    double ret = (p_term + i_term + d_term);

    DBG {
        printf("[fpid] e = %.3g\r\n", error);
        printf("[fpid] p = %.3g\r\n", p_term);
        printf("[fpid] i = %.3g\r\n", i_term);
        printf("[fpid] d = %.3g\r\n", d_term);
        printf("[fpid] r = %.3g\r\n", ret);
    }

    /* Apply PID input restrictions */
    ret = constrain(ret, PID_MIN_INPUT, PID_MAX_INPUT);

    return ret;
}

/* Reset PID */
void fpid_reset(fpid_data_t *pid)
{
    pid->sum_error = 0;
}