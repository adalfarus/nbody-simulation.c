#include "solar_system_conf.h"

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

