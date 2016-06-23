#include "Util.h"

#include <string.h>
#include <string>
#include <stdio.h>

using namespace StarGen;

// OS-specific constants for finding the default output directory and other dirs:
#ifdef WIN32
#define DIRSEP    "\\"
#else
#define DIRSEP    "/"
#endif

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

    const char* Util::getLastSubdir(const char * path) {
        std::string subdir(path);
        std::size_t pos = subdir.find_last_of("/\\");

        if (pos == std::string::npos) {
            return subdir.c_str();
        }


        size_t l = strlen(DIRSEP);
        const char* s = path;
        const char* e = s + strlen(s) - l;

        if (e < s || (strcmp(e, DIRSEP) != 0)) {
            fprintf(stderr, "Invalid path: '%s'\n", path);
            throw;
        }

        for (;;) {
            const char* p = strstr(s, DIRSEP);

            if (p >= e) {
                break;
            }

            s = p + l;
        }

        subdir = std::string(s);
        subdir = subdir.substr(0, subdir.size() - l);
        subdir.append("/");

        return subdir.c_str();
    }
}
