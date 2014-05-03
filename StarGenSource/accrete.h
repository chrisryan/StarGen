#pragma once

void set_initial_conditions(long double, long double );
long double stellar_dust_limit(long double);
long double nearest_planet(long double);
long double farthest_planet(long double);
long double inner_effect_limit(long double, long double, long double );
long double outer_effect_limit(long double, long double, long double );
int dust_available(long double, long double );
void update_dust_lanes(long double, long double, long double, long double, long double, long double );
long double critical_limit(long double, long double, long double );
void accrete_dust(long double *, long double *, long double *, long double, long double, long double, long double, long double );
void coalesce_planetesimals(long double, long double, long double, long double, long double, long double, long double, long double, long double, bool);
planet_pointer dist_planetary_masses(long double, long double, long double, long double, long double, long double, planet_pointer, bool);
void free_planet (planet_pointer);
void free_atmosphere(planet_pointer);
void free_generations();
