#include "Sun.h"

using namespace StarGen;

Sun::Sun(long double luminosity, long double mass, long double life,
            long double age, long double r_ecosphere, const char *name)
{
    this->luminosity = luminosity;
    this->mass = mass;
    this->life = life;
    this->age = age;
    this->r_ecosphere = r_ecosphere;
    this->name = name;
}
