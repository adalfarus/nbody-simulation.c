#ifndef NBODY_CONFIG_H
#define NBODY_CONFIG_H

#include <stddef.h>
#include <stdint.h>

enum MassType {
	MASS_TYPE_MG = 0,
	MASS_TYPE_G,
	MASS_TYPE_KG,
	MASS_TYPE_SOLAR,
	MASS_TYPE_EARTH,
	MASS_TYPE_COUNT
};

enum SpaceType {
	SPACE_TYPE_MM = 0,
	SPACE_TYPE_CM,
	SPACE_TYPE_M,
	SPACE_TYPE_KM,
	SPACE_TYPE_AU,
	SPACE_TYPE_LY,
	SPACE_TYPE_COUNT
};

enum TimeType {
	TIME_TYPE_SECOND = 0,
	TIME_TYPE_MINUTE,
	TIME_TYPE_HOUR,
	TIME_TYPE_DAY,
	TIME_TYPE_WEEK,
	TIME_TYPE_MONTH,  // Approx 30.44 days (Gregorian average)
	TIME_TYPE_YEAR,
	TIME_TYPE_COUNT
};

#define DT_DOUBLE 1
#define DT_FLOAT  2
#define DT_UINT64 3
#define DT_INT64  4
#define DT_UINT32 5
#define DT_INT32  6
#define DT_UINT16 7
#define DT_INT16  8
#define DT_UINT8  9
#define DT_INT8   10

extern const double mass_conversion_table[MASS_TYPE_COUNT][MASS_TYPE_COUNT];
extern const double space_conversion_table[SPACE_TYPE_COUNT][SPACE_TYPE_COUNT];
extern const double time_conversion_table[TIME_TYPE_COUNT][TIME_TYPE_COUNT];

typedef struct {
	const char* name;
	uint8_t color[3];

	enum MassType mass_type;
	enum SpaceType space_type;
	enum TimeType time_type;

	double mass;
	double radius;
	double pos[3];
	double vel[3];
} Body;

#endif

