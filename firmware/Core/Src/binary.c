
#include "binary.h"

uint8_t binary_getBit8(uint8_t binary, uint8_t pos) {
	return ((binary >> pos) & 1) == 1 ? 1 : 0;
}

uint16_t binary_getBit16(uint16_t binary, uint8_t pos) {
	return ((binary >> pos) & 1) == 1 ? 1 : 0;
}

uint32_t binary_getBit32(uint32_t binary, uint8_t pos) {
	return ((binary >> pos) & 1) == 1 ? 1 : 0;
}