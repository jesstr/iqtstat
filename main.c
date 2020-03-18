#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fpid.h"
#include "tres.h"
#include "stime.h"
#include "relays.h"
#include "options.h"

#define DBG if(0)

typedef enum {
    ALG_PID,
    ALG_TRES
} alg_type_t;

typedef struct {
    float tmin;
    float tmin_hyst;
    float tmax;
    float tmax_hyst;
} settings_t;

static settings_t settings = {-5.0, 0.5, 5.0, 0.5};

static fpid_data_t fpid;

static double temperature = 0;
static double reference = 0;

const char USAGE[] = "Usage: ./demo type [kP kI kD ref] iters\r\n\
    type - {pid|tres} - Algorithm type\r\n\
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
    temperature += 0.01 * (rand() % 10) + 0.2 * value;
}

/* Run PID simulation */
uint32_t pid_simulation(uint32_t iters, fpid_data_t *pid)
{
    bool fan = false;
    bool heat = false;
    uint32_t count = 0;

    for (uint32_t i = 0; i < iters; i++) {
        double ref = get_reference();
        double meas = get_measurement();

        /* PID magic goes here */
        double input = fpid_controller(ref, meas, pid);

        /* Set heater & fan relays */
        set_relays(input, &heat, &fan);

        /* Output data for plots */
        double err = ref - meas;
        printf("%f\t%f\t%f\t%f\t%i\t%i\r\n", meas, err, ref, input, fan, heat);

        /* Apply control input */
        apply_input(input);

        /* Stop emulation if temperature was stabilized enough */
        if (err == 0) {
            count++;
            if (count > STAB_TRESHOLD) {
                return i;
            }
        } else {
            count = 0;
        }

        /* Emulate system time */
        set_systick(i * 1000);
    }

    return iters;
}

/* Run treshold simulation */
void tres_simulation(uint32_t iters)
{
    bool fan = false;
    bool heat = false;

    for (uint32_t i = 0; i < iters; i++) {
        double meas = get_measurement();
        float tmin = settings.tmin;
        float tmax = settings.tmax;
        float tmin_hyst = settings.tmin_hyst;
        float tmax_hyst = settings.tmax_hyst;

        int input = tres_controller(meas, tmax, tmax_hyst, tmin, tmin_hyst);

        /* Set heater & fan relays */
        set_relays(input, &heat, &fan);

        /* Output data for plots */
        printf("%f\t%f\t%f\t%i\t%i\t%i\r\n", meas, tmin, tmax, input, fan, heat);

        /* Apply control input */
        if (input > 0) {
            input = heat;
        } else if (input < 0) {
            input = -fan;
        }
        apply_input(input);

        /* Emulate system time */
        set_systick(i * 1000);
    }
}

/* Run simulation */
int main(int argc, char *argv[])
{
    double kP = 0, kI = 0, kD = 0;
    uint32_t iters = 0;
    alg_type_t type = ALG_PID;

    srand(time(NULL));

    if (strcmp(argv[1], "pid") == 0) {
        /* Get PID params from the args */
        if (argc != 7) {
            printf(USAGE);
            return 1;
        }

        type = ALG_PID;
        kP = strtof(argv[2], NULL);
        kI = strtof(argv[3], NULL);
        kD = strtof(argv[4], NULL);
        reference = strtof(argv[5], NULL);
        iters = strtol(argv[6], NULL, 0);

        /* Init PID params */
        fpid_init(kP, kI, kD, &fpid);
        DBG printf("KP = %.3f, KI = %.3f, KD = %.3f, Iterations: %u\r\n", kP, kI, kD, iters);
    }
    else if (strcmp(argv[1], "tres") == 0) {
        if (argc != 3) {
            printf(USAGE);
            return 1;
        }
        /* Get treshold params from the args */
        type = ALG_TRES;
        iters = strtol(argv[2], NULL, 0);
    } else {
        printf(USAGE);
        return 1;
    }

    /* Run simulation */
    if (type == ALG_PID) {
        /* PID */
        uint8_t n = pid_simulation(iters, &fpid);
        DBG printf("Done for: %u\r\n", n);
    } else {
        /* Tresholds */
        tres_simulation(iters);
    }
}