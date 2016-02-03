#pragma once

int orb_zone(long double, long double);
long double volume_radius(long double, long double);
long double kothari_radius(long double, bool, int);
long double empirical_density(long double, long double, long double, bool);
long double volume_density(long double, long double);
long double period(long double, long double, long double);
long double day_length(planet_pointer);
int inclination(long double);
long double escape_vel(long double, long double);
long double rms_vel(long double, long double);
long double min_molec_weight (planet_pointer);
long double acceleration(long double, long double);
long double gravity(long double);
long double vol_inventory(long double, long double, long double, long double, int, bool, bool);
long double pressure(long double, long double, long double);
long double boiling_point(long double);
long double est_temp(long double, long double, long double);
bool grnhouse(long double r_ecosphere, long double);
long double gas_life(long double, planet_pointer);
void iterate_surface_temp(planet_pointer);

enum Breathability {
    None,
    Breathable,
    Unbreathable,
    Poisonous
};

Breathability breathability(planet_pointer);
const char* breathabilityToText(Breathability type);
