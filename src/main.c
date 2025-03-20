#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <sys/sysinfo.h>
#include <unistd.h>

enum opt {
	OPT_LIST_CORES = 1, // option that lists the total amount of cores that are set
	OPT_SET_ALL = 2,    // option to set all cores, regardless of their assumed state
	OPT_VERBOSE = 4,    // print which cores are disabled/enabled
	OPT_INVERT = 8,     // 'num' now represents the amount of cores to disable
};

char const* const cpu_path = "/sys/devices/system/cpu/cpu%i/online";

noreturn void fatal(char const* fmt, ...) {
	char buf[128];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 128, fmt, args);
	va_end(args);

	fprintf(stderr, "\033[91mE: %s\033[0m\n", buf);
	exit(1);
}

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
			opts |= OPT_VERBOSE;
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
	} else fatal("you must provide a number!");

	return opts;
}

bool getcore(uint32_t id) {
	// get the file path
	char path[64];                    // contains the file path (max length is 64 due to the path and a bunch of extra wiggle room)
	snprintf(path, 64, cpu_path, id); // writes the path using the id

	// if the file doesn't exist; return true
	if (access(path, R_OK) != 0)
		return true;

	// read a character from the file, store in state
	char state = '\0';
	FILE* f = fopen(path, "r");
	fread(&state, 1, 1, f);
	fclose(f);

	// return whether state is truthy
	return !!state;
}

void setcore(uint32_t id, bool state) {
	char path[64];
	snprintf(path, 64, cpu_path, id);

	char s = 0x30 + (char)state; // convert the state to a character

	// write the state to the file (creates file if it doesn't exist)
	FILE* f = fopen(path, "w");
	fwrite(&s, 1, 1, f);
	fclose(f);
}

int32_t main(int32_t argc, char** argv) {
	if (geteuid() != 0) fatal("must be executed as the root user!");

	int32_t ncpus;                                 // the number of CPUs to activate
	uint8_t opts = getoptions(argc, argv, &ncpus); // the options to use
	int32_t mcpus = get_nprocs_conf();             // the max number of CPUs that are available

	if (opts & OPT_INVERT)
		ncpus = mcpus - ncpus;

	if (ncpus > mcpus) fatal("%i exeeds the maximum numbers of cpus available (%i)", ncpus, mcpus);
	if (ncpus < 1) fatal("may not keep less than 1 cpu enabled, requested to enable %i", ncpus);
	return 0;
}
