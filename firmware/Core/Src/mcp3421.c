#include "mcp3421.h"

static MCP3421_config currentConfig = {
	ready: 0,
	mode: MCP3421_MODE_CONTINUOUS,
	sampleRate: MCP3421_RATE_240_00,
	gain: MCP3421_GAIN_1X
};

void MCP3421_writeI2C(I2C_HandleTypeDef * i2c, uint8_t data) {
	uint8_t buffer[1];

	buffer[0] = data;

	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c, MCP3421_I2C_ADDR, buffer, 1, 100);

	if (result != HAL_OK) {
		HAL_GPIO_WritePin(SIGNAL_LED_5_GPIO_Port, SIGNAL_LED_5_Pin, 1);
	}
}

void MCP3421_readI2C(I2C_HandleTypeDef * i2c, uint8_t* buffer) {
	uint8_t dataLength = currentConfig.sampleRate == MCP3421_RATE_003_75 ? 4 : 3;
	HAL_StatusTypeDef result = HAL_I2C_Master_Receive(i2c, MCP3421_I2C_ADDR, buffer, dataLength, 100);

	if (result != HAL_OK) {
		HAL_GPIO_WritePin(SIGNAL_LED_5_GPIO_Port, SIGNAL_LED_5_Pin, 1);
	}
}

void MCP3421_setConfig(uint8_t configBin, MCP3421_config* config) {
	config->ready = binary_getBit8(configBin, 7);
	config->mode = binary_getBit8(configBin, 4);
	config->sampleRate = (binary_getBit8(configBin, 3) << 1) & binary_getBit8(configBin, 2);
	config->gain = (binary_getBit8(configBin, 1) << 1) & binary_getBit8(configBin, 0);
}

void MCP3421_readConfig(I2C_HandleTypeDef * i2c, MCP3421_config* config) {
	uint8_t buffer[4] = {0};
	MCP3421_readI2C(i2c, buffer);
	uint8_t configByte = currentConfig.sampleRate == MCP3421_RATE_003_75 ? 3 : 2;
	uint8_t configBin = buffer[configByte];

	MCP3421_setConfig(configBin, config);
	MCP3421_setConfig(configBin, &currentConfig);
}

void MCP3421_writeConfig(I2C_HandleTypeDef * i2c, MCP3421_config* config) {
	uint8_t configBin = 0;

	configBin = configBin | config->ready << 7;
	configBin = configBin | config->mode << 4;
	configBin = configBin | binary_getBit8(config->sampleRate, 1) << 3;
	configBin = configBin | binary_getBit8(config->sampleRate, 0) << 2;
	configBin = configBin | binary_getBit8(config->gain, 1) << 1;
	configBin = configBin | binary_getBit8(config->gain, 0);

	MCP3421_writeI2C(i2c, configBin);
	MCP3421_setConfig(configBin, &currentConfig);
}

uint32_t MCP3421_readMeasurement(I2C_HandleTypeDef * i2c) {
	uint8_t buffer[4] = {0};
	MCP3421_readI2C(i2c, buffer);

	uint32_t measurement = 0;

	measurement = measurement | buffer[2];
	measurement = measurement | buffer[1] << 8;
	measurement = measurement | ((buffer[0] << 16) & 0b11);

	return measurement * 0.25;
}