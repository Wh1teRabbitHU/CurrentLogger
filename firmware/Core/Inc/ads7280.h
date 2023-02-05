#ifndef ADS7280_H
#define ADS7280_H

#include "main.h"

#define ADS7280_SELECT_IN0		0b0000
#define ADS7280_SELECT_IN1		0b0001
#define ADS7280_WAKE_UP			0b1011
#define ADS7280_READ_CFR		0b1100
#define ADS7280_READ_DATA		0b1101
#define ADS7280_WRITE_CFR		0b1110
#define ADS7280_DEFAULT_MODE	0b1111

void ADS7280_selectInput0();
void ADS7280_selectInput1();
uint16_t ADS7280_readCFR();
void ADS7280_writeCFR(uint16_t cfr);
uint16_t ADS7280_readData();
void ADS7280_setDefault();

#endif