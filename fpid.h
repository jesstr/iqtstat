#ifndef FPID_H
#define FPID_H

#include "stdint.h"


typedef struct {
    double kP;
    double kI;
    double kD;
    double last_value;
    double sum_error;
    double max_error;
    double max_sumerror;
} fpid_data_t;


void fpid_init(double kP, double kI, double kD, fpid_data_t *pid);
double fpid_controller(double reference, double measured, fpid_data_t *pid);
void fpid_reset(fpid_data_t *pid);


#endif