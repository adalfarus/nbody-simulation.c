#ifndef NBODY_TYPES_H
#define NBODY_TYPES_H

#include <stdint.h>
#include <inttypes.h>

#if DATA_TYPE_REAL == DT_DOUBLE
typedef double Real;
#define PRIReal "lf"
// Any macro functions
#elif DATA_TYPE_REAL == DT_FLOAT
typedef float Real;
#define PRIReal "f"
// Any macro functions
#elif DATA_TYPE_REAL == DT_UINT64
typedef uint64_t Real;
#define PRIReal PRIu64
// Any macro functions
#elif DATA_TYPE_REAL == DT_INT64
typedef int64_t Real;
#define PRIReal PRIi64
// Any macro functions
#else
#error "No real representation defined! Define one using DATA_TYPE_REAL."
#endif

#if DATA_TYPE_POS == DT_DOUBLE
typedef double PosT;
#define PRIPosT "lf"
// Any macro functions
#elif DATA_TYPE_POS == DT_FLOAT
typedef float PosT;
#define PRIPosT "f"
// Any macro functions
#elif DATA_TYPE_POS == DT_UINT64
typedef uint64_t PosT;
#define PRIPosT PRIu64
// Any macro functions
#elif DATA_TYPE_POS == DT_INT64
typedef int64_t PosT;
#define PRIPosT PRIi64
// Any macro functions
#else
#error "No position type representation defined! Define one using DATA_TYPE_POS."
#endif

#if DATA_TYPE_VEC == DT_DOUBLE
typedef double VecT;
#define PRIVecT "lf"
// Any macro functions
#elif DATA_TYPE_VEC == DT_FLOAT
typedef float VecT;
#define PRIVecT "f"
// Any macro functions
#define VEC_SQRT(x) sqrt(x)
#elif DATA_TYPE_VEC == DT_UINT64
typedef uint64_t VecT;
#define PRIVecT PRIu64
// Any macro functions
#elif DATA_TYPE_VEC == DT_INT64
typedef int64_t VecT;
#define PRIVecT PRIi64
// Any macro functions
#else
#error "No vector type representation defined! Define one using DATA_TYPE_VEC."
#endif

#if DATA_TYPE == DATA_TYPE_DOUBLE
#warning "Using DOUBLE types"
typedef double Real;
typedef double PosT;
typedef double VecT;
#define PRIReal "lf"
#define PRIPosT "lf"
#define PRIVecT "lf"

//#define VEC_SQRT(x) sqrt(x)

#elif DATA_TYPE == DATA_TYPE_FLOAT
typedef float Real;
typedef float PosT;
typedef float VecT;
#define PRIReal "f"
#define PRIPosT "f"
#define PRIVecT "f"
#elif DATA_TYPE == DATA_TYPE_MIXED_64
typedef uint64_t Real;
typedef int64_t PosT;
typedef float VecT;
#define PRIReal PRIu64
#define PRIPosT PRIi64
#define PRIVecT "f"

#define VEC_SQRT(x) sqrtf(x)

#elif DATA_TYPE == DATA_TYPE_MIXED_32
typedef uint32_t Real;
typedef int32_t PosT;
typedef float VecT;
#define PRIReal PRIu32
#define PRIPosT PRIi32
#define PRIVecT "f"
#elif DATA_TYPE == DATA_TYPE_MIXED_16
typedef uint16_t Real;
typedef int16_t PosT;
typedef float VecT;
#define PRIReal PRIu16
#define PRIPosT PRIi16
#define PRIVecT "f"
#elif DATA_TYPE == DATA_TYPE_MIXED_8
typedef uint8_t Real;
typedef int8_t PosT;
typedef float VecT;
#define PRIReal PRIu8
#define PRIPosT PRIi8
#define PRIVecT "f"
#else
#error "No body representation defined! Define one using DATA_TYPE."
#endif

typedef struct {
	Real mass;
	Real radius;

	PosT pos[3];

	VecT vel[3];
	VecT acc[3];
} BodySnapshot;

#endif

