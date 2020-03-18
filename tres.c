#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef enum {
    hyst_idle,
    hyst_up,
    hyst_down
} hyst_state_t;


int8_t tres_controller(double measured, float tmax, float tmax_hyst,
    float tmin, float tmin_hyst)
{
    static hyst_state_t hyst_state = hyst_idle;
    static int8_t state = 0;

    if (measured >= tmax) {
        if ((hyst_state == hyst_idle) || (hyst_state == hyst_down)) {
            hyst_state = hyst_up;
            /* Cooling */
            state = -1;
        }
    } else if (measured <= tmin) {
        if ((hyst_state == hyst_idle) || (hyst_state == hyst_up)) {
            hyst_state = hyst_down;
            /* Heating */
            state = 1;
        }
    } else if ((measured < (tmax - tmax_hyst)) && (measured > (tmin + tmin_hyst))) {
        if ((hyst_state == hyst_up) || (hyst_state == hyst_down)) {
            hyst_state = hyst_idle;
            /* All is off */
            state = 0;
        }
    } else {
        /* Nothing to do */
    }

    return state;
}