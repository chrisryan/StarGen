#pragma once
/*
 *    StarGen main API
 */

namespace StarGen {

    class StargenStat;

    /*
     * Output file formats
     */
    typedef enum {
        HTML,
        TEXT,
        CELESTIA,
        CSV,
        CSVdl,
        fSVG
    } OutputFormats;

    /*
     * Graphics output formats
     */
    typedef enum {
        GIF,
        SVG
    } GraphicFormats;

    class Stargen {
        public:
            static const char * version;


            Stargen();

            void setOutputFormat(OutputFormats of);
            void setGraphicFormat(GraphicFormats gf);

            void setFlags(int f);
            void addFlag(int f);
            bool isFlag(int f);

            // Change dust density (experimental)
            void setRatio(long double r);

            // Star within a catalog (0 = all)
            void setSystemNumber(int n);

             // Amount to increment seed by
            void setIncrement(int n);

            // Number of systems (or cats) to do
            void setCount(int n);

            void setSeed(long s);

            // Mass of star (not used with catalog)
            void setMass(long double m);

            long double getMass();

            void setProgramName(const char *name);

            void setCatalog(catalog * c);

            void setFlagChar(char c);

            int generate(
                 char *path,            // OS path to where to write files
                 char *url_path_arg,    // HTML path to parent of both the
                                     //  directory named in 'path' and
                                     //  the ref directory with images
                 char *filename_arg,    // Output file name (optional)
                 char *sys_name_arg,    // Human readble System name (opt.)

                 FILE *sgOut         // Main stream to write to
                                     //    Thumbnails will be written there
                                     //  for HTML format
            );

            /*
              * 0x0001 Earthlike count
              * 0x0002 Trace Min/max
              * 0x0004 List habitable
              * 0x0008 List Earth-like (and Sphinx-line)
              *
              * 0x0010 List Gases
              * 0x0020 Trace temp iterations
              * 0x0040 Gas lifetimes
              * 0x0080 List loss of accreted gas mass
              *
              * 0x0100 Injecting, collision
              * 0x0200 Checking..., Failed...
              * 0x0400 List binary info
              * 0x0800 List Gas Dwarfs etc.
              *
              * 0x1000 Moons
              * 0x2000 Oxygen poisoned
              * 0x4000 Trace gas %ages (whoops)
              * 0x8000 Jovians in habitable zone
              *
              * 0x10000 List type diversity
              * 0x20000 Trace Surface temp interations
              * 0x40000 Lunar orbits
              */
            static void setVerbosity(int value);
            static bool isVerbose(int value);

        private:
            static int flag_verbose;

            OutputFormats out_format;
            GraphicFormats graphic_format;
            int flags_arg;
            int sys_no_arg;
            int incr_arg;
             int count_arg;
            long seed_arg;
            long flag_seed;
            long double    mass_arg;
            const char * progname;
            long double dust_density_coeff;
            catalog * cat_arg;
            char flag_char;
            int type_counts[12];
            int type_count;
            int max_type_count;
            int habitable_jovians;

            StargenStat* stats;

            void initRandomGenerator();

            void generate_stellar_system(
                Sun* sun,
                bool use_seed_system,
                planet_pointer seed_system,
                int sys_no,
                char *system_name,
                long double outer_planet_limit
            );

            void calculate_gases(Sun* sun, planet_pointer planet, char* planet_id);

            void generate_planets(
                Sun* sun,
                bool random_tilt,
                int sys_no,
                char *system_name
            );

            void generate_planet(
                planet_pointer planet,
                int planet_no,
                Sun *sun,
                bool random_tilt,
                char *planet_id,
                bool is_moon
            );

            void count_planet(planet_pointer planet);
            void check_planet(planet_pointer planet, char *planet_id, int is_moon);
            void resetTypeCounts();
            void listBreathable(planet_pointer planet, const char* planet_id);
            void listStatistics();
            void listTypeDiversity(const char * system_name, int sys_no);
            void listHabitableJovians(Sun *sun, planet_pointer planet, const char* planet_id);
    };


    // helper functions to output the various information
    void ListGases();
    void ListVerbosity();
    void SizeCheck();
};

/*
 * Values for Stargen::setFlags() and Stargen::addFlag()
 */
#define    fUseSolarsystem            0x0001
#define    fReuseSolarsystem        0x0002
#define    fUseKnownPlanets        0x0004
#define fNoGenerate                0x0008

#define    fDoGases                0x0010
#define    fDoMoons                0x0020

#define fOnlyHabitable            0x0100
#define fOnlyMultiHabitable        0x0200
#define fOnlyJovianHabitable    0x0400
#define fOnlyEarthlike            0x0800

                                        // OS-specific constants for finding
                                        // the default output directory and
                                        // other dirs:
#ifdef WIN32
#define    SUBDIR    "html\\"
#define DIRSEP    "\\"
#else
#define    SUBDIR    "html/"
#define DIRSEP    "/"
#endif
