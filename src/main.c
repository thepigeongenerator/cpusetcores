#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "error.h"
#include "opts.h"

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
