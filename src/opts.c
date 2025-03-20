#include "opts.h"

#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"

uint8_t getoptions(int32_t argc, char* const* argv, int32_t* ncpus) {
	uint8_t opts = 0;
	char opt;

	char const* const msg = "-%c has already been set";
	while ((opt = getopt(argc, argv, "lavi")) != -1) {
		switch (opt) {
		case 'l':
			if (opts & OPT_LIST_CORES) fatal(msg, 'l');
			opts |= OPT_LIST_CORES;
			break;
		case 'a':
			if (opts & OPT_SET_ALL) fatal(msg, 'a');
			opts |= OPT_SET_ALL;
			break;
		case 'v':
			if (opts & OPT_VERBOSE) fatal(msg, 'v');
			opts |= OPT_VERBOSE | OPT_LIST_CORES;
			break;
		case 'i':
			if (opts & OPT_INVERT) fatal(msg, 'i');
			opts |= OPT_INVERT;
			break;
		case '?':
			fatal("usage: %s [number] [-l] [-a] [-v] [-i]", argv[0]);
		}
	}

	if (optind < argc) {
		char* num = argv[optind];
		*ncpus = atoi(num);
	} else if (opts & OPT_LIST_CORES) {
		*ncpus = -1;
	} else
		fatal("must execute with either a number or [-l]");

	return opts;
}
