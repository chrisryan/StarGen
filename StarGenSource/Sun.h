#pragma once

namespace StarGen {
    class Sun {
        public:
            long double luminosity;
            long double mass;
            long double life;
            long double age;
            long double r_ecosphere;
            const char *name;

            Sun(long double luminosity, long double mass, long double life,
                long double age, long double r_ecosphere, const char *name);
    };
}
