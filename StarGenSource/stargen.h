#pragma once
/*
 *	StarGen main API
 */

namespace StarGen {

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

			int generate(
				char flag_char,
			 	char *path,			// OS path to where to write files
			 	char *url_path_arg,	// HTML path to parent of both the
 									//  directory named in 'path' and
 									//  the ref directory with images
			 	char *filename_arg,	// Output file name (optional)
			 	char *sys_name_arg,	// Human readble System name (opt.)

			 	FILE *sgOut,		// Main stream to write to
			 						//	Thumbnails will be written there
			 						//  for HTML format
			 	catalog *cat_arg	// A star catalog (see below)
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
            long double ratio_arg;
            int sys_no_arg;
            int incr_arg;
		 	int count_arg;
			long seed_arg;
            long flag_seed;
			long double	mass_arg;
            const char * progname;

            void initRandomGenerator();

            void generate_stellar_system(
                StarGen::Sun* sun,
                bool use_seed_system,
                planet_pointer seed_system,
                char flag_char,
                int sys_no,
                char *system_name,
                long double outer_planet_limit,
                bool do_moons
            );

            void calculate_gases(StarGen::Sun* sun, planet_pointer planet, char* planet_id);

            void generate_planets(
                StarGen::Sun* sun,
                bool random_tilt,
                char flag_char,
                int sys_no,
                char *system_name,
                bool do_moons
            );

            void generate_planet(
                planet_pointer planet,
                int planet_no,
                StarGen::Sun *sun,
                bool random_tilt,
                char *planet_id,
                bool do_moons,
                bool is_moon
            );

            void check_planet(planet_pointer planet, char *planet_id, int is_moon);
	};


	// helper functions to output the various information
	void ListGases();
	void ListVerbosity();
	void SizeCheck();
	void ListCatalog(catalog * cat);
	void ListCatalogHTML(catalog * cat);
};

/*
 * Values for Stargen::setFlags() and Stargen::addFlag()
 */
#define	fUseSolarsystem			0x0001
#define	fReuseSolarsystem		0x0002
#define	fUseKnownPlanets		0x0004
#define fNoGenerate				0x0008

#define	fDoGases				0x0010
#define	fDoMoons				0x0020

#define fOnlyHabitable			0x0100
#define fOnlyMultiHabitable		0x0200
#define fOnlyJovianHabitable	0x0400
#define fOnlyEarthlike			0x0800

										// Various statistics that are kept:
extern int 		    total_earthlike;
extern int 		    total_habitable;

extern long double	min_breathable_terrestrial_g;
extern long double	min_breathable_g;
extern long double	max_breathable_terrestrial_g;
extern long double	max_breathable_g;
extern long double	min_breathable_terrestrial_l;
extern long double	min_breathable_l;
extern long double	max_breathable_terrestrial_l;
extern long double	max_breathable_l;
extern long double	min_breathable_temp;
extern long double	max_breathable_temp;
extern long double	min_breathable_p;
extern long double	max_breathable_p;

										// OS-specific constants for finding
										// the default output directory and
										// other dirs:
#ifdef WIN32
#define	SUBDIR	"html\\"
#define DIRSEP	"\\"
#else
#define	SUBDIR	"html/"
#define DIRSEP	"/"
#endif
