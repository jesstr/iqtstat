#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "fpid.h"


/* Wrap function for matlab PID simulation */
double matlab_fpid_controller(double reference, double measured, double kP, double kI, double kD)
{
    static fpid_data_t pid;
    static bool noinit = true;

    if (noinit) {
        fpid_init(kP, kI, kD, &pid);
        noinit = false;
    }

    return fpid_controller(reference, measured, &pid);
}