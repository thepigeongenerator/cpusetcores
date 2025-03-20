#include "error.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void fatal(char const* fmt, ...) {
	char buf[128];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 128, fmt, args);
	va_end(args);

	fprintf(stderr, "\033[91mE: %s\033[0m\n", buf);
	abort();
}
