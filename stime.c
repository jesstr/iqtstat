#include <time.h>
#include "options.h"

static time_t ticks = 0;


time_t get_systick()
{
    return ticks;
}

void set_systick(time_t ms)
{
    ticks = ms;
}