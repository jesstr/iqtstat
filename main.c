#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "pid.h"
#include "fpid.h"
#include "options.h"

#define DBG if(0)


fpid_data_t fpid;

static double temperature = 0;
static double reference = 0;

const char USAGE[] = "Usage: ./demo kP kI kD ref iters\r\n\
    kP - Proportional (P) component\r\n\
    kI - Integrative (I) component\r\n\
    kD - Derivative (D) component\r\n\
    ref - Reference temperature value\r\n\
    iters - Max number of PID iterations\r\n";


/* Returns reference value */
double get_reference(void)
{
    return reference;
}

/* Returns measured value */
double get_measurement(void)
{
    return temperature;
}

/* Apply PID output as control input */
void apply_input(double value)
{
    /* Simple emulation of temperature change */
    /* TODO: replace me by something more real */
    temperature += value  - 0.2 * (temperature - get_reference());
}

/* Run PID emulation */
int main(int argc, char *argv[])
{
    int16_t inp = 0, count = 0;

    /* Get PID params from the args */
    if (argc != 6) {
        printf(USAGE);
        return 1;
    }

    double kP = strtof(argv[1], NULL);
    double kI = strtof(argv[2], NULL);
    double kD = strtof(argv[3], NULL);
    reference = strtof(argv[4], NULL);
    uint32_t iters = strtol(argv[5], NULL, 0);

    /* Init PID params */
    fpid_init(kP, kI, kD, &fpid);
    DBG printf("KP = %.3f, KI = %.3f, KD = %.3f, Iterations: %u\r\n", kP, kI, kD, iters);

    for (uint32_t i = 0; i < iters; i++) {
        double ref = get_reference();
        double meas = get_measurement();

        /* PID magic goes here */
        double input = fpid_controller(ref, meas, &fpid);

        /* Output data & control input */
        double err = ref - meas;
        printf("%f\t%f\t%f\r\n", meas, err, input);

        /* Apply control input */
        apply_input(input);

        /* Reset PID just for fun */
        if (i == 110) {
            fpid_reset(&fpid);
        }

        /* Stop emulation if temperature was stabilized enough */
        if (err == 0) {
            count++;
            if (count > STAB_TRESHOLD) {
                DBG printf("Done for: %u\r\n", i);
                break;
            }
        } else {
            count = 0;
        }
    }
}