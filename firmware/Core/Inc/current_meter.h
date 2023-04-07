
#ifndef INC_CURRENT_METER_H_
#define INC_CURRENT_METER_H_

#include "main.h"
#include "stdint.h"
#include "ads7280.h"

#define PATH0_SHUNT				0.002
#define PATH1_SHUNT				0.1
#define PATH2_SHUNT				5.1
#define PATH3_SHUNT				200
#define PATH4_SHUNT				10000

#define SENSOR0_GAIN			100
#define SENSOR1_GAIN			20

#define TEST0_MA				50
#define TEST1_MA				5
#define TEST2_MA				0.5
#define TEST3_MA				0.005

#define PATH0_MA_UNIT			(ADS7280_UNIT_MV / SENSOR0_GAIN / PATH0_SHUNT)
#define PATH1_MA_UNIT			(ADS7280_UNIT_MV / SENSOR1_GAIN / PATH1_SHUNT)
#define PATH2_MA_UNIT			(ADS7280_UNIT_MV / SENSOR1_GAIN / PATH2_SHUNT)
#define PATH3_MA_UNIT			(ADS7280_UNIT_MV / SENSOR1_GAIN / PATH3_SHUNT)
#define PATH4_MA_UNIT			(ADS7280_UNIT_MV / SENSOR1_GAIN / PATH4_SHUNT)

#define PATH0_TOP_LIMIT			16000
#define PATH0_BOTTOM_LIMIT		4000
#define PATH1_TOP_LIMIT			12000
#define PATH1_BOTTOM_LIMIT		4000
#define PATH2_TOP_LIMIT			12000
#define PATH2_BOTTOM_LIMIT		4000
#define PATH3_TOP_LIMIT			12000
#define PATH3_BOTTOM_LIMIT		4000
#define PATH4_TOP_LIMIT			12000
#define PATH4_BOTTOM_LIMIT		100

#define PATH0_DEFAULT_TEST0		((uint16_t) (TEST0_MA/PATH0_MA_UNIT))
#define PATH1_DEFAULT_TEST0		((uint16_t) (TEST0_MA/PATH1_MA_UNIT))
#define PATH1_DEFAULT_TEST1		((uint16_t) (TEST1_MA/PATH1_MA_UNIT))
#define PATH2_DEFAULT_TEST1		((uint16_t) (TEST1_MA/PATH2_MA_UNIT))
#define PATH2_DEFAULT_TEST2		((uint16_t) (TEST2_MA/PATH2_MA_UNIT))
#define PATH3_DEFAULT_TEST2		((uint16_t) (TEST2_MA/PATH3_MA_UNIT))
#define PATH3_DEFAULT_TEST3		((uint16_t) (TEST3_MA/PATH3_MA_UNIT))
#define PATH4_DEFAULT_TEST3		((uint16_t) (TEST3_MA/PATH4_MA_UNIT))

enum currentResolution { A, mA, uA, nA, pA };

typedef struct currentValue {
	float current;
	enum currentResolution resolution;
} currentValue;

typedef struct CurrentMeter_pathDetails {
	float shunt;
	float maUnit;
	uint16_t topLimit;
	uint16_t bottomLimit;
} CurrentMeter_pathDetails;

typedef struct CurrentMeter_offset {
	int16_t voltage;
	int16_t current;
} CurrentMeter_offset;

typedef struct CurrentMeter_state {
	uint8_t path;
	uint8_t measurement;
	uint8_t sensorInput;
	uint8_t outputEnabled;
	CurrentMeter_offset offsets[5];
} CurrentMeter_state;

static CurrentMeter_pathDetails pathDetails[] = {
	{
		shunt: PATH0_SHUNT,
		maUnit: PATH0_MA_UNIT,
		topLimit: PATH0_TOP_LIMIT,
		bottomLimit: PATH0_BOTTOM_LIMIT
	},
	{
		shunt: PATH1_SHUNT,
		maUnit: PATH1_MA_UNIT,
		topLimit: PATH1_TOP_LIMIT,
		bottomLimit: PATH1_BOTTOM_LIMIT
	},
	{
		shunt: PATH2_SHUNT,
		maUnit: PATH2_MA_UNIT,
		topLimit: PATH2_TOP_LIMIT,
		bottomLimit: PATH2_BOTTOM_LIMIT
	},
	{
		shunt: PATH3_SHUNT,
		maUnit: PATH3_MA_UNIT,
		topLimit: PATH3_TOP_LIMIT,
		bottomLimit: PATH3_BOTTOM_LIMIT
	},
	{
		shunt: PATH4_SHUNT,
		maUnit: PATH4_MA_UNIT,
		topLimit: PATH4_TOP_LIMIT,
		bottomLimit: PATH4_BOTTOM_LIMIT
	}
};

void CurrentMeter_actualize();
void CurrentMeter_calibrate();
void CurrentMeter_changePath(uint8_t newPath);
void CurrentMeter_enable();
void CurrentMeter_disable();
void CurrentMeter_read(currentValue * value);

#endif /* INC_CURRENT_METER_H_ */
