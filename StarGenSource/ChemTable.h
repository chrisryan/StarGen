#pragma once

//  And atomic numbers, for use in ChemTable indexes
#define AN_H    1
#define AN_HE   2
#define AN_N    7
#define AN_O    8
#define AN_F    9
#define AN_NE   10
#define AN_P    15
#define AN_CL   17
#define AN_AR   18
#define AN_BR   35
#define AN_KR   36
#define AN_I    53
#define AN_XE   54
#define AN_HG   80
#define AN_AT   85
#define AN_RN   86
#define AN_FR   87

#define AN_NH3  900
#define AN_H2O  901
#define AN_CO2  902
#define AN_O3   903
#define AN_CH4  904
#define AN_CH3CH2OH 905

namespace StarGen {
    class Element
    {
        public:
            int num;
            const char *symbol;
            const char *html_symbol;
            const char *name;
            long double weight;
            long double melt;
            long double boil;
            long double density;
            long double abunde;
            long double abunds;
            long double reactivity;
            long double max_ipp; // Max inspired partial pressure im millibars

            Element(int, const char *, const char *, const char *,
                long double, long double, long double, long double,
                long double, long double, long double, long double);
    };

    class Gases {
        public:
            static int max_gas;
            static Element gases[];

            static void initialize();
    };
}
