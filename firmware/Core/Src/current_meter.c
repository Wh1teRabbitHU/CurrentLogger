#include "current_meter.h"

static CurrentMeter_state _state = {
	path: 0,
	measurement: 0,
	sensorInput: 1,
	outputEnabled: 0,
	offsets: {
		{ voltage: 0, current: 0 },
		{ voltage: 0, current: 0 },
		{ voltage: 0, current: 0 },
		{ voltage: 0, current: 0 },
		{ voltage: 0, current: 0 }
	}
};

void CurrentMeter_actualize() {
	uint16_t topLimit = pathDetails[_state.path].topLimit;
	uint16_t bottomLimit = pathDetails[_state.path].topLimit;

	if (_state.path == 0 && _state.measurement > topLimit) {
		// TODO: Stop output and write error message
		CurrentMeter_disable();
	} else if (_state.path == 4 && _state.measurement < bottomLimit) {
		// DO NOTHING
	} else if (_state.measurement > topLimit) {
		CurrentMeter_changePath(_state.path - 1);
	} else if (_state.measurement < bottomLimit) {
		CurrentMeter_changePath(_state.path + 1);
	}
}

void CurrentMeter_calibrate() {
	// TODO
}

void CurrentMeter_setSensorInput(uint8_t input) {
	if (input == 0) {
		ADS7280_selectInput0();
	} else {
		ADS7280_selectInput1();
	}

	_state.sensorInput = input;
}

void CurrentMeter_setPath(uint8_t path, uint8_t value) {
	switch (path) {
		case 0:
			HAL_GPIO_WritePin(PATH_0_EN_GPIO_Port, PATH_0_EN_Pin, value);
			break;
		case 1:
			HAL_GPIO_WritePin(PATH_1_EN_GPIO_Port, PATH_1_EN_Pin, value);
			break;
		case 2:
			HAL_GPIO_WritePin(PATH_2_EN_GPIO_Port, PATH_2_EN_Pin, value);
			break;
		case 3:
			HAL_GPIO_WritePin(PATH_3_EN_GPIO_Port, PATH_3_EN_Pin, value);
			break;
		case 4:
			HAL_GPIO_WritePin(PATH_4_EN_GPIO_Port, PATH_4_EN_Pin, value);
			break;
	}
}

void CurrentMeter_changePath(uint8_t newPath) {
	if (newPath == 0 && _state.sensorInput == 0) {
		CurrentMeter_setSensorInput(1);
	} else if (newPath != 0 && _state.sensorInput == 1) {
		CurrentMeter_setSensorInput(0);
	}

	CurrentMeter_setPath(_state.path, 0);
	CurrentMeter_setPath(newPath, 1);

	_state.path = newPath;
}

void CurrentMeter_enable() {
	CurrentMeter_setPath(_state.path, 1);

	_state.outputEnabled = 1;
}

void CurrentMeter_disable() {
	CurrentMeter_setPath(_state.path, 0);

	_state.outputEnabled = 0;
}

void CurrentMeter_read(currentValue * value) {
	float measurement = ADS7280_readData();
	int16_t offset = _state.offsets[_state.path].current;
	float adjustedMeasurement = measurement + offset;
	float current = adjustedMeasurement * pathDetails[_state.path].maUnit;

	if (current > 1000) {
		value->current = current / 1000;
		value->resolution = A;
	} else if (current > 1) {
		value->current = current;
		value->resolution = mA;
	} else if (current > 0.001) {
		value->current = current * 1000;
		value->resolution = uA;
	} else if (current > 0.000001) {
		value->current = current * 1000 * 1000;
		value->resolution = nA;
	} else {
		value->current = current * 1000 * 1000 * 1000;
		value->resolution = pA;
	}

	_state.measurement = adjustedMeasurement;
}