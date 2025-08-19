#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "cpu.h"
#include "error.h"
#include "opts.h"

/* sets CPU `id` to `nstate`, if it hasn't been set yet.
 * If `opt` contains `OPT_SET_ALL`, this check is ignored.
 * Returns `0` upon success, `1` upon failure. */
static int setcpu(uint id, int nstate, u8 opts) {
	bool nsetall = !(opts & OPT_SET_ALL);                               // if SET_ALL has not been set
	int cstate = nsetall && cpu_getenabled(id);                         // get state if nsetall=1
	return (nsetall && cstate == nstate) || cpu_setenabled(id, nstate); // set if cstate matches nstate, unless nsetall=1
}

static inline void print_cpu_count(uint mcpus) {
	printf("%i/%i cpus enabled.\n", get_nprocs(), mcpus); // get the number of available processors
}

int main(int argc, char **argv) {
	if (geteuid() != 0) fatal("must be executed as the root user!");

	int mcpus, ncpus;                         // the number of CPUs to activate
	u8 opts = getoptions(argc, argv, &ncpus); // the options to use
	mcpus = get_nprocs_conf();                // the max number of CPUs that are available

	if (opts & OPT_LIST_CORES && ncpus < 0) {
		print_cpu_count(mcpus);
		return 0;
	}

	if (opts & OPT_INVERT)
		ncpus = mcpus - ncpus;

	// validate arguments
	if (ncpus > mcpus) fatal("%i exeeds the maximum numbers of cpus available (%i)", ncpus, mcpus);
	if (ncpus < 1) fatal("may not keep less than 1 cpu enabled, requested to enable %i", ncpus);

	const char *const cpu_set_log = "set cpu %i to %hi\n";
	for (int id = 1; id < mcpus; id++) { // start at CPU 1, as CPU 0 is not writeable
		// whilst the id is less then the amount of cpus to enable
		if (id < ncpus) {
			// enable the cpu & print if it was actually set (if -v was given)
			if (!setcpu(id, true, opts) && (opts & OPT_VERBOSE))
				printf(cpu_set_log, id, 1);
			continue;
		}

		// disable the cpu & print if it was actually set (if -v was given)
		if (!setcpu(id, false, opts) && (opts & OPT_VERBOSE)) {
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
