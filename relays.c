#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "stime.h"
#include "options.h"


uint16_t get_relay_timeout()
{
    return RELAY_TIMEOUT;
}

static bool set_heat(bool state)
{
    static bool last_state;
    static time_t timestamp;
    uint16_t timeout = get_relay_timeout();

    time_t ticks = get_systick();
    if ((state != last_state && (ticks - timestamp) / 1000 > timeout) ||
            timestamp == 0) {
        last_state = state;
        timestamp = ticks;
    }

    return last_state;
}


static bool set_fan(bool state)
{
    static bool last_state;
    static time_t timestamp;
    uint16_t timeout = get_relay_timeout();

    time_t ticks = get_systick();
    if ((state != last_state && (ticks - timestamp) / 1000 > timeout) ||
            timestamp == 0) {
        last_state = state;
        timestamp = ticks;
    }

    return last_state;
}


void set_relays(double input, bool *heat, bool *fan)
{
    if (!heat || !fan) {
        return;
    }

    if (input > 0) {
        *heat = set_heat(true);
        *fan = set_fan(false);
    } else if (input < 0) {
        *heat = set_heat(false);
        *fan = set_fan(true);
    } else if (input == 0) {
        *fan = set_fan(false);
        *heat = set_heat(false);
    }
}