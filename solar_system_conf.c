#include "solar_system_conf.h"

Body sun = {
        .name = "Sun",
        .color = { 255, 204, 0 }, // bright yellow

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 1.9885e30,
        .radius = 6.955e8 / 149597870.7,
        .pos = { 0.0, 0.0, 0.0 },
        .vel = { 0.0, 0.0, 0.0 }
};
Body mercury = {
        .name = "Mercury",
        .color = { 169, 169, 169 },  // dark gray

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 3.3011e23,
        .radius = 2.4397e6 / 149597870.7,
        .pos = { -0.389724, -0.107659, 0.023782 },
        .vel = { 0.003748, -0.022831, -0.002297 }
};
Body venus = {
        .name = "Venus",
        .color = { 218, 165, 32 }, // golden/tan

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 4.8675e24,
        .radius = 6.0518e6 / 149597870.7,
        .pos = { -0.718171, -0.032961, 0.041181 },
        .vel = { 0.000815, -0.020145, -0.000324 }
};
Body earth = {
        .name = "Earth",
        .color = { 100, 149, 237 }, // "earth" blue

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 5.9724e24,
        .radius = 6.3781e6 / 149597870.7,
        .pos = { -0.177159, 0.967219, -4.744e-06 },
        .vel = { -0.017207, -0.003198, 4.163e-07 }
};
Body mars = {
        .name = "Mars",
        .color = { 188, 39, 50 }, // reddish

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 6.4171e23,
        .radius = 3.3895e6 / 149597870.7,
        .pos = { 1.390715, -0.013415, -0.034467 },
        .vel = { 0.000236, 0.015136, 0.000293 }
};
Body jupiter = {
        .name = "Jupiter",
        .color = { 210, 180, 140 }, //beige/brown bands

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 1.8982e27,
        .radius = 6.9911e7 / 149597870.7,
        .pos = { -3.994783, -2.764058, 0.085219 },
        .vel = { 0.004359, -0.006466, -9.35e-05 }
};
Body saturn = {
        .name = "Saturn",
        .color = { 210, 180, 140 }, //pale gold

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 5.6834e26,
        .radius = 5.8232e7 / 149597870.7,
        .pos = { 6.406406, 6.563736, -0.368653 },
        .vel = { -0.003967, 0.003740, 8.75e-05 }
};
Body uranus = {
        .name = "Uranus",
        .color = { 173, 216, 230 }, // light blue

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 8.6810e25,
        .radius = 2.5362e7 / 149597870.7,
        .pos = { 14.431893, -13.734316, -0.233958 },
        .vel = { 0.002683, 0.002660, -2.93e-05 }
};
Body neptune = {
        .name = "Neptune",
        .color = { 72, 61, 139 }, // deep blue

        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type = TIME_TYPE_DAY,

        .mass = 1.0241e26,
        .radius = 2.4622e7 / 149597870.7,
        .pos = { 16.812111, -24.991084, 0.127519 },
        .vel = { 0.002683, 0.001703, -8.90e-05 }
};

Body solar_system[N_BODIES];

void init_solar_system() {
        solar_system[0] = sun;
        solar_system[1] = mercury;
        solar_system[2] = venus;
        solar_system[3] = earth;
        solar_system[4] = mars;
        solar_system[5] = jupiter;
        solar_system[6] = saturn;
        solar_system[7] = uranus;
        solar_system[8] = neptune;
}

const enum MassType solar_preferred_mass_type = MASS_TYPE_KG;
const enum SpaceType solar_preferred_space_type = SPACE_TYPE_AU;
const enum TimeType solar_preferred_time_type = TIME_TYPE_DAY;
