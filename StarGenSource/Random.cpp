#include    <math.h>
#include    <stdlib.h>
#include    "const.h"
#include    "Random.h"

using namespace StarGen;

namespace StarGen {

    //
    // This function returns a random real number between the specified
    // inner and outer bounds.
    //
    long double Random::number(long double inner, long double outer)
    {
        long double range;

        range = outer - inner;
        return((((long double)rand()) / (long double)(RAND_MAX)) * range + inner);
    }

    //
    // This function returns a value within a certain variation of the
    // exact value given it in 'value'.
    //
    long double Random::about(long double value, long double variation)
    {
        return(value + (value * Random::number(-variation,variation)));
    }

    long double Random::eccentricity()
    {
        long double e;

        e = 1.0 - pow(Random::number(0.0, 1.0), ECCENTRICITY_COEFF);

        if (e > .99) {  // Note that this coresponds to a random
            e = .99;    // number less than 10E-26
        }               // It happens with GNU C for -S254 -W27

        return(e);
    }
}
