#include <stdint.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "cpu.h"
#include "error.h"
#include "opts.h"

static inline bool cpu_setter(uint32_t id, bool nstate, uint8_t opts) {
	bool cstate = !nstate;
	if (!(opts & OPT_SET_ALL))
		cstate = getcpu(id);

	if (cstate != nstate) {
		setcpu(id, nstate);
		return true;
	}
	return false;
}

static inline void print_cpu_count(int32_t mcpus) {
	printf("%i/%i cpus enabled.\n", get_nprocs(), mcpus); // get the number of available processors
}

int32_t main(int32_t argc, char** argv) {
	if (geteuid() != 0) fatal("must be executed as the root user!");

	int32_t ncpus;                                 // the number of CPUs to activate
	uint8_t opts = getoptions(argc, argv, &ncpus); // the options to use
	int32_t mcpus = get_nprocs_conf();             // the max number of CPUs that are available

	if (opts & OPT_LIST_CORES && ncpus < 0) {
		print_cpu_count(mcpus);
		return 0;
	}

	if (opts & OPT_INVERT)
		ncpus = mcpus - ncpus;

	// validate arguments
	if (ncpus > mcpus) fatal("%i exeeds the maximum numbers of cpus available (%i)", ncpus, mcpus);
	if (ncpus < 1) fatal("may not keep less than 1 cpu enabled, requested to enable %i", ncpus);

	char const* const cpu_set_log = "set cpu %i to %hi\n";
	for (int32_t id = 1; id < mcpus; id++) { // start at CPU 1, as CPU 0 is not writeable
		// whilst the id is less then the amount of cpus to enable
		if (id < ncpus) {
			// enable the cpu & print if it was actually set (if -v was given)
			if (cpu_setter(id, true, opts) && (opts & OPT_VERBOSE))
				printf(cpu_set_log, id, 1);
			continue;
		}

		// disable the cpu & print if it was actually set (if -v was given)
		if (cpu_setter(id, false, opts) && (opts & OPT_VERBOSE)) {
			printf(cpu_set_log, id, 0);
			continue;
		}

		// break if OPT_SET_ALL is not set (assuming the rest of the CPUs are already off)
		if (!(opts & OPT_SET_ALL))
			break;
	}

	if ((opts & OPT_LIST_CORES))
		print_cpu_count(mcpus);

	return 0;
}
