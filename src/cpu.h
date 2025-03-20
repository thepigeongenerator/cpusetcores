#pragma once
#include <stdbool.h>
#include <stdint.h>

bool getcore(uint32_t);       // gets the state of core (id)
void setcore(uint32_t, bool); // sets the state of core (id)
