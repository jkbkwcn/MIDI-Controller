#pragma once

#include <stdint.h>

typedef struct param param;

struct param {
	uint8_t value;
	uint8_t minValue;
	uint8_t maxValue;
};
