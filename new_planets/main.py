#!/usr/bin/env python3
# Generates C `Body` structs from JPL Horizons for Sun/planets/Pluto + major moons.
# Frame: heliocentric, ecliptic, J2000 ; Units: AU (pos), AU/day (vel)
# Mass: kg ; Radius: meters/1 AU (so radius is in AU to match your data).

from astroquery.jplhorizons import Horizons
import datetime as dt
from astropy.time import Time

# ==== CONFIG ====
# Choose the epoch you want ALL vectors for (UTC calendar; Horizons uses TDB for vector epochs internally)
EPOCH_UTC = "2025-08-17 00:00"
CENTER = "@sun"
REF_PLANE = "ecliptic"

REF_SYSTEM = "J2000"
# Horizons returns in km/s by default; we want AU/day:
OUT_UNITS = "AU-D"

# Which bodies to include (names as Horizons understands them)
BODIES = [
    # Sun & planets + Pluto
    "Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto",
    # Earth system
    "Moon",
    # Jupiter major moons
    "Io", "Europa", "Ganymede", "Callisto",
    # Saturn major moons
    "Mimas", "Enceladus", "Tethys", "Dione", "Rhea", "Titan", "Iapetus",
    # Uranus major moons
    "Miranda", "Ariel", "Umbriel", "Titania", "Oberon",
    # Neptune major moons
    "Triton", "Proteus",
    # Pluto system
    "Charon", "Nix", "Hydra", "Kerberos", "Styx",
]

# Physical constants / properties
AU_M = 1.495978707e11

# Minimal color palette (RGB) — tweak as you like
COLORS = {
    "Sun": (255,204,0), "Mercury": (169,169,169), "Venus": (218,165,32), "Earth": (100,149,237),
    "Mars": (188,39,50), "Jupiter": (210,180,140), "Saturn": (210,180,140), "Uranus": (173,216,230),
    "Neptune": (72,61,139), "Pluto": (210,180,140),
    "Moon": (220,220,220),
    "Io": (222,184,135), "Europa": (205,197,191), "Ganymede": (190,190,190), "Callisto": (160,160,160),
    "Mimas": (180,180,180), "Enceladus": (230,230,255), "Tethys": (200,200,200), "Dione": (205,205,205),
    "Rhea": (210,210,210), "Titan": (210,180,100), "Iapetus": (180,160,140),
    "Miranda": (190,190,200), "Ariel": (200,200,210), "Umbriel": (180,190,200),
    "Titania": (190,200,210), "Oberon": (180,190,210),
    "Triton": (150,170,200), "Proteus": (150,150,160),
    "Charon": (200,200,200), "Nix": (220,220,230), "Hydra": (220,230,230),
    "Kerberos": (210,210,220), "Styx": (210,220,220),
}

# Mass (kg) and mean radius (m). Sources: standard planetary fact sheets / literature.
# (These are stable reference values; Horizons provides state vectors, not masses.)
MASS_KG = {
    "Sun": 1.9885e30, "Mercury": 3.3011e23, "Venus": 4.8675e24, "Earth": 5.9724e24, "Moon": 7.342e22,
    "Mars": 6.4171e23, "Jupiter": 1.8982e27, "Saturn": 5.6834e26, "Uranus": 8.6810e25, "Neptune": 1.0241e26,
    "Pluto": 1.303e22,
    # Jupiter moons
    "Io": 8.9319e22, "Europa": 4.7998e22, "Ganymede": 1.4819e23, "Callisto": 1.0759e23,
    # Saturn moons
    "Mimas": 3.7493e19, "Enceladus": 1.08022e20, "Tethys": 6.17449e20, "Dione": 1.09545e21,
    "Rhea": 2.306518e21, "Titan": 1.3452e23, "Iapetus": 1.805635e21,
    # Uranus moons
    "Miranda": 6.59e19, "Ariel": 1.353e21, "Umbriel": 1.172e21, "Titania": 3.527e21, "Oberon": 3.076e21,
    # Neptune moons
    "Triton": 2.14e22, "Proteus": 4.40e19,
    # Pluto system
    "Charon": 1.586e21, "Nix": 4.5e16, "Hydra": 4.8e16, "Kerberos": 1.65e16, "Styx": 7.5e15,
}

RADIUS_M = {
    "Sun": 6.955e8, "Mercury": 2.4397e6, "Venus": 6.0518e6, "Earth": 6.3781e6, "Moon": 1.7374e6,
    "Mars": 3.3895e6, "Jupiter": 6.9911e7, "Saturn": 5.8232e7, "Uranus": 2.5362e7, "Neptune": 2.4622e7,
    "Pluto": 1.1883e6,
    # Jupiter moons
    "Io": 1.8216e6, "Europa": 1.5608e6, "Ganymede": 2.6341e6, "Callisto": 2.4103e6,
    # Saturn moons
    "Mimas": 1.98e5, "Enceladus": 2.52e5, "Tethys": 5.31e5, "Dione": 5.62e5,
    "Rhea": 7.64e5, "Titan": 2.575e6, "Iapetus": 7.34e5,
    # Uranus moons
    "Miranda": 2.36e5, "Ariel": 5.79e5, "Umbriel": 5.85e5, "Titania": 7.89e5, "Oberon": 7.61e5,
    # Neptune moons
    "Triton": 1.3534e6, "Proteus": 2.10e5,
    # Pluto system
    "Charon": 6.06e5, "Nix": 2.49e4, "Hydra": 3.15e4, "Kerberos": 8.96e3, "Styx": 8.25e3,
}

NAIF = {
    # Sun & planets
    "Sun": "10",
    "Mercury": "199", "Venus": "299", "Earth": "399", "Mars": "499",
    "Jupiter": "599", "Saturn": "699", "Uranus": "799", "Neptune": "899",
    "Pluto": "999",

    # Earth system
    "Moon": "301",

    # Jupiter major moons
    "Io": "501", "Europa": "502", "Ganymede": "503", "Callisto": "504",

    # Saturn major moons
    "Mimas": "601", "Enceladus": "602", "Tethys": "603", "Dione": "604",
    "Rhea": "605", "Titan": "606", "Hyperion": "607", "Iapetus": "608",

    # Uranus major moons
    "Miranda": "701", "Ariel": "702", "Umbriel": "703", "Titania": "704", "Oberon": "705",

    # Neptune major moons
    "Triton": "801", "Nereid": "802", "Naiad": "803", "Thalassa": "804",
    "Despina": "805", "Galatea": "806", "Larissa": "807", "Proteus": "808",

    # Pluto system (Charon + small moons)
    "Charon": "901", "Nix": "902", "Hydra": "903", "Kerberos": "904", "Styx": "905",
}

def vec_for(name: str):
    jd = Time(EPOCH_UTC, scale="utc").jd
    target = NAIF.get(name, name)  # prefer numeric ID; fall back to provided name
    # id_type=None is the modern default (replaces 'id'/'majorbody'); it works with numeric IDs
    obj = Horizons(id=target, id_type=None, location=CENTER, epochs=[jd])
    vec = obj.vectors(refplane=REF_PLANE)

    # Horizons vectors table columns have units; pull as floats
    x, y, z   = float(vec['x'][0]),  float(vec['y'][0]),  float(vec['z'][0])
    vx, vy, vz = float(vec['vx'][0]), float(vec['vy'][0]), float(vec['vz'][0])

    # (Optional sanity: ensure units are AU and AU/day; astroquery usually returns that for vectors)
    # u_pos = str(vec['x'].unit)  # 'AU'
    # u_vel = str(vec['vx'].unit) # 'AU / d'
    return x, y, z, vx, vy, vz

print("// Generated from JPL Horizons via astroquery")
print(f"// Epoch (UTC): {EPOCH_UTC} | center={CENTER} | refplane={REF_PLANE} | refsystem={REF_SYSTEM} | units: AU, AU/day\n")

print("// clang-format off\n")

for name in BODIES:
    x,y,z,vx,vy,vz = vec_for(name)
    mass = MASS_KG[name]
    r_AU = RADIUS_M[name] / AU_M
    r,g,b = COLORS.get(name, (200,200,200))
    print(f"Body {name.lower()} = {{")
    print(f"    .name = \"{name}\",")
    print(f"    .color = {{ {r}, {g}, {b} }},")
    print(f"    .mass_type = MASS_TYPE_KG,")
    print(f"    .space_type = SPACE_TYPE_AU,")
    print(f"    .time_type  = TIME_TYPE_DAY,")
    print(f"    .mass = {mass:.8e},")
    print(f"    .radius = {r_AU:.12g},")
    print(f"    .pos = {{ {x:.6f}, {y:.6f}, {z:.6f} }},")
    print(f"    .vel = {{ {vx:.6f}, {vy:.6f}, {vz:.6f} }}")
    print("};\n")

