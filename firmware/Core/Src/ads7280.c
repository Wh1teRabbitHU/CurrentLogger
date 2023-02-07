#include "binary.h"
#include "ads7280.h"

uint16_t ADS7280_send4BitDataVariable(uint8_t data, uint8_t readLength) {
	uint16_t result = 0;

	HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 0);
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 0);

	uint8_t resultBit;

	for (uint8_t i = 0; i < 4; i++) {
		uint8_t dataBit = binary_getBit8(data, 3 - i);

		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 1);
		HAL_GPIO_WritePin(ADC_SDO_GPIO_Port, ADC_SDO_Pin, dataBit);
		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 0);

		resultBit = HAL_GPIO_ReadPin(ADC_SDI_GPIO_Port, ADC_SDI_Pin);
		result = (result << 1) | resultBit;
	}

	HAL_GPIO_WritePin(ADC_SDO_GPIO_Port, ADC_SDO_Pin, 0);

	for (uint8_t i = 0; i < readLength - 4; i++) {
		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 1);
		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 0);

		if (i != readLength - 5) {
			resultBit = HAL_GPIO_ReadPin(ADC_SDI_GPIO_Port, ADC_SDI_Pin);
			result = (result << 1) | resultBit;
		}
	}

	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 1);

	return result;
}

uint16_t ADS7280_send4BitData(uint8_t data) {
	return ADS7280_send4BitDataVariable(data, 16);
}

uint16_t ADS7280_send16BitData(uint16_t data) {
	uint16_t result = 0;

	HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 0);
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 0);

	for (uint8_t i = 0; i < 16; i++) {
		uint8_t dataBit = binary_getBit16(data, 15 - i);

		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 1);
		HAL_GPIO_WritePin(ADC_SDO_GPIO_Port, ADC_SDO_Pin, dataBit);
		HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, 0);
		uint8_t resultBit = HAL_GPIO_ReadPin(ADC_SDI_GPIO_Port, ADC_SDI_Pin);

		result = (result << 1) | resultBit;
	}

	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 1);

	return result;
}

void ADS7280_selectInput0() {
	ADS7280_send4BitDataVariable(ADS7280_SELECT_IN0, 0);
}

void ADS7280_selectInput1() {
	ADS7280_send4BitDataVariable(ADS7280_SELECT_IN1, 0);
}

void ADS7280_readConfig(ADS7280_config* config) {
	uint16_t result = ADS7280_readCFR();

	config->channelType = binary_getBit16(result, ADS7280_CHANNEL_TYPE_POS);
	config->cclkSource = binary_getBit16(result, ADS7280_CCLK_SOURCE_POS);
	config->triggerMode = binary_getBit16(result, ADS7280_TRIGGER_MODE_POS);
	config->eocActiveMode = binary_getBit16(result, ADS7280_EOC_ACTIVE_MODE_POS);
	config->eocType = binary_getBit16(result, ADS7280_EOC_TYPE_POS);
	config->chainMode = binary_getBit16(result, ADS7280_CHAIN_MODE_POS);
	config->autoNap = binary_getBit16(result, ADS7280_AUTO_NAP_POS);
	config->tagEnable = binary_getBit16(result, ADS7280_TAG_ENABLE_POS);
}

void ADS7280_writeConfig(ADS7280_config* config) {
	uint16_t cfr = 0;

	cfr = cfr | (config->channelType << ADS7280_CHANNEL_TYPE_POS);
	cfr = cfr | (config->cclkSource << ADS7280_CCLK_SOURCE_POS);
	cfr = cfr | (config->triggerMode << ADS7280_TRIGGER_MODE_POS);
	cfr = cfr | (config->eocActiveMode << ADS7280_EOC_ACTIVE_MODE_POS);
	cfr = cfr | (config->eocType << ADS7280_EOC_TYPE_POS);
	cfr = cfr | (config->chainMode << ADS7280_CHAIN_MODE_POS);
	cfr = cfr | (config->autoNap << ADS7280_AUTO_NAP_POS);
	cfr = cfr | (1 << ADS7280_PWR_DOWN_POS);
	cfr = cfr | (1 << ADS7280_DEEP_PWR_DOWN_POS);
	cfr = cfr | (config->tagEnable << ADS7280_TAG_ENABLE_POS);
	cfr = cfr | (1 << ADS7280_SYSTEM_RESET_POS);

	uint16_t data = (ADS7280_WRITE_CFR << 12) | cfr;
	ADS7280_send16BitData(data);
}

uint16_t ADS7280_readCFR() {
	uint16_t result = ADS7280_send4BitData(ADS7280_READ_CFR);

	return result;
}

void ADS7280_writeCFR(uint16_t cfr) {
	uint16_t data = (ADS7280_WRITE_CFR << 12) | cfr;
	ADS7280_send16BitData(data);
}

uint16_t ADS7280_readData() {
	HAL_GPIO_WritePin(ADC_CONVST_GPIO_Port, ADC_CONVST_Pin, 0);
	HAL_GPIO_WritePin(ADC_CONVST_GPIO_Port, ADC_CONVST_Pin, 1);

	while (HAL_GPIO_ReadPin(ADC_INT_GPIO_Port, ADC_INT_Pin) == 1);

	uint16_t result = ADS7280_send4BitDataVariable(ADS7280_READ_DATA, 17);

	return result >> 2;
}

void ADS7280_setDefault() {
	ADS7280_send4BitData(ADS7280_DEFAULT_MODE);
}