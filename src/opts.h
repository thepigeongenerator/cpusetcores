#pragma once
#include <stdint.h>

#include "util/intdef.h"

enum opt {
	OPT_LIST_CORES = 1, // option that lists the total amount of cores that are set
	OPT_SET_ALL = 2,    // option to set all cores, regardless of their assumed state
	OPT_VERBOSE = 4,    // print which cores are disabled/enabled
	OPT_INVERT = 8,     // 'num' now represents the amount of cores to disable
};

/* acquires the options given by the user through arguments.
 * returns the bitmap of these options.
 * `ncpus` is set to `-1`, or the amount of CPUs that the user requested to enable.*/
u8 getoptions(int argc, char *const *argv, int *ncpus);
