#pragma once

namespace StarGen {
    class Gas {
        public:
            int num;
            long double surf_pressure; /* units of millibars (mb) */

            static int diminishing_pressure(const void *xp, const void *yp);

            Gas(int num = 0, long double surf_pressure = 0);

            long double inspired_partial_pressure(long double surf_pressure);
    };

}
