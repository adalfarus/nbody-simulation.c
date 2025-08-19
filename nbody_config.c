#include "nbody_config.h"

static const double M_PER_KM   = 1e3;
static const double M_PER_AU   = 1.496e11;
static const double M_PER_LY   = 9.461e15;
static const double KG_PER_SOL = 1.9885e30;
static const double KG_PER_EAR = 5.9724e24;

// MASS:    TO →   KG             SOLAR                 EARTH                     G                 MG
const double mass_conversion_table[MASS_TYPE_COUNT][MASS_TYPE_COUNT] = {
    /* FROM KG     */ { 1.0,           1.0 / KG_PER_SOL,    1.0 / KG_PER_EAR,       1.0e3,           1.0e6 },
    /* FROM SOLAR  */ { KG_PER_SOL,    1.0,                 KG_PER_SOL / KG_PER_EAR, KG_PER_SOL*1.0e3, KG_PER_SOL*1.0e6 },
    /* FROM EARTH  */ { KG_PER_EAR,    KG_PER_EAR/KG_PER_SOL, 1.0,                   KG_PER_EAR*1.0e3, KG_PER_EAR*1.0e6 },
    /* FROM G      */ { 1.0e-3,        1.0e-3 / KG_PER_SOL, 1.0e-3 / KG_PER_EAR,    1.0,             1.0e3 },
    /* FROM MG     */ { 1.0e-6,        1.0e-6 / KG_PER_SOL, 1.0e-6 / KG_PER_EAR,    1.0e-3,          1.0 }
};

// SPACE:   TO →   M               KM                    AU                       LY                     CM               MM
const double space_conversion_table[SPACE_TYPE_COUNT][SPACE_TYPE_COUNT] = {
    /* FROM M   */ { 1.0,           1.0 / M_PER_KM,       1.0 / M_PER_AU,          1.0 / M_PER_LY,       1.0e2,           1.0e3 },
    /* FROM KM  */ { M_PER_KM,      1.0,                  M_PER_KM / M_PER_AU,     M_PER_KM / M_PER_LY,  1.0e5,           1.0e6 },
    /* FROM AU  */ { M_PER_AU,      M_PER_AU / M_PER_KM,  1.0,                      M_PER_AU / M_PER_LY,  1.0e2 * M_PER_AU, 1.0e3 * M_PER_AU },
    /* FROM LY  */ { M_PER_LY,      M_PER_LY / M_PER_KM,  M_PER_LY / M_PER_AU,     1.0,                  1.0e2 * M_PER_LY, 1.0e3 * M_PER_LY },
    /* FROM CM  */ { 1.0e-2,        1.0e-5,               1.0e-2 / M_PER_AU,       1.0e-2 / M_PER_LY,    1.0,             1.0e1 },
    /* FROM MM  */ { 1.0e-3,        1.0e-6,               1.0e-3 / M_PER_AU,       1.0e-3 / M_PER_LY,    1.0e-1,          1.0 }
};

const double time_conversion_table[TIME_TYPE_COUNT][TIME_TYPE_COUNT] = {
    // TO:     SECOND     MINUTE       HOUR         DAY           WEEK         MONTH         YEAR
    /* SEC */ { 1.0,      1.0/60.0,    1.0/3600.0,   1.0/86400.0,  1.0/604800.0,1.0/2.62974e6,1.0/3.15576e7 },
    /* MIN */ { 60.0,     1.0,         1.0/60.0,     1.0/1440.0,   1.0/10080.0, 1.0/43829.1,  1.0/525960.0 },
    /* HOUR*/ { 3600.0,   60.0,        1.0,          1.0/24.0,     1.0/168.0,   1.0/730.484,  1.0/8760.0 },
    /* DAY */ { 86400.0,  1440.0,      24.0,         1.0,          1.0/7.0,     1.0/30.44,    1.0/365.25 },
    /* WEEK*/ { 604800.0, 10080.0,     168.0,        7.0,          1.0,         7.0/30.44,    7.0/365.25 },
    /* MONTH*/{ 2.62974e6,43829.1,     730.484,      30.44,        30.44/7.0,   1.0,          1.0/12.0 },
    /* YEAR */{ 3.15576e7,525960.0,    8760.0,       365.25,       365.25/7.0,  12.0,         1.0 }
};
