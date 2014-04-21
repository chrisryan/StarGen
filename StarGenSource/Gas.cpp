#include "Gas.h"

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
