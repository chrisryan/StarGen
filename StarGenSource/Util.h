#pragma once

namespace StarGen {
    class Util {
        public:
            static bool between(long double, long double, long double, bool = true);
            static const char* getLastSubdir(const char* path);
    };
}
