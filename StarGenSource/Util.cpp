#include    "Util.h"

using namespace StarGen;

namespace StarGen {

    //
    // This function returns if the value under test is between the desired numbers
    // optionally inclusive
    //
    bool Util::between(long double test, long double min, long double max, bool inclusive)
    {
        if (inclusive) {
            return (test >= min && test <= max);
        }

        return (test > min && test < max);
    }
}
