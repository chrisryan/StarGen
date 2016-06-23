//
// StarGen Main Routine
//
// This file provides the main command-line interface to StarGen.
// Other platform-specific UIs can be created by duplicating its
// general functionality and then calling stargen(), whose API is
// defined in stargen.h
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "structs.h"
#include "stargen.h"

#include "Catalogs.h"
#include "Util.h"

using namespace StarGen;

void usage(const char *prognam) {
    fprintf(stderr, "Usage: %s [options] [system name]\n", prognam);
    fprintf(
        stderr,
        "  Options:\n"
        "    -s#  Set random number seed\n"
        "    -m#  Specify stellar mass\n"
        "    -n#  Specify number of systems\n"
        "    -i#  Number to increment each seed by\n"
        "    -x   Use the Solar System's masses/orbits\n"
        "    -d   Use Dole's %d nearby stars\n"
        "    -D#  Use Dole's system #n\n"
        "    -w   Use %d nearby stars taken from the Web\n"
        "    -W#  Use Web system #n\n"
        "    -p   Specify the path for the output file\n"
        "    -o   Output file name\n"
        "    -t   Text-only output\n"
        "    -v#  Set verbosity (hex value)\n"
        "    -l   List nearby stars and exit\n"
        "    -H   Output only systems with habitable planets\n"
        "    -2   Only systems with 2 or more habitable planets\n"
        "    -E   Only systems with earthlike planets\n"
        "\n"
        "  Experimental options (may go away):\n"
        "    -c   Output Celestia .ssc file on stdout\n"
        "    -e   Output Excel .csv file\n"
        "    -V   Create vector graphics (SVG) system image\n"
        "    -k   Incorporate known planets (incomplete)\n"
        "          (use only orbital data at present)\n"
        "          Without -k, -c skips systems with known planets.\n"
        "    -g   Show atmospheric gases\n"
        "    -Z   Dump tables used for gases and exit\n"
        "    -M   Do moons (highly experimental)\n"
        "\n"
        "        Nearest stars taken from:\n"
        "          http://www.solstation.com/stars.htm\n"
        "\n"
        "        StarGen: %s\n"
        "\n",
        dole.count - 1,
        solstation.count - 1,
        Stargen::version
    );
}

int main(int argc, char *argv[]) {
    char flag_char = '?';
    char path[300] = "html";
    char url_path_arg[300] = "";
    char filename_arg[300] = "";
    char arg_name [80] = "";

    bool use_stdout = false;
    const char * prognam;
    catalog * catalog = NULL;

    char * c = NULL;
    bool skip = false;

    bool listCatalog = false;
    bool listCatalogAsHTML = false;

    prognam = Util::getLastSubdir(argv[0]);

    if (argc <= 1) {
        usage(prognam);
        return(1);
    }

    Stargen *oStargen = new Stargen();
    oStargen->setProgramName(prognam);

    while (--argc > 0 && (*++argv)[0] == '-') {
        for (c = argv[0]+1, skip=false; (*c != '\0') && (!(skip)); c++) {
            switch (*c) {
                case '-':
                    use_stdout = true;
                    break;
                case 's': // set random seed
                    oStargen->setSeed(atol(&(*++c)));
                    skip = true;
                    break;
                case 'm': // set mass of star
                {
                    double m; // gnu C doesn't like to scanf long doubles

                    sscanf (++c, "%lf", &m);
                    oStargen->setMass(m);

                    skip = true;
                    break;
                }
                case 'n': // number of systems
                    oStargen->setCount(atoi(&(*++c)));
                    skip = true;
                    break;
                case 'i': // number of systems
                    oStargen->setIncrement(atoi(&(*++c)));
                    skip = true;
                    break;
                case 'x': // Use the solar system
                    flag_char = *c;
                    oStargen->addFlag(fUseSolarsystem);
                    if (oStargen->getMass() == 0.0)
                        oStargen->setMass(1.0);
                    break;
                case 'a': // Use the solar system varying earth
                    flag_char = *c;
                    oStargen->addFlag(fReuseSolarsystem);
                    break;
                case 'D':
                    catalog = &dole;
                    flag_char = toupper(*c);
                    ++c;
                    if ((toupper(*c) != 'X') && (*c != '\0')) {
                        oStargen->setSystemNumber(atoi(c) + 1);
                    }

                    skip = true;
                    break;
                case 'W':
                    catalog = &solstation;
                    flag_char = toupper(*c);
                    ++c;
                    if ((toupper(*c) != 'X') && (*c != '\0')) {
                        oStargen->setSystemNumber(atoi(c) + 1);
                    }

                    skip = true;
                    break;
                case 'F':
                    catalog = &jimb;
                    flag_char = toupper(*c);
                    ++c;
                    if ((toupper(*c) != 'X') && (*c != '\0')) {
                        oStargen->setSystemNumber(atoi(c) + 1);
                    }

                    skip = true;
                    break;
                case 'f':
                    catalog = &jimb;
                    flag_char = toupper(*c);
                    break;
                case 'd':
                    catalog = &dole;
                    flag_char = toupper(*c);
                    break;
                case 'w':
                    catalog = &solstation;
                    flag_char = toupper(*c);
                    break;
                case 'b': // experimental catal (Manticore, Helios etc.)
                    catalog = &manticore_cat;
                    flag_char = toupper(*c);
                    break;
                case 'B':
                    catalog = &manticore_cat;
                    flag_char = toupper(*c);
                    ++c;
                    if ((toupper(*c) != 'X') && (*c != '\0')) {
                        oStargen->setSystemNumber(atoi(c) + 1);
                    }

                    skip = true;
                    oStargen->addFlag(fNoGenerate);
                    sphinx.greenhouse_effect = true;
                    break;
                case 'G':
                    catalog = &ilAqr_cat;
                    flag_char = toupper(*c);
                    ++c;
                    if ((toupper(*c) != 'X') && (*c != '\0')) {
                        oStargen->setSystemNumber(atoi(c) + 1);
                    }

                    skip = true;
                    oStargen->addFlag(fNoGenerate);
                    break;
                case 'o':
                    if (*++c == '\0') {
                        if (argc > 1) {
                            --argc;
                            c = (++argv)[0];
                        }
                    }

                    if (*c != '\0') {
                        strcpy(filename_arg, c);
                    }

                    skip = true;
                    break;
                case 't': // display text
                    oStargen->setOutputFormat(StarGen::TEXT);
                    break;
                case 'e':
                    oStargen->setOutputFormat(StarGen::CSV);
                    break;
                case 'C':
                    oStargen->setOutputFormat(StarGen::CSVdl);
                    break;
                case 'c':
                    oStargen->setOutputFormat(StarGen::CELESTIA);
                    break;
                case 'V':
                    oStargen->setGraphicFormat(StarGen::GIF);
                    break;
                case 'S':
                    oStargen->setGraphicFormat(StarGen::SVG);
                    oStargen->setOutputFormat(StarGen::fSVG);
                    break;
                case 'k':
                    oStargen->addFlag(fUseKnownPlanets);
                    break;
                case 'p':
                    if (*++c == '\0') {
                        if (argc > 1) {
                            --argc;
                            c = (++argv)[0];
                        }
                    }

                    if (*c != '\0') {
                        strcpy(path, c);
                    }

                    skip = true;
                    break;
                case 'u':
                    if (*++c == '\0') {
                        if (argc > 1) {
                            --argc;
                            c = (++argv)[0];
                        }
                    }

                    if (*c != '\0') {
                        strcpy(url_path_arg, c);
                    }

                    if (strcmp(url_path_arg + strlen(url_path_arg) - strlen("/"), "/") != 0) {
                        strncat (url_path_arg, "/", 80-strlen(url_path_arg));
                    }

                    skip = true;
                    break;
                case 'g':
                    oStargen->addFlag(fDoGases);
                    break;
                case 'v': // verbosity
                    if (!isdigit(*(c+1))) {
                        ListVerbosity();
                        return 1;
                    }

                    int flag_verbose;
                    sscanf (++c, "%x", &flag_verbose);
                    skip = true;
                    Stargen::setVerbosity(flag_verbose);
                    if (Stargen::isVerbose(0x0001)) {
                        oStargen->addFlag(fDoGases);
                    }

                    break;
                case 'l':
                    listCatalog = true;
                    if (catalog == NULL) {
                        catalog = &dole;
                    }

                    break;
                case 'L':
                    listCatalogAsHTML = true;
                    if (catalog == NULL) {
                        catalog = &dole;
                    }

                    break;
                case 'z':
                    StarGen::SizeCheck();
                    return 1;
                    break;
                case 'Z':
                    StarGen::ListGases();
                    return 1;
                    break;
                case 'M':
                    oStargen->addFlag(fDoMoons);
                    break;
                case 'H':
                    oStargen->addFlag(fDoGases);
                    oStargen->addFlag(fOnlyHabitable);
                    break;
                case '2':
                    oStargen->addFlag(fDoGases);
                    oStargen->addFlag(fOnlyMultiHabitable);
                    break;
                case 'J':
                    oStargen->addFlag(fDoGases);
                    oStargen->addFlag(fOnlyJovianHabitable);
                    break;
                case 'E':
                    oStargen->addFlag(fDoGases);
                    oStargen->addFlag(fOnlyEarthlike);
                    break;
                case 'A':
                {
                    double ratio;

                    sscanf (++c, "%lf", &ratio);
                    skip = true;

                    if (ratio > 0.0) {
                        oStargen->setRatio(ratio);
                    }

                    break;
                }

                default:
                    fprintf (stderr, "Unknown option: %s\n", c);
                case '?':
                case 'h':
                    usage(prognam);
                    return 1;
            }
        }
    }

    for (int index = 0; index < argc; index++) {
        if ((strlen(argv[index]) + strlen(arg_name)) < sizeof(arg_name)) {
            if (strlen(arg_name)) {
                strcpy(arg_name+strlen(arg_name), " ");
            }

            strcpy(arg_name+strlen(arg_name), argv[index]);
        }
    }

    if (listCatalog) {
        StarGen::ListCatalog(catalog);
        return 1;
    }

    if (listCatalogAsHTML) {
        StarGen::ListCatalogHTML(catalog);
        return 1;
    }

    oStargen->setCatalog(catalog);
    oStargen->setFlagChar(flag_char);
    oStargen->setOutputPath(path);
    oStargen->generate(url_path_arg, filename_arg, arg_name, use_stdout ? stdout : NULL);

    return 0;
}
