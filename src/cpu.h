#pragma once
#include "util/intdef.h"

/* gets the current state of a CPU thread,
 * returns a boolean value (`1` for enabled, `0` for disabled) */
int cpu_getenabled(uint id);

/* sets the current state of a CPU thread.
 * returns `0` upon success, `1` upon failure */
int cpu_setenabled(uint id, int state);
