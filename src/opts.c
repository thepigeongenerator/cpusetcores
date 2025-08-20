#include "opts.h"

#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "util/intdef.h"

u8 getoptions(int argc, char *const *argv, int *ncpus) {
	*ncpus = -1;
	uint8_t opts = 0;
	char opt;

	while ((opt = getopt(argc, argv, "lavih")) != -1) {
		switch (opt) {
		case 'l': opts |= OPT_LIST_CORES; break;
		case 'a': opts |= OPT_SET_ALL; break;
		case 'v': opts |= OPT_VERBOSE | OPT_LIST_CORES; break;
		case 'i': opts |= OPT_INVERT; break;
		case '?':
			printf("specify -h for help\n");
			exit(EXIT_FAILURE);
		case 'h':
			printf(
				"%s: a tool for turning on/off CPU threads\n"
				"USAGE: %s [integer] [-l] [-a] [-v] [-i] [-h]\n"
				"ARGUMENTS:\n"
				"\t-l : List cores after executing the command. May be specified without integer.\n"
				"\t-a : Writes to all cores, regardless of their state.\n"
				"\t-v : Print the cores that are written. (implies -l)\n"
				"\t-i : Instead of the given number.\n"
				"\t-h : Shows this dialogue.\n",
				argv[0], argv[0]);
			exit(EXIT_SUCCESS);
		}
	}

	if (optind < argc) {
		char *end;
		char *num = argv[optind];
		*ncpus = strtol(num, &end, 0);
		if (errno || end == num || *end != '\0')
			fatal("failed to parse numeric value from string '%s'", num);
	} else if (!(opts & OPT_LIST_CORES))
		fatal("must execute with either a number or [-l]");

	return opts;
}
