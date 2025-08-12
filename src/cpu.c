#include "cpu.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

const char cpu_path[] = "/sys/devices/system/cpu/cpu%i/online";

int cpu_getenabled(uint id) {
	const size_t pathlen = sizeof(cpu_path) + (uint)(log10(-1u) + 1);
	char path[pathlen];
	snprintf(path, pathlen, cpu_path, id);

	if (access(path, R_OK) != 0)
		return 1;

	char state = '\0';
	FILE *f = fopen(path, "r");
	assert(f);
	fread(&state, 1, 1, f);
	fclose(f);

	return state != '0';
}

int cpu_setenabled(uint id, int state) {
	const size_t pathlen = sizeof(cpu_path) + (uint)(log10(-1u) + 1);
	char path[pathlen];
	snprintf(path, pathlen, cpu_path, id);

	char s = 0x30 + (state & 1);

	if (access(path, W_OK) != 0)
		return 1;

	FILE *f = fopen(path, "w");
	assert(f);
	fwrite(&s, 1, 1, f);
	fclose(f);
	return 0;
}
