#pragma once
#include <stdint.h>

enum opt {
	OPT_LIST_CORES = 1, // option that lists the total amount of cores that are set
	OPT_SET_ALL = 2,    // option to set all cores, regardless of their assumed state
	OPT_VERBOSE = 4,    // print which cores are disabled/enabled
	OPT_INVERT = 8,     // 'num' now represents the amount of cores to disable
};

uint8_t getoptions(int32_t, char* const*, int32_t*);
