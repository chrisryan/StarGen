#include <cstdlib>

#include "ChemTable.h"
#include "const.h"

using namespace StarGen;

Element::Element(int num, const char *symbol, const char *html_symbol, const char *name,
         long double weight, long double melt, long double boil, long double density,
         long double abunde, long double abunds, long double reactivity, long double max_ipp)
{
    this->num = num;
    this->symbol = symbol;
    this->html_symbol = html_symbol;
    this->name = name;
    this->weight = weight;
    this->melt = melt;
    this->boil = boil;
    this->density = density;
    this->abunde = abunde;
    this->abunds = abunds;
    this->reactivity = reactivity;
    this->max_ipp = max_ipp;
}

Element Gases::gases[] = {
// An  sym  HTML symbol  name  Aw  melt  boil  dens  ABUNDe  ABUNDs  Rea Max inspired pp
    Element(AN_H, "H", "H<SUB><SMALL>2</SMALL></SUB>", "Hydrogen", 1.0079, 14.06, 20.40, 8.99e-05, 0.00125893, 27925.4, 1, 0.0),
    Element(AN_HE, "He", "He", "Helium", 4.0026, 3.46, 4.20, 0.0001787, 7.94328e-09, 2722.7, 0, MAX_HE_IPP),
    Element(AN_N, "N", "N<SUB><SMALL>2</SMALL></SUB>", "Nitrogen", 14.0067, 63.34, 77.40, 0.0012506, 1.99526e-05, 3.13329, 0, MAX_N2_IPP),
    Element(AN_O, "O", "O<SUB><SMALL>2</SMALL></SUB>", "Oxygen", 15.9994, 54.80, 90.20, 0.001429, 0.501187, 23.8232, 10, MAX_O2_IPP),
    Element(AN_NE, "Ne", "Ne", "Neon", 20.1700, 24.53, 27.10, 0.0009, 5.01187e-09, 3.4435e-5, 0, MAX_NE_IPP),
    Element(AN_AR, "Ar", "Ar", "Argon", 39.9480, 84.00, 87.30, 0.0017824, 3.16228e-06, 0.100925, 0, MAX_AR_IPP),
    Element(AN_KR, "Kr", "Kr", "Krypton", 83.8000, 116.60, 119.70, 0.003708, 1e-10, 4.4978e-05, 0, MAX_KR_IPP),
    Element(AN_XE, "Xe", "Xe", "Xenon", 131.3000, 161.30, 165.00, 0.00588, 3.16228e-11, 4.69894e-06, 0, MAX_XE_IPP),
    Element(AN_NH3, "NH3", "NH<SUB><SMALL>3</SMALL></SUB>", "Ammonia", 17.0000, 195.46, 239.66, 0.001, 0.002, 0.0001, 1, MAX_NH3_IPP),
    Element(AN_H2O, "H2O", "H<SUB><SMALL>2</SMALL></SUB>O", "Water", 18.0000, 273.16, 373.16, 1.000, 0.03, 0.001, 0, 0.0),
    Element(AN_CO2, "CO2", "CO<SUB><SMALL>2</SMALL></SUB>", "CarbonDioxide", 44.0000, 194.66, 194.66, 0.001, 0.01, 0.0005, 0, MAX_CO2_IPP),
    Element(AN_O3, "O3", "O<SUB><SMALL>3</SMALL></SUB>", "Ozone", 48.0000, 80.16, 161.16, 0.001, 0.001, 0.000001, 2, MAX_O3_IPP),
    Element(AN_CH4, "CH4", "CH<SUB><SMALL>4</SMALL></SUB>", "Methane", 16.0000, 90.16, 109.16, 0.010, 0.005, 0.0001, 1, MAX_CH4_IPP),
    Element(0, "", "", 0, 0, 0, 0, 0, 0, 0, 0, 0)
};

/*
  {AN_F,  "F",  "F",  "Fluorine", 18.9984,  53.58,  85.10, 0.001696, 0.000630957, 0.000843335, 50, MAX_F_IPP},
  {AN_CL, "Cl", "Cl", "Chlorine", 35.4530, 172.22, 239.20, 0.003214, 0.000125893, 0.005236,    40, MAX_CL_IPP},

  { 910, "H2", "H2",  2, 14.06, 20.40, 8.99e-05,  0.00125893, 27925.4  },
  { 911, "N2", "N2", 28, 63.34, 77.40, 0.0012506, 1.99526e-05,3.13329  },
  { 912, "O2", "O2", 32, 54.80, 90.20, 0.001429,  0.501187, 23.8232, 10},
  {AN_CH3CH2OH, "CH3CH2OH", "Ethanol", 46.0000, 159.06, 351.66, 0.895, 0.001, 0.001, 0},
*/

int Gases::max_gas = (sizeof(Gases::gases) / sizeof(Element)) - 1;

/*
 *  Sort a ChemTable by decreasing abundance.
 */
static int diminishing_abundance(const void *xp, const void *yp)
{
    const StarGen::Element *x = (StarGen::Element *) xp;
    const StarGen::Element *y = (StarGen::Element *) yp;
    long double    xx = x->abunds * x->abunde;
    long double    yy = y->abunds * y->abunde;

    if (xx < yy)
        return +1;
    return (xx > yy ? -1 : 0);
}

void Gases::initialize()
{
    for (int index = 0; index < StarGen::Gases::max_gas; index++)
    {
        if (StarGen::Gases::gases[index].max_ipp == 0.0)
        {
            StarGen::Gases::gases[index].max_ipp = INCREDIBLY_LARGE_NUMBER;
        }
    }

    qsort(
        StarGen::Gases::gases,
        Gases::max_gas,
        sizeof(*StarGen::Gases::gases),
        diminishing_abundance
    );
}
