
#include "binary.h"

uint8_t binary_getBit(uint8_t binary, uint8_t pos) {
	return ((binary >> pos) & 1) == 1 ? 1 : 0;
}