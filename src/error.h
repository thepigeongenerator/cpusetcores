#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stdnoreturn.h>

// prints an error message and aborts execution
noreturn void fatal(char const*, ...);
