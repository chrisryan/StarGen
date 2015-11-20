#include "Gas.h"
#include "const.h"

using namespace StarGen;

Gas::Gas(int num, long double surf_pressure)
{
    this->num = num;
    this->surf_pressure = surf_pressure;
}

/*
 *  Sort a ChemTable by decreasing pressure.
 */
int Gas::diminishing_pressure(const void *xp, const void *yp)
{
    const Gas *x = (Gas *) xp;
    const Gas *y = (Gas *) yp;

    if (x->surf_pressure < y->surf_pressure)
        return +1;
    return (x->surf_pressure > y->surf_pressure ? -1 : 0);
}

/*
 * Inspired partial pressure, taking into account humidification of the
 * air in the nasal passage and throat This formula is on Dole's p. 14
 */
long double Gas::inspired_partial_pressure(long double surf_pressure) {
    long double pH2O = (H20_ASSUMED_PRESSURE);
    long double fraction = this->surf_pressure / surf_pressure;

    return (surf_pressure - pH2O) * fraction;
}
