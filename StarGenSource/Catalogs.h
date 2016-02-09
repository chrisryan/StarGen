#pragma once

#include "const.h"
#include "structs.h"

#define EM(x)    (x)/SUN_MASS_IN_EARTH_MASSES

// Catalogs used in stargen
extern catalog solstation;
extern catalog dole;
extern catalog jimb;
extern planet_pointer solar_system;
extern planets earth;

// Catalogs used in main
extern catalog manticore_cat;
extern planets sphinx;

extern catalog ilAqr_cat;

namespace StarGen {
    void ListCatalog(catalog * cat);                                                                                    
    void ListCatalogHTML(catalog * cat);
}
