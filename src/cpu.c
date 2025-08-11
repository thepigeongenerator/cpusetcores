#include "cpu.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

const char *const cpu_path = "/sys/devices/system/cpu/cpu%i/online";

bool getcpu(uint32_t id) {
	// get the file path
	char path[64];                    // contains the file path (max length is 64 due to the path and a bunch of extra wiggle room)
	snprintf(path, 64, cpu_path, id); // writes the path using the id

	// if the file doesn't exist; return true
	if (access(path, R_OK) != 0)
		return true;

	// read a character from the file, store in state
	char state = '\0';
	FILE *f = fopen(path, "r");
	fread(&state, 1, 1, f);
	fclose(f);

	// return whether state is truthy
	return !!(state - 0x30);
}

void setcpu(uint32_t id, bool state) {
	char path[64];
	snprintf(path, 64, cpu_path, id);

	char s = 0x30 + (char)state; // convert the state to a character

	// write the state to the file (creates file if it doesn't exist)
	FILE *f = fopen(path, "w");
	fwrite(&s, 1, 1, f);
	fclose(f);
}
