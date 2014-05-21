#pragma once

long double stellar_dust_limit(long double);
planet_pointer dist_planetary_masses(long double, long double, long double, long double, long double, long double, planet_pointer, bool);
void free_atmosphere(planet_pointer);
void free_generations();
