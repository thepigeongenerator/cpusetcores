#pragma once
#include <stdbool.h>
#include <stdint.h>

bool getcpu(uint32_t);       // gets the state of core (id)
void setcpu(uint32_t, bool); // sets the state of core (id)
