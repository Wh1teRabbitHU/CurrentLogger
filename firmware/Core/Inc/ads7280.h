#ifndef ADS7280_H
#define ADS7280_H

#include "main.h"

#define ADS7280_SELECT_IN0				0b0000
#define ADS7280_SELECT_IN1				0b0001
#define ADS7280_WAKE_UP					0b1011
#define ADS7280_READ_CFR				0b1100
#define ADS7280_READ_DATA				0b1101
#define ADS7280_WRITE_CFR				0b1110
#define ADS7280_DEFAULT_MODE			0b1111

#define ADS7280_CFR_DEFAULT				0b111111111111

#define ADS7280_MANUAL_CHANNEL			0
#define ADS7280_AUTO_CHANNEL			1
#define ADS7280_CCLK_SRC_SCLK			0
#define ADS7280_CCLK_SRC_INTERNAL		1
#define ADS7280_AUTO_TRIGGER			0
#define ADS7280_MANUAL_TRIGGER			1
#define ADS7280_EOC_ACTIVE_HIGH			0
#define ADS7280_EOC_ACTIVE_LOW			1
#define ADS7280_USE_INT					0
#define ADS7280_USE_EOC					1
#define ADS7280_CHAIN_MODE_ON			0
#define ADS7280_CHAIN_MODE_OFF			1
#define ADS7280_AUTO_NAP_ON				0
#define ADS7280_AUTO_NAP_OFF			1
#define ADS7280_PWR_DOWN_ON				0
#define ADS7280_PWR_DOWN_OFF			1
#define ADS7280_DEEP_PWR_DOWN_ON		0
#define ADS7280_DEEP_PWR_DOWN_OFF		1
#define ADS7280_TAG_DISABLED			0
#define ADS7280_TAG_ENABLED				1
#define ADS7280_RESET					0

#define ADS7280_CHANNEL_TYPE_POS		11
#define ADS7280_CCLK_SOURCE_POS			10
#define ADS7280_TRIGGER_MODE_POS		9
#define ADS7280_EOC_ACTIVE_MODE_POS		7
#define ADS7280_EOC_TYPE_POS			6
#define ADS7280_CHAIN_MODE_POS			5
#define ADS7280_AUTO_NAP_POS			4
#define ADS7280_PWR_DOWN_POS			3
#define ADS7280_DEEP_PWR_DOWN_POS		2
#define ADS7280_TAG_ENABLE_POS			1
#define ADS7280_SYSTEM_RESET_POS		0

#define ADS7280_UNIT_MV					2500.0/16384.0 // 0,152587

typedef struct ADS7280_config {
	uint8_t channelType;
	uint8_t cclkSource;
	uint8_t triggerMode;
	uint8_t eocActiveMode;
	uint8_t eocType;
	uint8_t chainMode;
	uint8_t autoNap;
	uint8_t tagEnable;
} ADS7280_config;

void ADS7280_selectInput0();
void ADS7280_selectInput1();
void ADS7280_setDefault();
void ADS7280_readConfig(ADS7280_config* config);
void ADS7280_writeConfig(ADS7280_config* config);
uint16_t ADS7280_readCFR();
void ADS7280_writeCFR(uint16_t cfr);

uint16_t ADS7280_readData();
float ADS7280_readVoltage();

#endif