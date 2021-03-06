#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "Catalogs.h"

#define AVE(x,y) ((x+y)/2.)

            /*     No Orbit   Eccen. Tilt Mass    Gas Giant? Dust Mass   Gas */
planets luna     ={1,2.571e-3,0.055,1.53, EM(.01229), false, EM(.01229), 0, ZEROES,0,NULL, NULL};
planets callisto ={4,1.259e-2,0    ,0   , EM(1.62e-2),false,EM(1.62-2 ), 0, ZEROES,0,NULL, NULL};
planets ganymede ={3,7.16e-3,0.0796,0   , EM(2.6e-2 ),false,EM(2.6e-2 ), 0, ZEROES,0,NULL, &callisto};
planets europa   ={2,4.49e-3,0.0075,0   , EM(7.9e-3 ),false,EM(7.9e-3 ), 0, ZEROES,0,NULL, &ganymede};
planets io       ={1,2.82e-3,0.0006,0   , EM(1.21e-2),false,EM(1.21e-2), 0, ZEROES,0,NULL, &europa};
planets iapetus  ={6,2.38e-2,0.029, 0   , EM(8.4e-4 ),false,EM(8.4e-4 ), 0, ZEROES,0,NULL, NULL};
planets hyperion ={5,9.89e-3,0.110, 0   , EM(1.82e-5),false,EM(1.82e-5), 0, ZEROES,0,NULL, &iapetus};
planets titan    ={4,8.17e-3,0.0289,0   , EM(2.3e-2 ),false,EM(2.3e-2 ), 0, ZEROES,0,NULL, &hyperion};
planets rhea     ={3,3.52e-3,0.0009,0   , EM(3.85e-4),false,EM(3.85e-4), 0, ZEROES,0,NULL, &titan};
planets dione    ={2,2.52e-3,0.0021,0   , EM(1.74e-4),false,EM(1.74e-4), 0, ZEROES,0,NULL, &rhea};
planets tethys   ={1,1.97e-3,0.000, 0   , EM(1.09e-4),false,EM(1.09e-4), 0, ZEROES,0,NULL, &dione};
planets triton   ={1,2.36e-3,0.000, 0   , EM(2.31e-2),false,EM(2.31e-2), 0, ZEROES,0,NULL, NULL};
planets charon   ={1,19571/
                   KM_PER_AU,0.000, 0   , EM(2.54e-4),false,EM(2.54e-4), 0, ZEROES,0,NULL, NULL};

planets xena   ={11,67.6681,0.44177,0   , EM(.0025),false, EM(.0025),    0, ZEROES,0,NULL,    NULL};
planets pluto  ={10,39.529,0.248,122.5, EM(0.002),  false, EM(0.002),    0, ZEROES,0,&charon, &xena};
planets neptune={ 9,30.061,0.010, 29.6, EM(17.14),  true,  0,   EM(17.14),  ZEROES,0,&triton, &pluto};
planets uranus ={ 8,19.191,0.046, 97.9, EM(14.530), true,  0,   EM(14.530), ZEROES,0,NULL,    &neptune};
planets saturn ={ 7,9.539, 0.056, 26.7, EM(95.18),  true,  0,   EM(95.18),  ZEROES,0,&tethys, &uranus};
planets jupiter={ 6,5.203, 0.048,  3.1, EM(317.9),  true,  0,   EM(317.9),  ZEROES,0,&io,     &saturn};
planets ceres  ={ 5,2.766, 0.080,  0,   9.5e20 /
                           SOLAR_MASS_IN_KILOGRAMS, false, 9.5e20 /
                                                SOLAR_MASS_IN_KILOGRAMS, 0, ZEROES,0,NULL,    &jupiter};
planets mars   ={ 4,1.524, 0.093, 25.2, EM(0.1074), false, EM(0.1074),   0, ZEROES,0,NULL,    &ceres};
planets earth  ={ 3,1.000, 0.017, 23.5, EM(1.00),   false, EM(1.00),     0, ZEROES,0,&luna,   &mars};
planets venus  ={ 2,0.723, 0.007,177.3, EM(0.815),  false, EM(0.815),    0, ZEROES,0,NULL,    &earth};
planets mercury={ 1,0.387, 0.206,  2,   EM(0.055),  false, EM(0.055),    0, ZEROES,0,NULL,    &venus};
planet_pointer solar_system = &mercury;

/* planets ceres  ={ 5,2.767, 0.079,  0, EM(0.000134), false, EM(0.000134), 0, ZEROES,0,NULL , &jupiter}; */

/* planets  titan2 ={ 7,9.539, 0.056, 26.7 , EM(0.02259), false, EM(0.02259),EM(.001), ZEROES,0,NULL, &uranus}; */

/* Seeds for accreting the solar system */
            /*   No Orbit  Eccen... */
planets pluto1  ={10,39.529,0.248, 0, 0, 0, 0, 0, ZEROES,0,NULL, NULL};
planets pluto2  ={10,39.529,0.15,  0, 0, 0, 0, 0, ZEROES,0,NULL, NULL}; // The real eccentricity
planets mars1   ={ 4,1.524, 0.093, 0, 0, 0, 0, 0, ZEROES,0,NULL, &pluto2};  // collides Pluto+Neptune
planets ceres1  ={ 5,2.767, 0.079, 0, 0, 0, 0, 0, ZEROES,0,NULL, &mars1};
planets saturn1 ={ 7,9.539, 0.056, 0, 0, 0, 0, 0, ZEROES,0,NULL, &ceres1};
planets uranus1 ={ 8,19.191,0.046, 0, 0, 0, 0, 0, ZEROES,0,NULL, &saturn1};
planets neptune1={ 9,30.061,0.010, 0, 0, 0, 0, 0, ZEROES,0,NULL, &uranus1};
planets jupiter1={ 6,5.203, 0.048, 0, 0, 0, 0, 0, ZEROES,0,NULL, &neptune1};
planets mercury1={ 1,0.387, 0.206, 0, 0, 0, 0, 0, ZEROES,0,NULL, &jupiter1};
planets earth1  ={ 3,1.000, 0.017, 0, 0, 0, 0, 0, ZEROES,0,NULL, &mercury1};
planets venus1  ={ 2,0.723, 0.007, 0, 0, 0, 0, 0, ZEROES,0,NULL, &earth1};
planet_pointer solar_system1 = &venus1;

planets eriEpsI ={ 1,3.3,   0.608,  0, 0, 0, 0, 0, ZEROES,0,NULL, NULL};
planets UMa47II ={ 2,3.73,  0,      0, 0, 0, 0, 0, ZEROES,0,NULL, NULL};
planets UMa47I  ={ 1,2.11,  0.096,  0, 0, 0, 0, 0, ZEROES,0,NULL, &UMa47II};
planets horIotI ={ 1,0.925, 0.161,  0, 0, 0, 0, 0, ZEROES,0,NULL, NULL};

            /*  No Orbit Eccen. Tilt   Mass    Gas Giant? Dust Mass   Gas */
planets smallest={0, 0.0, 0.0,  20.0,   EM(0.4),   false,  EM(0.4),   0.0, ZEROES,0,NULL, NULL};
planets average ={0, 0.0, 0.0,  20.0,   EM(1.0),   false,  EM(1.0),    0.0, ZEROES,0,NULL, NULL};
planets largest ={0, 0.0, 0.0,  20.0,   EM(1.6),   false,  EM(1.6),   0.0, ZEROES,0,NULL, NULL};

                /*    L  Mass   Mass2   Eccen.  SemiMajorAxis   Designation Name    */
star    perdole[] = {{0, 1.00,  0,      0,      0,               &mercury,  "Sol",       1, "The Solar System"},
                     {0, 1.08,  0.88,   0.52,   23.2,            NULL,      "ALF Cen A", 1, "Alpha Centauri A"},
                     {0, 0.88,  1.08,   0.52,   23.2,            NULL,      "ALF Cen B", 1, "Alpha Centauri B"},
                     {0, 0.80,  0,      0,      0,               &eriEpsI,  "EPS Eri",   1, "Epsilon Eridani"},
                     {0, 0.82,  0,      0,      0,               NULL,      "TAU Cet",   1, "Tau Ceti"},
                     {0, 0.90,  0.65,   0.50,   AVE(22.8,24.3),  NULL,      "70 Oph",    1, "70 Ophiuchi A"},
                     {0, 0.94,  0.58,   0.53,   AVE(69.,71.),    NULL,      "ETA Cas",   1, "Eta Cassiopeiae A"},
                     {0, 0.82,  0,      0,      0,               NULL,      "SIG Dra",   1, "Sigma Draconis"},
                     {0, 0.77,  0.76,   0,      22.,             NULL,      "36 Oph",    1, "36 Ophiuchi A"},
                     {0, 0.76,  0.77,   0,      22.,             NULL,      "36 Oph B",  0, "36 Ophiuchi B"},
/*                   {0, 0.76,  0,      0,      46.,             NULL,      "HD 191408", 1, "HR7703 A"}, */
/* Fake up a B just to clip the distances -- need the real data */
                     {0, 0.76,  .5,     .5,     46.,             NULL,      "HD 191408", 1, "HR7703 A"},
                     {0, 0.98,  0,      0,      0,               NULL,      "DEL Pav",   1, "Delta Pavonis"},
                     {0, 0.91,  0,      0,      0,               NULL,      "82 Eri",    1, "82 Eridani"},
                     {0, 1.23,  0,      0,      0,               NULL,      "BET Hyi",   1, "Beta Hydri"},
                     {0, 0.74,  0,      0,      0,               NULL,      "HD 219134", 1, "HR8832"},
                     {0, 0.725,0,       0,      1100.,           NULL,      "HD 16160",  1, "HR753 A"}
                    };

/*
 *  The following values were taken from: http://www.solstation.com/stars.htm
 */

star    web[] =
// L            Mass            Mass2           Eccen.  SMAxis   Planets    Designation Name
{{1.00,         1.00,           0,              0,      0,       &mercury,  "Sol",       1, "The Solar System"},        // 0
 {1.60,         1.09,           0.90,           0.519,  23.7,    NULL,      "ALF Cen A", 1, "Alpha Centauri A"},        // 4.4
 {0.45,         0.90,           1.09,           0.519,  23.7,    NULL,      "ALF Cen B", 1, "Alpha Centauri B"},        // 4.4
 {0.34,         0.85,           0,              0,      0,       &eriEpsI,  "EPS Eri",   1, "Epsilon Eridani"},         // 10.5
//{AVE(6.3,8.9),0.59,           0.5,            .48,    85.2,    NULL,      "61 Cyg A",  1, "61 Cygni A"},              // 11.4
 {0.085,        0.59,           0.5,            .48,    85.2,    NULL,      "61 Cyg A",  1, "61 Cygni A"},              // 11.4
 {0.59,         0.82,           0,              0,      0,       NULL,      "TAU Cet",   1, "Tau Ceti"},                // 11.9
 {0.38,         0.75,           (0.16+0.43),    0,      418.,    NULL,      "40 Eri",    1, "40 Eridani A"},            // 16.5
 {AVE(.44,.47), 0.924,          0.701,          0.495,  23.3,    NULL,      "70 Oph",    1, "70 Ophiuchi A"},           // 16.6
 {0.39,         0.82,           0,              0,      0,       NULL,      "SIG Dra",   1, "Sigma Draconis"},          // 18.8
 {0.156,        0.76,           (0.55+0.35),    0.20,   190.,    NULL,      "33 g Lib",  1, "HR 5568"},                 // 19.3
 {AVE(1.0,1.29),0.91,           0.56,           0.497,  71.0,    NULL,      "ETA Cas",   1, "Eta Cassiopeiae A"},       // 19.4
 {0.23,         0.82,           0.20,           .5,     43.,     NULL,      "HD 191408", 1, "HR 7703 (HJ 5173) A"},     // 19.7
 {0.65,         0.97,           0,              0,      0,       NULL,      "82 Eri",    1, "82 Eridani"},              // 19.8
 {1.2,          0.98,           0,              0,      0,       NULL,      "DEL Pav",   1, "Delta Pavonis"},           // 19.9
 {0,            0.74,           0,              0,      0,       NULL,      "HD 219134", 1, "HR 8832"},                 // 21.3
 {0.52,         0.90,           0.76,           0.51,   33.6,    NULL,      "XI Boo",    1, "Xi Bootis A"},             // 21.8
 {0.21,         0.81,           0.082,          .75,    15.,     NULL,      "HD 16160",  1, "HR 753 A"},                // 23.5
 {0.24,         0.83,           0,              0,      0,       NULL,      "HD 4628",   1, "BD+04 123 (HR 222)"},      // 24.3
 {3.6,          1.1,            0,              0,      0,       NULL,      "BET Hyi",   1, "Beta Hydri"},              // 24.4
 {0.37,         0.89,           0,              0,      0,       NULL,      "107 Psc",   1, "107 Piscium"},             // 24.4
                                                        // 107 Psc p1 = Klotho in Celestia's imagined.ssc
 {3.,           1.3,            0,              0,      0,       NULL,      "PI3 Ori",   1, "Pi3 Orionis A"},           // 26.2
 {0.28,         0.88,           0.86,           0.534,  63.7,    NULL,      "RHO1 Eri",  1, "Rho Eridani A"},           // 26.6
 {0.25,         0.86,           0.88,           0.534,  63.7,    NULL,      "RHO2 Eri",  1, "Rho Eridani B"},           // 26.6
 {1.2,          1.07,           0,              0,      0,       NULL,      "BET CVn",   1, "Chara"},                   // 27.3
 {2.9,          .90,            1.45,           0.412,  21.2,    NULL,      "XI UMa",    1, "Xi Ursae Majoris Ba"},     // 27.3
//                                                                  Xi Urs Maj aka Alula Australis
//                  55203:Alula Australis:XI UMa:53 UMa defined in Celestia starnames, but no data
 {0.80,         0.96,           0,              0,      0,       NULL,      "61 Vir",    1, "61 Virginis"},             // 27.8
 {1.3,          0.98,           0,              0,      0,       NULL,      "ZET Tuc",   1, "Zeta Tucanae"},            // 28.0
 {1.08,         1.0,            .15,            0.45,   6.4,     NULL,      "CHI1 Ori",  1, "Chi1 Orionis A"},          // 28.3
//                  41 Arae masses are Wieth-Knudsen's 1957 estimates,
 {0.41,         0.9,            .6,             0.779,  91.5,    NULL,      "41 Ari",    1, "41 Arae A"},               // 28.7
 {0.21,         0.845,          0,              0,      0,       NULL,      "HR 1614",   0, "BD-05 1123 (HR 1614) A"},  // 28.8
 {0.33,         0.87,           0,              0,      0,       NULL,      "HR 7722",   0, "CD-27 14659 (HR 7722)"},   // 28.8
 {2.6,          1.2,            .63,            .5,     864.,    NULL,      "GAM Lep",   1, "Gamma Leporis A"},         // 29.3
 {1.4,          1.05,           0,              0,      0,       NULL,      "BET Com",   1, "Beta Comae Berenices"},    // 29.9
 {0.85,         1.0,            0,              0,      0,       NULL,      "KAP1 Cet",  1, "Kappa Ceti"},              // 29.9
 {1.5,          0.8,            0,              0,      0,       NULL,      "GAM Pav",   1, "Gamma Pavonis"},           // 30.1
 {0.82,         0.8,            0.07,           0.6,    235.,    NULL,      "HD 102365", 1, "HR 4523"},                 // 30.1
 {0.588,        0.81,           0,              0,      0,       NULL,      "61 UMa",    1, "61 Ursae Majoris"},        // 31.1
 {0.31,         0.87,           0,              0.5,    80.5,    NULL,      "HR 4458",   0, "CD-32 8179 (HR 4458)"},    // 31.1
 {AVE(.39,.41), 0.90,           0,              0,      0,       NULL,      "12 Oph",    1, "12 Ophiuchi"},             // 31.9
 {0.46,         0.92,           0,              0,      0,       NULL,      "HR 511",    0, "BD+63 238 (HR 511)"},      // 32.5
 {0.83,         0.87,           0,              0,      0,       NULL,      "ALF Men",   1, "Alpha Mensae"},            // 33.1
 {0.93,         0.79,           1.02,           0.5,    9000.,   NULL,      "ZET1 Ret",  1, "Zeta 1 Reticuli"},         // 39.4-39.5
 {0.99,         1.02,           0.79,           0.5,    9000.,   NULL,      "ZET2 Ret",  1, "Zeta 2 Reticuli"},         // 39.4-39.5
 {1.14,         1.05,           2.0,            0.55,   48.5,    NULL,      "44 Boo",    1, "44 Bootis A"},             // 41.6
 {1.7,          1.03,           0,              0,      0,       &UMa47I,   "47 UMa",    1, "47 Ursae Majoris"},        // 45.9
 {1.8,          1.03,           0,              0,      0,       &horIotI,  "IOT Hor",   1, "Iota Horologii"},          // 56.2

 {AVE(.13,.15), AVE(.59,.71),   0,              0,      0,       NULL,      "EPS Ind",   1, "Epsilon Indi"},            // 11.8
 {AVE(.083,.09),0.701,          0.924,          0.495,  23.3,    NULL,      "70 Oph",    1, "70 Ophiuchi B"},           // 16.6
 {0.28,         0.85,           0.85,           0.922,  88.,     NULL,      "36 Oph",    1, "36 Ophiuchi A"},           // 19.5
 {0.27,         0.85,           0.85,           0.922,  88.,     NULL,      "36 Oph B",  0, "36 Ophiuchi B"},           // 19.5
 {0.12,         0.75,           0.65,           0.58,   12.6,    NULL,      "HR 6426",   0, "MLO 4 (HR 6426) A"},       // 22.7
 {0.146,        0.80,           0.50,           0.5,    500.,    NULL,      "BD-05 1844 A",0,"BD-05 1844 A"}            // 28.3
};

// BD-05 1123 A:     HR 1614, Gl 183 A, Hip 23311, HD 32147, SAO 131688, LHS 200, LTT 2412, LFT 382, and LPM 200.
// CD-27 14659:      HR 7722, Gl 785, Hip 99825, HD 192310, CP(D)-27 6972, SAO 189065, LHS 488, LTT 8009, LFT 1535, and LPM 731
// CD-32 8179 A:     HR 4458, Gl 432 A, Hip 56452, HD 100623, CP(D)-32 3122, SAO 202583, LHS 308, LTT 4280, LFT 823, LPM 389, and E 439-246.
// BD+63 238:        HR 511*, Gl 75, Hip 8362, HD 10780, SAO 11983, LHS 1297, LTT 10619, and LFT 162.
// 36 Ophiuchi B:    HR 6401, Gl 663 B, HD 155885, SAO 185199, LHS 438, and ADS 10417 B.
// MLO 4 A:          HR 6426*, Gl 667 A, Hip 84709, HD 156384, CD-34 11626 A, CP-34 6803, SAO 208670, LHS 442, LTT 6888, LFT 1336, LPM 638, and UGPMF 433.
// BD-05 1844 A:     Gl 250 A, Hip 32984, HD 50281, SAO 133805, LHS 1875, LTT 2662, LFT 494, and LPM 244.

// {.00006,     0.105,          0.1,            0.62,   5.5,     NULL,      "", "Luyten 726-8 A"},      // 8.7
// {0.039,      0.5,            0.59,           .48,    85.2,    NULL,      "", "61 Cygni B"},          // 11.4
// {0.05,       0.65,           0.75,           0.58,   12.6,    NULL,      "", "MLO 4 (HR 6426) B"},   // 22.7
// {1.1,        1.05,           0.4,            0.53,   0.6,     NULL,      "", "Xi Ursae Majoris Aa"}, // 27.3
// {0,          0.4,            1.05,           0.53,   0.6,     NULL,      "", "Xi Ursae Majoris Ab"}, // 27.3
// {0.064,      0.76,           0.90,           0.51,   33.0,    NULL,      "", "Xi Bootis B"},         // 21.8

star    various[] =
{
// L            Mass            Mass2           Eccen.  SMAxis   Planets    Designation Name
 {1.00,         1.00,           0,              0,      0,       &mercury,  "Sol",       1, "The Solar System"},        // 0
 {14800.,       8,              0,              0,      0,       NULL,      "ALF Car",   1, "Canopus"}
};

catalog solstation  = {sizeof(web) / sizeof (star),     "w",    &web[0]};
catalog dole        = {sizeof(perdole) / sizeof (star), "d",    &perdole[0]};
catalog jimb        = {sizeof(various) / sizeof (star), "F",    &various[0]};

//
// StarGen supports private catalogs. The two here are ones I am using
// for debuggery. They may go away.
//

                /*  No  Orbit   Eccen.  Tilt    Mass        Giant?  Dust Mass   Gas */
planets sphinx3   ={ 4, 3.0,    0.046,  10.5,   EM(2.35),   false,  EM(2.35),   0,  ZEROES,0,NULL, NULL};
planets sphinx2   ={ 3, 2.25,   0.02,   10.5,   EM(2.35),   false,  EM(2.35),   0,  ZEROES,0,NULL, &sphinx3};
planets sphinx    ={ 2, 1.6,    0.02,   10.5,   EM(2.2),    false,  EM(2.2),    0,  ZEROES,0,NULL, &sphinx2};
planets manticore ={ 1, 1.115,  0.017,  23.5,   EM(1.01),   false,  EM(1.01),   0,  ZEROES,0,NULL, &sphinx};

star    manticores[] =
// L        Mass    Mass2   Eccen.  SMAxis   Planets    Designation         Name
{{1.00,     1.00,   0,      0,      0,       &mercury,  "Sol",           1, "The Solar System"},
 {1.24,     1.047,  1.0,    0.05,   79.2,    &manticore,"Manticore A",   1, "Manticore A"},
 {1.0,      1.00,   1.047,  0.05,   79.2,    NULL,      "Manticore B",   1, "Manticore B"},
};

catalog manticore_cat   = {sizeof(manticores) / sizeof (star),  "B", &manticores[0]};

star    helios[] =
// L        Mass    Mass2   Eccen.  SMAxis   Planets    Designation     Name
{{1.00,     1.00,   0,      0,      0,       &mercury,  "Sol",       1, "The Solar System"},
 {1.08,     1.0,    0.87,   0.45,   8.85,    NULL,      "Helio A",   1, "Helio A"},
 {0.83,     0.87,   1.0,    0.45,   8.85,    NULL,      "Helio B",   1, "Helio B"},
};

catalog helio       = {sizeof(helios) / sizeof (star), "?", &helios[0]};

               /* No Orbit Eccen. Tilt   Mass      Gas Giant? Dust Mass  Gas */
planets ilaqrb = {1, 0.21, 0.1,   0,     EM(600.), true,      0,   EM(600.), ZEROES,0,NULL, NULL};
planets ilaqrc = {2, 0.13, 0.27,  0,     EM(178.), true,      0,   EM(178.), ZEROES,0,NULL, &ilaqrb};
planets ilaqrd = {3, 0.021,0.22,  0,     EM(5.9),  false,     EM(5.9),    0, ZEROES,0,NULL, &ilaqrc};   // EM(5.9) or 7.53 +/- 0.70 Earth-masses

star    ilAqrs[] =
// L        Mass    Mass2   Eccen.  SMAxis   Planets    Designation Celes   Name
{{1.00,     1.00,   0,      0,      0,       &mercury,  "Sol",      1, "The Solar System"},
{0.0016,    0.32,   0,      0,      0,       &ilaqrd,   "IL Aqr",   1, "IL Aquarii/Gliese 876"} // 15.2
};

catalog ilAqr_cat = {sizeof(ilAqrs) / sizeof (star),  "G", &ilAqrs[0]};

namespace StarGen {
    void ListCatalog(catalog * cat) {                                                                                   
        int catalog_count = cat->count;                                                                                 
        for (int index = 0; index < catalog_count; index++) {                                                           
            fprintf(stdout,                                                                                             
                    "%3d: %-30.30s M: %4.2LG L: %4.2LG\n",                                                              
                    index,                                                                                              
                    cat->stars[index].name,                                                                             
                    cat->stars[index].mass,                                                                             
                    cat->stars[index].luminosity                                                                        
                    );                                                                                                  
        }                                                                                                               
    }                                                                                                                   
                                                                                                                        
    void ListCatalogHTML(catalog * cat) {                                                                               
        int catalog_count = cat->count;                                                                                 
        for (int index = 0; index < catalog_count; index++) {                                                           
            fprintf(stdout, "\t<option value=%d>%s</option>\n", index, cat->stars[index].name);                         
        }                                                                                                               
    }
};
