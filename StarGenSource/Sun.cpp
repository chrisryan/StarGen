#include <math.h>
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

void Sun::calculateLuminosity() {
    long double n = 0.5 * (2.0 - this->mass) + 4.4;
    if (this->mass < 1.0) {
        n = 1.75 * (this->mass - 0.1) + 3.325;
    }

    this->luminosity = pow(this->mass, n);
}
