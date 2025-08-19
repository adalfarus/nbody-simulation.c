#include "solar_system_conf.h"

// t₀ = 2025-08-17 00:00 UTC
Body sun = {
        .name = "Sun",
        .color = { 255, 204, 0 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.98850000e+30,
        .radius = 0.00464913034354,
        .pos = { 0.000000, 0.000000, 0.000000 },
        .vel = { 0.000000, 0.000000, 0.000000 }
};

Body mercury = {
        .name = "Mercury",
        .color = { 169, 169, 169 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 3.30110000e+23,
        .radius = 1.63083872022e-05,
        .pos = { 0.325686, 0.089249, -0.022578 },
        .vel = { -0.012886, 0.028388, 0.003502 }
};

Body venus = {
        .name = "Venus",
        .color = { 218, 165, 32 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 4.86750000e+24,
        .radius = 4.04537843465e-05,
        .pos = { 0.399464, 0.601288, -0.014788 },
        .vel = { -0.016912, 0.011104, 0.001128 }
};

Body earth = {
        .name = "Earth",
        .color = { 100, 149, 237 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 5.97240000e+24,
        .radius = 4.26349651245e-05,
        .pos = { 0.819167, -0.595009, 0.000028 },
        .vel = { 0.009838, 0.013852, -0.000001 }
};

Body mars = {
        .name = "Mars",
        .color = { 188, 39, 50 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 6.41710000e+23,
        .radius = 2.26574080509e-05,
        .pos = { -1.367776, -0.822600, 0.016303 },
        .vel = { 0.007737, -0.010795, -0.000416 }
};

Body jupiter = {
        .name = "Jupiter",
        .color = { 210, 180, 140 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.89820000e+27,
        .radius = 0.000467326170305,
        .pos = { -0.676503, 5.117498, -0.006122 },
        .vel = { -0.007576, -0.000636, 0.000172 }
};

Body saturn = {
        .name = "Saturn",
        .color = { 210, 180, 140 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 5.68340000e+26,
        .radius = 0.000389256877304,
        .pos = { 9.540261, -0.504725, -0.370966 },
        .vel = { -0.000018, 0.005561, -0.000096 }
};

Body uranus = {
        .name = "Uranus",
        .color = { 173, 216, 230 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 8.68100000e+25,
        .radius = 0.000169534498595,
        .pos = { 10.345982, 16.545621, -0.072702 },
        .vel = { -0.003371, 0.001903, 0.000051 }
};

Body neptune = {
        .name = "Neptune",
        .color = { 72, 61, 139 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.02410000e+26,
        .radius = 0.000164587904124,
        .pos = { 29.880234, 0.086281, -0.690341 },
        .vel = { -0.000036, 0.003159, -0.000064 }
};

Body pluto = {
        .name = "Pluto",
        .color = { 210, 180, 140 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.30300000e+22,
        .radius = 7.94329487739e-06,
        .pos = { 18.854899, -29.788838, -2.264773 },
        .vel = { 0.002722, 0.000973, -0.000898 }
};

Body moon = {
        .name = "Moon",
        .color = { 220, 220, 220 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 7.34200000e+22,
        .radius = 1.16138016662e-05,
        .pos = { 0.820235, -0.592785, 0.000248 },
        .vel = { 0.009291, 0.014123, 0.000013 }
};

Body io = {
        .name = "Io",
        .color = { 222, 184, 135 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 8.93190000e+22,
        .radius = 1.21766439019e-05,
        .pos = { -0.679311, 5.117850, -0.006149 },
        .vel = { -0.008793, -0.010530, -0.000198 }
};

Body europa = {
        .name = "Europa",
        .color = { 205, 197, 191 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 4.79980000e+22,
        .radius = 1.04333035804e-05,
        .pos = { -0.674860, 5.121657, -0.005956 },
        .vel = { -0.014998, 0.002223, 0.000103 }
};

Body ganymede = {
        .name = "Ganymede",
        .color = { 190, 190, 190 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.48190000e+23,
        .radius = 1.76078709388e-05,
        .pos = { -0.674361, 5.110686, -0.006351 },
        .vel = { -0.001576, 0.001258, 0.000331 }
};

Body callisto = {
        .name = "Callisto",
        .color = { 160, 160, 160 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.07590000e+23,
        .radius = 1.61118603408e-05,
        .pos = { -0.664365, 5.120449, -0.005866 },
        .vel = { -0.008703, 0.004000, 0.000302 }
};

Body mimas = {
        .name = "Mimas",
        .color = { 180, 180, 180 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 3.74930000e+19,
        .radius = 1.32354825021e-06,
        .pos = { 9.541483, -0.504603, -0.371116 },
        .vel = { -0.001036, 0.012940, -0.003727 }
};

Body enceladus = {
        .name = "Enceladus",
        .color = { 230, 230, 255 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.08022000e+20,
        .radius = 1.68451595481e-06,
        .pos = { 9.538716, -0.504953, -0.370697 },
        .vel = { 0.001508, -0.000845, 0.003113 }
};

Body tethys = {
        .name = "Tethys",
        .color = { 200, 200, 200 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 6.17449000e+20,
        .radius = 3.54951576192e-06,
        .pos = { 9.540542, -0.506477, -0.370112 },
        .vel = { 0.006451, 0.006171, -0.000973 }
};

Body dione = {
        .name = "Dione",
        .color = { 205, 205, 205 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.09545000e+21,
        .radius = 3.75673796271e-06,
        .pos = { 9.538057, -0.505722, -0.370231 },
        .vel = { 0.002767, 0.000967, 0.002038 }
};

Body rhea = {
        .name = "Rhea",
        .color = { 210, 210, 210 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 2.30651800e+21,
        .radius = 5.10702456141e-06,
        .pos = { 9.538398, -0.502011, -0.372229 },
        .vel = { -0.004157, 0.003426, 0.001416 }
};

Body titan = {
        .name = "Titan",
        .color = { 210, 180, 100 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.34520000e+23,
        .radius = 1.72128118398e-05,
        .pos = { 9.534960, -0.498792, -0.373498 },
        .vel = { -0.002470, 0.003917, 0.000996 }
};

Body iapetus = {
        .name = "Iapetus",
        .color = { 180, 160, 140 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.80563500e+21,
        .radius = 4.90648694775e-06,
        .pos = { 9.519591, -0.515914, -0.364231 },
        .vel = { 0.000911, 0.003988, 0.000080 }
};

Body miranda = {
        .name = "Miranda",
        .color = { 190, 190, 200 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 6.59000000e+19,
        .radius = 1.57756256086e-06,
        .pos = { 10.344903, 16.545941, -0.072100 },
        .vel = { -0.001813, 0.001947, 0.002823 }
};

Body ariel = {
        .name = "Ariel",
        .color = { 200, 200, 210 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.35300000e+21,
        .radius = 3.87037594379e-06,
        .pos = { 10.345039, 16.546027, -0.071259 },
        .vel = { -0.001153, 0.001632, 0.001575 }
};

Body umbriel = {
        .name = "Umbriel",
        .color = { 180, 190, 200 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.17200000e+21,
        .radius = 3.91048346653e-06,
        .pos = { 10.344273, 16.545667, -0.075058 },
        .vel = { -0.005020, 0.002432, 0.001254 }
};

Body titania = {
        .name = "Titania",
        .color = { 190, 200, 210 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 3.52700000e+21,
        .radius = 5.27413923947e-06,
        .pos = { 10.343914, 16.546524, -0.069519 },
        .vel = { -0.001875, 0.001722, 0.001070 }
};

Body oberon = {
        .name = "Oberon",
        .color = { 180, 190, 210 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 3.07600000e+21,
        .radius = 5.08697080005e-06,
        .pos = { 10.346816, 16.545378, -0.072697 },
        .vel = { -0.003526, 0.001270, -0.003748 }
};

Body triton = {
        .name = "Triton",
        .color = { 150, 170, 200 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 2.14000000e+22,
        .radius = 9.04692021128e-06,
        .pos = { 29.880331, 0.084564, -0.691975 },
        .vel = { -0.002192, 0.002178, 0.000839 }
};

Body proteus = {
        .name = "Proteus",
        .color = { 150, 150, 160 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 4.40000000e+19,
        .radius = 1.40376329568e-06,
        .pos = { 29.880095, 0.086987, -0.690024 },
        .vel = { -0.004062, 0.001834, 0.001131 }
};

Body charon = {
        .name = "Charon",
        .color = { 200, 200, 200 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.58600000e+21,
        .radius = 4.05085979609e-06,
        .pos = { 18.854856, -29.788810, -2.264652 },
        .vel = { 0.002807, 0.001069, -0.000890 }
};

Body nix = {
        .name = "Nix",
        .color = { 220, 220, 230 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 4.50000000e+16,
        .radius = 1.66446219344e-07,
        .pos = { 18.854808, -29.789063, -2.264976 },
        .vel = { 0.002675, 0.000954, -0.000845 }
};

Body hydra = {
        .name = "Hydra",
        .color = { 220, 230, 230 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 4.80000000e+16,
        .radius = 2.10564494351e-07,
        .pos = { 18.855106, -29.788835, -2.265134 },
        .vel = { 0.002693, 0.000927, -0.000919 }
};

Body kerberos = {
        .name = "Kerberos",
        .color = { 210, 210, 220 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 1.65000000e+16,
        .radius = 5.98939006155e-08,
        .pos = { 18.855010, -29.788934, -2.265115 },
        .vel = { 0.002681, 0.000927, -0.000898 }
};

Body styx = {
        .name = "Styx",
        .color = { 210, 220, 220 },
        .mass_type = MASS_TYPE_KG,
        .space_type = SPACE_TYPE_AU,
        .time_type  = TIME_TYPE_DAY,
        .mass = 7.50000000e+15,
        .radius = 5.51478437587e-08,
        .pos = { 18.854919, -29.788964, -2.265011 },
        .vel = { 0.002667, 0.000927, -0.000875 }
};

Body solar_system[N_BODIES];

void init_solar_system(void) {
        int i = 0;

        // Sun and planets
        solar_system[i++] = sun;
        solar_system[i++] = mercury;
        solar_system[i++] = venus;
        solar_system[i++] = earth;
        solar_system[i++] = mars;
        solar_system[i++] = jupiter;
        solar_system[i++] = saturn;
        solar_system[i++] = uranus;
        solar_system[i++] = neptune;
        solar_system[i++] = pluto;

        // Earth system
        solar_system[i++] = moon;

        // Jupiter major moons
        solar_system[i++] = io;
        solar_system[i++] = europa;
        solar_system[i++] = ganymede;
        solar_system[i++] = callisto;

        // Saturn major moons
        solar_system[i++] = mimas;
        solar_system[i++] = enceladus;
        solar_system[i++] = tethys;
        solar_system[i++] = dione;
        solar_system[i++] = rhea;
        solar_system[i++] = titan;
        solar_system[i++] = iapetus;

        // Uranus major moons
        solar_system[i++] = miranda;
        solar_system[i++] = ariel;
        solar_system[i++] = umbriel;
        solar_system[i++] = titania;
        solar_system[i++] = oberon;

        // Neptune major moons
        solar_system[i++] = triton;
        solar_system[i++] = proteus;

        // Pluto system
        solar_system[i++] = charon;
        solar_system[i++] = nix;
        solar_system[i++] = hydra;
        solar_system[i++] = kerberos;
        solar_system[i++] = styx;
}

const enum MassType solar_preferred_mass_type = MASS_TYPE_KG;
const enum SpaceType solar_preferred_space_type = SPACE_TYPE_AU;
const enum TimeType solar_preferred_time_type = TIME_TYPE_DAY;
