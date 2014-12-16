/*
 *	StarGen API
 *
 *	This file provides the main program interface to StarGen.
 *	An example of calling it is the command-line interface defined in
 *	main.c.
 *
 *	$Id: stargen.c,v 1.43 2008/12/30 23:15:13 brons Exp $
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include	<ctype.h>

#include	"const.h"
#include	"structs.h"
#include 	"accrete.h"
#include	"enviro.h"
#include	"display.h"
#include	"stargen.h"
#include	"utils.h"
#include "ChemTable.h"
#include "Catalogs.h"

namespace StarGen {
	const char * Stargen::version = "1.43";

	Stargen::Stargen()
	{
		this->out_format = HTML;
		this->graphic_format = GIF;
		this->flags_arg = 0;
        this->ratio_arg = 0.0;
        this->sys_no_arg = 0;

		StarGen::Gases::initialize();
	}

	void Stargen::setOutputFormat(OutputFormats of)
	{
		this->out_format = of;
	}

	void Stargen::setGraphicFormat(GraphicFormats gf)
	{
		this->graphic_format = gf;
	}

    void Stargen::setFlags(int f)
    {
        this->flags_arg = f;
    }

    void Stargen::addFlag(int f)
    {
        this->flags_arg |= f;
    }

    void Stargen::setRatio(long double r)
    {
        this->ratio_arg = r;
    }

    void Stargen::setSystemNumber(int n)
    {
        this->sys_no_arg = n;
    }

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
	int Stargen::flag_verbose = 0;

	void Stargen::setVerbosity(int value)
	{
		Stargen::flag_verbose = value;
	}

    bool Stargen::isVerbose(int value)
	{
		return (Stargen::flag_verbose & value);
	}

	void ListGases()
	{
		long double total = 0.0;

		StarGen::Gases::initialize();

		for (int index = 0; index < StarGen::Gases::max_gas; index++)
		{
			if (StarGen::Gases::gases[index].weight >= AN_N && StarGen::Gases::gases[index].max_ipp < 1E9)
			{
				total += StarGen::Gases::gases[index].max_ipp;
			}

			fprintf(stdout,
					" %2d: %4s - %-13s %3.0f mb - %5.0Lf mb\n",
					index,
					StarGen::Gases::gases[index].symbol,
					StarGen::Gases::gases[index].name,
					StarGen::Gases::gases[index].num == AN_O ? MIN_O2_IPP : 0.0,
					StarGen::Gases::gases[index].max_ipp
			);
		}

		fprintf (stdout, "Total Max ipp: %5.0Lf\n", total);
		fprintf (stdout, "Max pressure: %5.0f atm\n", MAX_HABITABLE_PRESSURE);
	}

	void ListVerbosity()
	{
		fprintf(stdout,
				"Verbosity flags are hexidecimal numbers:\n"
				"\t0001\tEarthlike count\n"
				"\t0002\tTrace Min/Max\n"
				"\t0004\tList Earthlike\n"
				"\t\n"
				"\t0010\tList Gases\n"
				"\t0020\tTrace temp iterations\n"
				"\t0040\tGas lifetimes\n"
				"\t0080\tList loss of accreted gas mass\n"
				"\t\n"
				"\t0100\tInjecting, collision\n"
				"\t0200\tChecking..., Failed...\n"
				"\t0400\tList binary info\n"
				"\t0800\tList accreted atmospheres\n"
				"\t\n"
				"\t1000\tMoons (experimental)\n"
				"\t2000\tOxygen poisoned (experimental)\n"
				"\t4000\tTrace gas percentages\n"
				"\t8000\tList Jovians in the ecosphere\n"
				"\t\n"
				"\t10000\tList type diversity\n"
				"\t20000\tTrace Surface temp interations\n"
		);
	}

	void SizeCheck()
	{
		long double	temp = est_temp(1.0, 1.0,  EARTH_ALBEDO);

		fprintf(stdout, "Size of float: %u\n", sizeof(float));
		fprintf(stdout, "Size of doubles: %u\n", sizeof(double));
		fprintf(stdout, "Size of long doubles: %u\n\n", sizeof(long double));
		fprintf(stdout, "Earth Eff Temp: %5.1Lf K, %5.1Lf C, Earth rel: %5.1Lf C\n\n",
				temp,
				temp - FREEZING_POINT_OF_WATER,
				temp - EARTH_AVERAGE_KELVIN);
	}

	void ListCatalog(catalog * cat)
	{
		int catalog_count = cat->count;
		for (int index = 0; index < catalog_count; index++)
		{
			fprintf(stdout,
					"%3d: %-30.30s M: %4.2LG L: %4.2LG\n",
					index,
					cat->stars[index].name,
					cat->stars[index].mass,
					cat->stars[index].luminosity
					);
		}
	}

	void ListCatalogHTML(catalog * cat)
	{
		int catalog_count = cat->count;
		for (int index = 0; index < catalog_count; index++)
		{
			fprintf(stdout,
					"\t<option value=%d>%s</option>\n",
					index,
					cat->stars[index].name
					);
		}
	}
};

/*  These are the global variables used during accretion:  */
planet_pointer	innermost_planet;
long double		dust_density_coeff = DUST_DENSITY_COEFF;


long flag_seed		 = 0;

int earthlike		 = 0;
int total_earthlike	 = 0;
int habitable		 = 0;
int habitable_jovians= 0;
int total_habitable	 = 0;

long double	min_breathable_terrestrial_g = 1000.0;
long double	min_breathable_g			 = 1000.0;
long double	max_breathable_terrestrial_g = 0.0;
long double	max_breathable_g			 = 0.0;
long double	min_breathable_temp			 = 1000.0;
long double	max_breathable_temp			 = 0.0;
long double	min_breathable_p			 = 100000.0;
long double	max_breathable_p			 = 0.0;
long double	min_breathable_terrestrial_l = 1000.0;
long double	min_breathable_l			 = 1000.0;
long double	max_breathable_terrestrial_l = 0.0;
long double	max_breathable_l			 = 0.0;
long double max_moon_mass				 = 0.0;

int type_counts[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int	type_count = 0;
int max_type_count = 0;

void generate_planets(StarGen::Sun*, bool, char, int, char *, bool, bool);

void init()
{
	if (!flag_seed)
	{
		time_t		temp_time;
		unsigned	seed = (unsigned)(time(&temp_time));

		(void)srand(seed);

		flag_seed = rand();
	}

	(void)srand(flag_seed);
}

void generate_stellar_system(StarGen::Sun* sun,
							 bool 			use_seed_system,
							 planet_pointer seed_system,
							 char			flag_char,
							 int			sys_no,
							 char*			system_name,
							 long double 	outer_planet_limit,
							 bool			do_gases,
							 bool			do_moons)
{
	long double		outer_dust_limit;

	if ((sun->mass < 0.2) || (sun->mass > 1.5))
		sun->mass		 = StarGen::Random::number(0.7, 1.4);

	outer_dust_limit	 = stellar_dust_limit(sun->mass);

	if (sun->luminosity == 0)
		sun->luminosity	 = luminosity(sun->mass);

	sun->r_ecosphere	 = sqrt(sun->luminosity);
	sun->life			 = 1.0E10 * (sun->mass / sun->luminosity);

	if (use_seed_system)
	{
		innermost_planet = seed_system;
		sun->age = 5.0E9;
	}
	else
	{
		long double min_age = 1.0E9;
		long double max_age = 6.0E9;

		if (sun->life < max_age)
			max_age = sun->life;

		innermost_planet = dist_planetary_masses(sun->mass,
												 sun->luminosity,
												 0.0,outer_dust_limit,
												 outer_planet_limit,
												 dust_density_coeff,
												 seed_system,
												 do_moons);

		sun->age = StarGen::Random::number(min_age, max_age);
	}

	generate_planets(sun,
					 !use_seed_system,
					 flag_char,
					 sys_no,
					 system_name,
					 do_gases,
					 do_moons);
}

void calculate_gases(StarGen::Sun* sun,
					 planet_pointer	planet,
					 char*			planet_id)
{
	if (planet->surf_pressure > 0)
	{
		long double	*amount = (long double*)calloc((StarGen::Gases::max_gas + 1), sizeof(long double));
		long double	totamount = 0;
		long double pressure  = planet->surf_pressure/MILLIBARS_PER_BAR;
		int			n         = 0;
		int			i;

		for (i = 0; i < StarGen::Gases::max_gas; i++)
		{
			long double yp = StarGen::Gases::gases[i].boil /
							 (373. * ((log((pressure) + 0.001) / -5050.5) +
									 (1.0 / 373.)));

			if ((yp >= 0 && yp < planet->low_temp)
			 && (StarGen::Gases::gases[i].weight >= planet->molec_weight))
			{
				long double	vrms	= rms_vel(StarGen::Gases::gases[i].weight, planet->exospheric_temp);
				long double	pvrms	= pow(1 / (1 + vrms / planet->esc_velocity), sun->age / 1e9);
				long double	abund	= StarGen::Gases::gases[i].abunds; 				/* gases[i].abunde */
				long double react	= 1.0;
				long double fract	= 1.0;
				long double pres2	= 1.0;

				if (strcmp(StarGen::Gases::gases[i].symbol, "Ar") == 0)
				{
					react = .15 * sun->age/4e9;
				}
				else if (strcmp(StarGen::Gases::gases[i].symbol, "He") == 0)
				{
					abund = abund * (0.001 + (planet->gas_mass / planet->mass));
					pres2 = (0.75 + pressure);
					react = pow(1 / (1 + StarGen::Gases::gases[i].reactivity),
								sun->age/2e9 * pres2);
				}
				else if ((strcmp(StarGen::Gases::gases[i].symbol, "O") == 0 ||
						  strcmp(StarGen::Gases::gases[i].symbol, "O2") == 0) &&
						 sun->age > 2e9 &&
						 planet->surf_temp > 270 && planet->surf_temp < 400)
				{
				/*	pres2 = (0.65 + pressure/2);			Breathable - M: .55-1.4 	*/
					pres2 = (0.89 + pressure/4);		/*	Breathable - M: .6 -1.8 	*/
					react = pow(1 / (1 + StarGen::Gases::gases[i].reactivity),
								pow(sun->age/2e9, 0.25) * pres2);
				}
				else if (strcmp(StarGen::Gases::gases[i].symbol, "CO2") == 0 &&
						 sun->age > 2e9 &&
						 planet->surf_temp > 270 && planet->surf_temp < 400)
				{
					pres2 = (0.75 + pressure);
					react = pow(1 / (1 + StarGen::Gases::gases[i].reactivity),
								pow(sun->age/2e9, 0.5) * pres2);
					react *= 1.5;
				}
				else
				{
					pres2 = (0.75 + pressure);
					react = pow(1 / (1 + StarGen::Gases::gases[i].reactivity),
								sun->age/2e9 * pres2);
				}

				fract = (1 - (planet->molec_weight / StarGen::Gases::gases[i].weight));

				amount[i] = abund * pvrms * react * fract;

				if (StarGen::Stargen::isVerbose(0x4000) &&
					(strcmp(StarGen::Gases::gases[i].symbol, "O") == 0 ||
					 strcmp(StarGen::Gases::gases[i].symbol, "N") == 0 ||
					 strcmp(StarGen::Gases::gases[i].symbol, "Ar") == 0 ||
					 strcmp(StarGen::Gases::gases[i].symbol, "He") == 0 ||
					 strcmp(StarGen::Gases::gases[i].symbol, "CO2") == 0))
				{
					fprintf (stderr, "%-5.2Lf %-3.3s, %-5.2Lf = a %-5.2Lf * p %-5.2Lf * r %-5.2Lf * p2 %-5.2Lf * f %-5.2Lf\t(%.3Lf%%)\n",
							  planet->mass * SUN_MASS_IN_EARTH_MASSES,
							  StarGen::Gases::gases[i].symbol,
							  amount[i],
							  abund,
							  pvrms,
							  react,
							  pres2,
							  fract,
							  100.0 * (planet->gas_mass / planet->mass)
							 );
				}

				totamount += amount[i];
				if (amount[i] > 0.0)
					n++;
			}
			else
				amount[i] = 0.0;
		}

		if (n > 0)
		{
			planet->gases = n;
			planet->atmosphere = new StarGen::Gas[n];

			for (i = 0, n = 0; i < StarGen::Gases::max_gas; i++)
			{
				if (amount[i] > 0.0)
				{
					planet->atmosphere[n].num = StarGen::Gases::gases[i].num;
					planet->atmosphere[n].surf_pressure = planet->surf_pressure
														* amount[i] / totamount;

					if (StarGen::Stargen::isVerbose(0x2000))
					{
						if ((planet->atmosphere[n].num == AN_O) &&
							inspired_partial_pressure (planet->surf_pressure,
													   planet->atmosphere[n].surf_pressure)
							> StarGen::Gases::gases[i].max_ipp)
						{
							fprintf (stderr, "%s\t Poisoned by O2\n",
									 planet_id);
						}
					}

					n++;
				}
			}

			qsort(planet->atmosphere,
				  planet->gases,
				  sizeof(StarGen::Gas),
				  StarGen::Gas::diminishing_pressure);

			if (StarGen::Stargen::isVerbose(0x0010))
			{
				fprintf (stderr, "\n%s (%5.1Lf AU) gases:\n",
						planet_id, planet->a);

				for (i = 0; i < planet->gases; i++)
				{
					fprintf (stderr, "%3d: %6.1Lf, %11.7Lf%%\n",
							planet->atmosphere[i].num,
							planet->atmosphere[i].surf_pressure,
							100. * (planet->atmosphere[i].surf_pressure /
									planet->surf_pressure)
							);
				}
			}
		}

		free(amount);
	}
}

void generate_planet(planet_pointer	planet,
					 int			planet_no,
					 StarGen::Sun* sun,
					 bool 			random_tilt,
					 char*			planet_id,
					 bool			do_gases,
					 bool			do_moons,
					 bool			is_moon)
{
		planet->atmosphere		= NULL;
		planet->gases			= 0;
		planet->surf_temp		= 0;
		planet->high_temp		= 0;
		planet->low_temp		= 0;
		planet->max_temp		= 0;
		planet->min_temp		= 0;
		planet->greenhs_rise	= 0;
		planet->planet_no		= planet_no;
		planet->sun				= sun;
		planet->resonant_period = false;

		planet->orbit_zone 		= orb_zone(sun->luminosity, planet->a);

		planet->orb_period		= period(planet->a,planet->mass,sun->mass);
		if (random_tilt)
			planet->axial_tilt 	= inclination(planet->a);
		planet->exospheric_temp = EARTH_EXOSPHERE_TEMP / pow2(planet->a / sun->r_ecosphere);
		planet->rms_velocity 	= rms_vel(MOL_NITROGEN,planet->exospheric_temp);
		planet->core_radius 	= kothari_radius(planet->dust_mass,false,planet->orbit_zone);

		// Calculate the radius as a gas giant, to verify it will retain gas.
		// Then if mass > Earth, it's at least 5% gas and retains He, it's
		// some flavor of gas giant.

		planet->density 		= empirical_density(planet->mass,planet->a, sun->r_ecosphere, true);
		planet->radius 			= volume_radius(planet->mass,planet->density);

		planet->surf_accel   	= acceleration(planet->mass,planet->radius);
		planet->surf_grav 	 	= gravity(planet->surf_accel);

		planet->molec_weight	= min_molec_weight(planet);

		if (((planet->mass * SUN_MASS_IN_EARTH_MASSES) > 1.0)
		  && ((planet->gas_mass / planet->mass)        > 0.05)
		  && (min_molec_weight(planet)				  <= 4.0))
		{
			if ((planet->gas_mass / planet->mass) < 0.20)
				planet->type = tSubSubGasGiant;
			else if ((planet->mass * SUN_MASS_IN_EARTH_MASSES) < 20.0)
				planet->type = tSubGasGiant;
			else
				planet->type = tGasGiant;
		}
		else // If not, it's rocky.
		{
			planet->radius 		= kothari_radius(planet->mass,false,planet->orbit_zone);
			planet->density 	= volume_density(planet->mass,planet->radius);

			planet->surf_accel  = acceleration(planet->mass,planet->radius);
			planet->surf_grav 	= gravity(planet->surf_accel);

			if ((planet->gas_mass / planet->mass)        > 0.000001)
			{
				long double h2_mass = planet->gas_mass * 0.85;
				long double he_mass = (planet->gas_mass - h2_mass) * 0.999;

				long double h2_loss = 0.0;
				long double he_loss = 0.0;

				long double h2_life = gas_life(MOL_HYDROGEN, planet);
				long double he_life = gas_life(HELIUM, planet);

				if (h2_life < sun->age)
				{
					h2_loss = ((1.0 - (1.0 / exp(sun->age / h2_life))) * h2_mass);

					planet->gas_mass -= h2_loss;
					planet->mass     -= h2_loss;

					planet->surf_accel  = acceleration(planet->mass,planet->radius);
					planet->surf_grav 	= gravity(planet->surf_accel);
				}

				if (he_life < sun->age)
				{
					he_loss = ((1.0 - (1.0 / exp(sun->age / he_life))) * he_mass);

					planet->gas_mass -= he_loss;
					planet->mass     -= he_loss;

					planet->surf_accel  = acceleration(planet->mass,planet->radius);
					planet->surf_grav 	= gravity(planet->surf_accel);
				}

				if (((h2_loss + he_loss) > .000001) && (StarGen::Stargen::isVerbose(0x0080)))
					fprintf (stderr, "%s\tLosing gas: H2: %5.3Lf EM, He: %5.3Lf EM\n",
							 planet_id,
							 h2_loss * SUN_MASS_IN_EARTH_MASSES, he_loss * SUN_MASS_IN_EARTH_MASSES);
			}
		}

		planet->day = day_length(planet);	/* Modifies planet->resonant_period */
		planet->esc_velocity 	= escape_vel(planet->mass,planet->radius);

		if ((planet->type == tGasGiant)
		 || (planet->type == tSubGasGiant)
		 || (planet->type == tSubSubGasGiant))
		{
			planet->greenhouse_effect 		= false;
			planet->volatile_gas_inventory 	= INCREDIBLY_LARGE_NUMBER;
			planet->surf_pressure 			= INCREDIBLY_LARGE_NUMBER;

			planet->boil_point 				= INCREDIBLY_LARGE_NUMBER;

			planet->surf_temp				= INCREDIBLY_LARGE_NUMBER;
			planet->greenhs_rise 			= 0;
			planet->albedo 					= StarGen::Random::about(GAS_GIANT_ALBEDO,0.1);
			planet->hydrosphere 			= 1.0;
			planet->cloud_cover	 			= 1.0;
			planet->ice_cover	 			= 0.0;
			planet->surf_grav			 	= gravity(planet->surf_accel);
			planet->molec_weight			= min_molec_weight(planet);
			planet->surf_grav 				= INCREDIBLY_LARGE_NUMBER;
 			planet->estimated_temp			= est_temp(sun->r_ecosphere, planet->a,  planet->albedo);
			planet->estimated_terr_temp		= est_temp(sun->r_ecosphere, planet->a,  EARTH_ALBEDO);

			{
				long double temp = planet->estimated_terr_temp;

				if ((temp >= FREEZING_POINT_OF_WATER)
				 && (temp <= EARTH_AVERAGE_KELVIN + 10.)
				 && (sun->age > 2.0E9))
				{
					habitable_jovians++;

					if (StarGen::Stargen::isVerbose(0x8000))
					{
						fprintf (stderr, "%s\t%s (%4.2LfEM %5.3Lf By)%s with earth-like temperature (%.1Lf C, %.1Lf F, %+.1Lf C Earth).\n",
								 planet_id,
								 planet->type == tGasGiant ? "Jovian" :
								 planet->type == tSubGasGiant ? "Sub-Jovian" :
								 planet->type == tSubSubGasGiant ? "Gas Dwarf" :
								 "Big",
								 planet->mass * SUN_MASS_IN_EARTH_MASSES,
								 sun->age /1.0E9,
								 planet->first_moon == NULL ? "" : " WITH MOON",
								 temp - FREEZING_POINT_OF_WATER,
								 32 + ((temp - FREEZING_POINT_OF_WATER) * 1.8),
								 temp - EARTH_AVERAGE_KELVIN);
					}
				}
			}
		}
		else
		{
			planet->estimated_temp			= est_temp(sun->r_ecosphere, planet->a,  EARTH_ALBEDO);
			planet->estimated_terr_temp		= est_temp(sun->r_ecosphere, planet->a,  EARTH_ALBEDO);

			planet->surf_grav 				= gravity(planet->surf_accel);
			planet->molec_weight			= min_molec_weight(planet);

			planet->greenhouse_effect 		= grnhouse(sun->r_ecosphere, planet->a);
			planet->volatile_gas_inventory 	= vol_inventory(planet->mass,
														    planet->esc_velocity,
														    planet->rms_velocity,
														    sun->mass,
														    planet->orbit_zone,
														    planet->greenhouse_effect,
														    (planet->gas_mass
														     / planet->mass) > 0.000001);
			planet->surf_pressure 			= pressure(planet->volatile_gas_inventory,
													   planet->radius,
													   planet->surf_grav);

			if ((planet->surf_pressure == 0.0))
				planet->boil_point 			= 0.0;
			else
				planet->boil_point 			= boiling_point(planet->surf_pressure);

			iterate_surface_temp(planet);		/*	Sets:
												 *		planet->surf_temp
												 *		planet->greenhs_rise
												 *		planet->albedo
												 *		planet->hydrosphere
												 *		planet->cloud_cover
												 *		planet->ice_cover
												 */

			if (do_gases &&
				(planet->max_temp >= FREEZING_POINT_OF_WATER) &&
				(planet->min_temp <= planet->boil_point))
				calculate_gases(sun, planet, planet_id);

			/*
			 *	Next we assign a type to the planet.
			 */

			if (planet->surf_pressure < 1.0)
			{
				if (!is_moon
				 && ((planet->mass * SUN_MASS_IN_EARTH_MASSES) < ASTEROID_MASS_LIMIT))
					planet->type 			= tAsteroids;
				else
					planet->type 			= tRock;
			}
			else if ((planet->surf_pressure > 6000.0) &&
					 (planet->molec_weight <= 2.0))	// Retains Hydrogen
			{
				planet->type = tSubSubGasGiant;
				planet->gases = 0;
				free(planet->atmosphere);
				planet->atmosphere = NULL;
			}
			else
			{										// Atmospheres:
				if (((int)planet->day == (int)(planet->orb_period * 24.0) ||
										 (planet->resonant_period)))
					planet->type = t1Face;
				else if (planet->hydrosphere >= 0.95)
					planet->type = tWater;				// >95% water
				else if (planet->ice_cover >= 0.95)
					planet->type = tIce;				// >95% ice
				else if (planet->hydrosphere > 0.05)
					planet->type = tTerrestrial;		// Terrestrial
														// else <5% water
				else if (planet->max_temp > planet->boil_point)
					planet->type = tVenusian;			// Hot = Venusian
				else if ((planet->gas_mass / planet->mass) > 0.0001)
				{										// Accreted gas
					planet->type = tIce;				// But no Greenhouse
					planet->ice_cover = 1.0;			// or liquid water
				}										// Make it an Ice World
				else if (planet->surf_pressure <= 250.0)// Thin air = Martian
					planet->type = tMartian;
				else if (planet->surf_temp < FREEZING_POINT_OF_WATER)
					planet->type = tIce;
				else
				{
					planet->type = tUnknown;

					if (StarGen::Stargen::isVerbose(0x0001))
						fprintf (stderr, "%12s\tp=%4.2Lf\tm=%4.2Lf\tg=%4.2Lf\tt=%+.1Lf\t%s\t Unknown %s\n",
										type_string (planet->type),
										planet->surf_pressure,
										planet->mass * SUN_MASS_IN_EARTH_MASSES,
										planet->surf_grav,
										planet->surf_temp  - EARTH_AVERAGE_KELVIN,
										planet_id,
										((int)planet->day == (int)(planet->orb_period * 24.0) ||
										 (planet->resonant_period)) ? "(1-Face)" : ""
								 );
				}
			}
		}

		if (do_moons && !is_moon)
		{
			if (planet->first_moon != NULL)
			{
				int 			n;
				planet_pointer	ptr;

				for (n=0, ptr=planet->first_moon;
					 ptr != NULL;
					 ptr=ptr->next_planet)
				{
					if (ptr->mass * SUN_MASS_IN_EARTH_MASSES > .000001)
					{
						char	moon_id[80];
						long double	roche_limit = 0.0;
						long double	hill_sphere = 0.0;

						ptr->a = planet->a;
						ptr->e = planet->e;

						n++;

						sprintf(moon_id,
								"%s.%d",
								planet_id, n);

						generate_planet(ptr, n,
										sun, random_tilt,
										moon_id,
										do_gases,
										do_moons, true);	// Adjusts ptr->density

						roche_limit = 2.44 * planet->radius * pow((planet->density / ptr->density), (1.0 / 3.0));
						hill_sphere = planet->a * KM_PER_AU * pow((planet->mass / (3.0 * sun->mass)), (1.0 / 3.0));

						if ((roche_limit * 3.0) < hill_sphere)
						{
							ptr->moon_a = StarGen::Random::number(roche_limit * 1.5, hill_sphere / 2.0) / KM_PER_AU;
							ptr->moon_e = StarGen::Random::eccentricity ();
						}
						else
						{
							ptr->moon_a = 0;
							ptr->moon_e = 0;
						}

						if (StarGen::Stargen::isVerbose(0x40000))
						{
							fprintf (stderr,
										"   Roche limit: R = %4.2Lg, rM = %4.2Lg, rm = %4.2Lg -> %.0Lf km\n"
										"   Hill Sphere: a = %4.2Lg, m = %4.2Lg, M = %4.2Lg -> %.0Lf km\n"
										"%s Moon orbit: a = %.0Lf km, e = %.0Lg\n",
										planet->radius, planet->density, ptr->density,
										roche_limit,
										planet->a * KM_PER_AU, planet->mass * SOLAR_MASS_IN_KILOGRAMS, sun->mass * SOLAR_MASS_IN_KILOGRAMS,
										hill_sphere,
										moon_id,
										ptr->moon_a * KM_PER_AU, ptr->moon_e
									);
						}

						if (StarGen::Stargen::isVerbose(0x1000))
						{
							fprintf (stderr, "  %s: (%7.2LfEM) %d %4.2LgEM\n",
								planet_id,
								planet->mass * SUN_MASS_IN_EARTH_MASSES,
								n,
								ptr->mass * SUN_MASS_IN_EARTH_MASSES);
						}
					}
				}
			}
		}

}

void check_planet(planet_pointer	planet,
				  char*				planet_id,
				  int				is_moon)
{
	{
		int tIndex = 0;

		switch (planet->type)
		{
			case tUnknown:			tIndex = 0;		break;
			case tRock:				tIndex = 1;		break;
			case tVenusian:			tIndex = 2;		break;
			case tTerrestrial:		tIndex = 3;		break;
			case tSubSubGasGiant:	tIndex = 4;		break;
			case tSubGasGiant:		tIndex = 5;		break;
			case tGasGiant:			tIndex = 6;		break;
			case tMartian:			tIndex = 7;		break;
			case tWater:			tIndex = 8;		break;
			case tIce:				tIndex = 9;		break;
			case tAsteroids: 		tIndex = 10;	break;
			case t1Face:			tIndex = 11;	break;
		}

		if (type_counts[tIndex] == 0)
			++type_count;

		++type_counts[tIndex];

	}

	/* Check for and list planets with breathable atmospheres */

	{
		Breathability breathe = breathability(planet);

		if ((breathe == Breathable) &&
			(!planet->resonant_period) &&		// Option needed?
			((int)planet->day != (int)(planet->orb_period * 24.0)))
		{
			int	list_it	= false;
			long double illumination = pow2 (1.0 / planet->a)
										* (planet->sun)->luminosity;

			habitable++;

			if (min_breathable_temp > planet->surf_temp)
			{
				min_breathable_temp = planet->surf_temp;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (max_breathable_temp < planet->surf_temp)
			{
				max_breathable_temp = planet->surf_temp;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (min_breathable_g > planet->surf_grav)
			{
				min_breathable_g = planet->surf_grav;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (max_breathable_g < planet->surf_grav)
			{
				max_breathable_g = planet->surf_grav;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (min_breathable_l > illumination)
			{
				min_breathable_l = illumination;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (max_breathable_l < illumination)
			{
				max_breathable_l = illumination;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (planet->type == tTerrestrial)
			{
				if (min_breathable_terrestrial_g > planet->surf_grav)
				{
					min_breathable_terrestrial_g = planet->surf_grav;

					if (StarGen::Stargen::isVerbose(0x0002))
						list_it = true;
				}

				if (max_breathable_terrestrial_g < planet->surf_grav)
				{
					max_breathable_terrestrial_g = planet->surf_grav;

					if (StarGen::Stargen::isVerbose(0x0002))
						list_it = true;
				}

				if (min_breathable_terrestrial_l > illumination)
				{
					min_breathable_terrestrial_l = illumination;

					if (StarGen::Stargen::isVerbose(0x0002))
						list_it = true;
				}

				if (max_breathable_terrestrial_l < illumination)
				{
					max_breathable_terrestrial_l = illumination;

					if (StarGen::Stargen::isVerbose(0x0002))
						list_it = true;
				}
			}

			if (min_breathable_p > planet->surf_pressure)
			{
				min_breathable_p = planet->surf_pressure;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (max_breathable_p < planet->surf_pressure)
			{
				max_breathable_p = planet->surf_pressure;

				if (StarGen::Stargen::isVerbose(0x0002))
					list_it = true;
			}

			if (StarGen::Stargen::isVerbose(0x0004))
				list_it = true;

			if (list_it)
			fprintf (stderr, "%12s\tp=%4.2Lf\tm=%4.2Lf\tg=%4.2Lf\tt=%+.1Lf\tl=%4.2Lf\t%s\n",
						type_string (planet->type),
						planet->surf_pressure,
						planet->mass * SUN_MASS_IN_EARTH_MASSES,
						planet->surf_grav,
						planet->surf_temp  - EARTH_AVERAGE_KELVIN,
						illumination,
						planet_id);
		}
	}

	if (is_moon
	 && max_moon_mass < planet->mass)
	{
		max_moon_mass = planet->mass;

		if (StarGen::Stargen::isVerbose(0x0002))
			fprintf (stderr, "%12s\tp=%4.2Lf\tm=%4.2Lf\tg=%4.2Lf\tt=%+.1Lf\t%s Moon Mass\n",
					type_string (planet->type),
					planet->surf_pressure,
					planet->mass * SUN_MASS_IN_EARTH_MASSES,
					planet->surf_grav,
					planet->surf_temp  - EARTH_AVERAGE_KELVIN,
					planet_id);
	}

	if (StarGen::Stargen::isVerbose(0x0800)
	 && (planet->dust_mass * SUN_MASS_IN_EARTH_MASSES >= 0.0006)
	 && (planet->gas_mass * SUN_MASS_IN_EARTH_MASSES >= 0.0006)
	 && (planet->type != tGasGiant)
	 && (planet->type != tSubGasGiant)
	   )
	{
		int core_size = (int)((50. * planet->core_radius) / planet->radius);

		if (core_size <= 49)
		{
			fprintf (stderr, "%12s\tp=%4.2Lf\tr=%4.2Lf\tm=%4.2Lf\t%s\t%d\n",
					type_string (planet->type),
					planet->core_radius,
					planet->radius,
					planet->mass * SUN_MASS_IN_EARTH_MASSES,
					planet_id,
					50-core_size);
		}
	}

	{
		long double  rel_temp   = (planet->surf_temp -  FREEZING_POINT_OF_WATER) -
								   EARTH_AVERAGE_CELSIUS;
		long double	 seas       = (planet->hydrosphere * 100.0);
		long double	 clouds     = (planet->cloud_cover * 100.0);
		long double	 pressure   = (planet->surf_pressure /
								   EARTH_SURF_PRES_IN_MILLIBARS);
		long double	 ice        = (planet->ice_cover * 100.0);
		long double	 gravity    = planet->surf_grav;
		Breathability breathe = breathability(planet);

		if ((gravity 	>= .8) &&
			(gravity 	<= 1.2) &&
			(rel_temp 	>= -2.0) &&
			(rel_temp 	<= 3.0) &&
			(ice 		<= 10.) &&
			(pressure   >= 0.5) &&
			(pressure   <= 2.0) &&
			(clouds		>= 40.) &&
			(clouds		<= 80.) &&
			(seas 		>= 50.) &&
			(seas 		<= 80.) &&
			(planet->type != tWater) &&
			(breathe == Breathable))
		{
			earthlike++;

			if (StarGen::Stargen::isVerbose(0x0008))
				fprintf (stderr, "%12s\tp=%4.2Lf\tm=%4.2Lf\tg=%4.2Lf\tt=%+.1Lf\t%d %s\tEarth-like\n",
								type_string (planet->type),
								planet->surf_pressure,
								planet->mass * SUN_MASS_IN_EARTH_MASSES,
								planet->surf_grav,
								planet->surf_temp  - EARTH_AVERAGE_KELVIN,
								habitable,
								planet_id);
		} else if (StarGen::Stargen::isVerbose(0x0008) &&
				 (breathe == Breathable) &&
				 (gravity	 > 1.3) &&
				 (habitable	 > 1) &&
				 ((rel_temp  < -2.0) ||
				  (ice		 > 10.))
				)
		{
			fprintf (stderr, "%12s\tp=%4.2Lf\tm=%4.2Lf\tg=%4.2Lf\tt=%+.1Lf\t%s\tSphinx-like\n",
					type_string (planet->type),
					planet->surf_pressure,
					planet->mass * SUN_MASS_IN_EARTH_MASSES,
					planet->surf_grav,
					planet->surf_temp  - EARTH_AVERAGE_KELVIN,
					planet_id);
		}
	}
}

void generate_planets(StarGen::Sun* sun,
					  bool 			random_tilt,
					  char			flag_char,
					  int			sys_no,
					  char*			system_name,
					  bool			do_gases,
					  bool			do_moons)
{
	planet_pointer	planet;
	int				planet_no = 0;
	planet_pointer 	moon;
	int 			moons = 0;

	for (planet = innermost_planet, planet_no = 1;
		 planet != NULL;
		 planet = planet->next_planet, planet_no++)
	{
		char	planet_id[80];

		sprintf(planet_id,
				"%s (-s%ld -%c%d) %d",
				system_name, flag_seed, flag_char, sys_no, planet_no);

		generate_planet(planet, planet_no,
						sun, random_tilt,
						planet_id,
						do_gases, do_moons, false);

		/*
		 *	Now we're ready to test for habitable planets,
		 *	so we can count and log them and such
		 */

		 check_planet(planet, planet_id, false);

		for (moon=planet->first_moon, moons=1;
			moon != NULL;
			moon=moon->next_planet, moons++)
		{
			char	moon_id[80];

			sprintf(moon_id,
					"%s.%d",
					planet_id, moons);
			check_planet(moon, moon_id, true);
		}
	}
}

namespace StarGen {
int Stargen::generate(
			 char			flag_char,
			 char *			path,
			 char *			url_path_arg,
			 char *			filename_arg,
			 char *			sys_name_arg,

			 FILE *			sgOut,
			 FILE *			sgErr,
			 const char *	prognam,
			 long double	mass_arg,
			 long			seed_arg,
			 int			count_arg,
			 int			incr_arg,
			 catalog *		cat_arg
			 )
{
	StarGen::Sun sun					= StarGen::Sun(0.0, 0.0, 0.0, 0.0, 0.0, "");
	long double		min_mass 			= 0.4;
	long double		inc_mass 			= 0.05;
	long double		max_mass 			= 2.35;
	int				system_count		= 1;
	int				seed_increment		= 1;

	char			default_path[]		= SUBDIR;			/* OS specific */
	char 			default_url_path[]	= "../";
	char 			*url_path			= default_url_path;
	char			thumbnail_file[300]	= "Thumbnails";
	char 			file_name[300]		= "StarGen";
	char			subdir[300]			= "";
	char			csv_file_name[300]	= "StarGen.csv";

	FILE 			*html_file			= NULL;
	FILE 			*thumbnails			= NULL;
	FILE			*csv_file			= NULL;

	int  			index				= 0;
	int				do_catalog			= ((cat_arg != NULL) && (this->sys_no_arg == 0));
	int				catalog_count		= 0;
	bool			do_gases			= (this->flags_arg & fDoGases) != 0;
	bool			use_solar_system	= (this->flags_arg & fUseSolarsystem) != 0;
	bool			reuse_solar_system	= (this->flags_arg & fReuseSolarsystem) != 0;
	bool			use_known_planets	= (this->flags_arg & fUseKnownPlanets) != 0;
	bool			no_generate			= (this->flags_arg & fNoGenerate) != 0;
	bool			do_moons			= (this->flags_arg & fDoMoons) != 0;
	bool			only_habitable		= (this->flags_arg & fOnlyHabitable) != 0;
	bool			only_multi_habitable= (this->flags_arg & fOnlyMultiHabitable) != 0;
	bool			only_jovian_habitable=(this->flags_arg & fOnlyJovianHabitable) != 0;
	bool			only_earthlike		= (this->flags_arg & fOnlyEarthlike) != 0;

	if (do_catalog)
		catalog_count = cat_arg->count;

	if (only_habitable && only_multi_habitable)
		only_habitable = false;

	if (only_habitable && only_earthlike)
		only_habitable = false;

	if (sgErr == NULL)
		sgErr = stderr;

	if ((prognam == NULL) || (prognam[0] == '\0'))
		prognam = "StarGen";

	if ((path == NULL) || (path[0] == '\0'))
		path 		= default_path;

	if ((url_path_arg != NULL) && (url_path_arg[0] != '\0'))
		url_path	= url_path_arg;

	{									// Find the last sub-dir in the path:
		size_t	l = strlen(DIRSEP);
		char*	s = path;
		char*	e = s + strlen(s) - l;

		if (e < s || (strcmp(e, DIRSEP) != 0))
		{
			fprintf (stderr, "Invalid path: '%s'\n", path);
			return 1;
		}

		for (;;)
		{
			char*	p = strstr(s, DIRSEP);

			if (p >= e)
				break;

			s = p + l;
		}

		strncpy (subdir, s, strlen(s) - l);
		strncat (subdir, "/", 80-strlen(subdir));
	}

	flag_seed		= seed_arg;
	sun.mass 		= mass_arg;
	system_count	= count_arg;
	seed_increment	= incr_arg;

	if (this->ratio_arg > 0.0)
		dust_density_coeff *= this->ratio_arg;

	if (reuse_solar_system)
	{
		system_count = 1 + (int) ((max_mass - min_mass) / inc_mass);

		earth.mass = (EM(min_mass));

		sun.luminosity 	= 1.0;
		sun.mass 		= 1.0;
		sun.life 		= 1.0E10;
		sun.age 		= 5.0E9;
		sun.r_ecosphere	= 1.0;

		use_solar_system = true;
	}
	else if (do_catalog)
	{
		system_count = catalog_count + ((system_count - 1) * (catalog_count - 1));
		use_solar_system = true;
	}

	if ((system_count > 1)
	 && !(this->out_format == CSVdl))
	{
		if (strlen(filename_arg) > 0)
			strcpy(thumbnail_file, filename_arg);

		thumbnails = open_html_file ("Thumbnails", flag_seed, path, url_path, thumbnail_file, ".html",
									 prognam, sgOut);
		if (thumbnails == NULL)
		{
			fprintf(sgErr, "Could not open file %s%s\n",
					path, thumbnail_file);
			exit(0);
		}
	}

	if ((this->out_format == CSV) || (this->out_format == CSVdl))
	{
		char	csv_url[300]	= "";

		if ((sgOut != NULL))
		{
			char sys_no[10] = "x";

			if (!do_catalog)
				sprintf(&sys_no[0], "%d", this->sys_no_arg-1);

			if (this->out_format == CSVdl)
				csv_file = sgOut;

			sprintf (&csv_url[0],
					 "/cgi-bin/StarGen.pl?Catalog=%s&Dole=%s&SolStation=%s&Mass=%LG&Output=%s&Seed=%ld&Count=%d&Incr=%d&Gas=%s&Moon=%s&SVG=%s&DoIt=CSV",
					 (cat_arg == NULL) ? "none" : cat_arg->arg,
					 sys_no,
					 sys_no,
					 sun.mass,
					 (only_earthlike) ? "E"
					 : (only_multi_habitable) ? "2"
					 : (only_habitable) ? "H"
					 : "all",
					 flag_seed,
					 count_arg,
					 incr_arg,
					 (do_gases)					? "on" : "off",	// one of ("on", "off")
					 (do_moons)					? "on" : "off",	// one of ("on", "off")
					 (this->graphic_format == SVG)	? "on" : "off"	// one of ("on", "off")
					);
		}
		else
		{
			char cleaned_arg[300] = "StarGen";

			if (strlen(filename_arg) > 0)
			{
				char *ptr;

				strcpy (cleaned_arg, filename_arg);

				ptr = strrchr(cleaned_arg, '.');

				if ((ptr != NULL)
				 && (strcmp(ptr, ".html") == 0))
					*ptr = '\0';
			}

			if (thumbnails != NULL)
			{
				sprintf (&csv_file_name[0],
						 "%s-%ld.csv",
						 cleaned_arg,
						 flag_seed);
			}
			else
			{
				sprintf (&csv_file_name[0],
						 "%s.csv",
						 cleaned_arg);
			}

			sprintf (&csv_url[0],
					 "%s%s%s",
					 url_path,
					 subdir,
					 csv_file_name);

			csv_file = open_csv_file (path, csv_file_name);
		}

		if ((csv_file == NULL) &&
			!((this->out_format == CSV) && (sgOut != NULL)))
		{
			fprintf(sgErr, "Could not open file %s%s\n",
				path, csv_file_name);
			exit(0);
		}

		if (thumbnails != NULL)
			csv_thumbnails(thumbnails, url_path, subdir, csv_file_name, csv_url);
	}

	for (index = 0; index < system_count; index++)
	{
		char			system_name[80];
		char			designation[80];
		char			*cp;
		long double		outer_limit			= 0.0;
		int 			sys_no 				= 0;
		bool			has_known_planets 	= false;
		planet_pointer	seed_planets 		= NULL;
		bool			use_seed_system		= false;
		bool			in_celestia 		= false;

		init();

		if (do_catalog || this->sys_no_arg)
		{
			if (this->sys_no_arg)
				sys_no = this->sys_no_arg - 1;
			else
			{
				if (index >= catalog_count)
					sys_no = ((index - 1) % (catalog_count - 1)) + 1 ;
				else
					sys_no = index;
			}

			if (cat_arg->stars[sys_no].known_planets != NULL)
				has_known_planets = true;

			if (use_known_planets || no_generate)
			{
				seed_planets = cat_arg->stars[sys_no].known_planets;

				use_seed_system	= no_generate;
			}
			else
			{
				seed_planets = NULL;
			}

			in_celestia = cat_arg->stars[sys_no].in_celestia;

			sun.mass = cat_arg->stars[sys_no].mass;
			sun.luminosity = cat_arg->stars[sys_no].luminosity;

			if (do_catalog || sys_name_arg[0] == '\0')
			{
				sprintf (&system_name[0], "%s", cat_arg->stars[sys_no].name);
				sprintf (&designation[0], "%s", cat_arg->stars[sys_no].desig);

			}
			else
			{
				sprintf (&system_name[0], "%s", sys_name_arg);
				sprintf (&designation[0], "%s", sys_name_arg);
			}

			sprintf (&file_name[0], "%s-%ld", designation, flag_seed);

			if (cat_arg->stars[sys_no].m2 > .001)
			{
				/*
				 *	The following is Holman & Wiegert's equation 1 from
				 *	Long-Term Stability of Planets in Binary Systems
				 *	The Astronomical Journal, 117:621-628, Jan 1999
				 */
				long double m1 = sun.mass;
				long double m2 = cat_arg->stars[sys_no].m2;
				long double mu = m2 / (m1 + m2);
				long double e = cat_arg->stars[sys_no].e;
				long double a = cat_arg->stars[sys_no].a;

				outer_limit = (0.464 + (-0.380 * mu) + (-0.631 * e) +
							   (0.586 * mu * e) + (0.150 * pow2(e)) +
							   (-0.198 * mu * pow2(e))) * a;
			}
			else
				outer_limit = 0.0;
		}
		else if (reuse_solar_system)
		{
			sprintf (&system_name[0], "Earth-M%LG", earth.mass * SUN_MASS_IN_EARTH_MASSES);
			sprintf (&designation[0], "Earth-M%LG", earth.mass * SUN_MASS_IN_EARTH_MASSES);
			sprintf (&file_name[0], "Earth-M%LG", earth.mass * SUN_MASS_IN_EARTH_MASSES);

			outer_limit = 0.0;
		}
		else
		{
			if (sys_name_arg[0])
			{
				sprintf (&system_name[0], "%s", sys_name_arg);
				sprintf (&designation[0], "%s", sys_name_arg);
			}
			else
			{
				sprintf (&system_name[0], "%s %ld-%LG", prognam, flag_seed, sun.mass);
				sprintf (&designation[0], "%s", prognam);
			}

			sprintf (&file_name[0], "%s-%ld-%LG", designation, flag_seed, sun.mass);
			outer_limit = 0;
		}

		sun.name = system_name;

		if (StarGen::Stargen::isVerbose(0x0400) && (outer_limit > 0.0))
		{
			fprintf (sgErr, "%s, Outer Limit: %LG\n", system_name, outer_limit);
		}

		if ((system_count == 1) && (strlen(filename_arg) > 0))
			strcpy(file_name, filename_arg);

		while ((cp = strchr(file_name,' ')) != 0)
			*cp = '-';

		while ((cp = strchr(file_name,'\'')) != 0)
			*cp = '-';

		earthlike			 = 0;
		habitable			 = 0;
		habitable_jovians	 = 0;

		if (reuse_solar_system)
		{
			seed_planets	= solar_system;
			use_seed_system	= true;
		}
		else if (use_solar_system)
		{
			if  (index == 0)
			{
				seed_planets	= solar_system;
				use_seed_system	= true;
			}
			else
			{
				use_seed_system	= false;

				if (!use_known_planets)
					seed_planets = NULL;
			}
		}

		{
			int	i;

			for (i = 0; i < 12; i++)
				type_counts[i] = 0;

			type_count = 0;
		}

		generate_stellar_system(&sun,
								use_seed_system,
								seed_planets,	// solar_system
								flag_char,
								sys_no,
								system_name,
								outer_limit,
								do_gases,
								do_moons);

		{
			planet_pointer	planet;
			int 			counter;
			int				wt_type_count = type_count;
			int				norm_type_count = 0;

			if (type_counts[3]  > 0)	wt_type_count += 20;	// Terrestrial
			if (type_counts[8]  > 0)	wt_type_count += 18;	// Water
			if (type_counts[2]  > 0)	wt_type_count += 16;	// Venusian
			if (type_counts[7]  > 0)	wt_type_count += 15;	// Martian
			if (type_counts[9]  > 0)	wt_type_count += 14;	// Ice
			if (type_counts[10] > 0)	wt_type_count += 13;	// Asteroids
			if (type_counts[4]  > 0)	wt_type_count += 12;	// Gas Dwarf
			if (type_counts[5]  > 0)	wt_type_count += 11;	// Sub_Jovian
			if (type_counts[11] > 0)	wt_type_count += 10;	// 1-Face
			if (type_counts[1]  > 0)	wt_type_count += 3;		// Rock
			if (type_counts[6]  > 0)	wt_type_count += 2;		// Jovian
			if (type_counts[0]  > 0)	wt_type_count += 1;		// Unknown

			for (planet=innermost_planet, counter=0;
				planet != NULL;
				planet=planet->next_planet, counter++)
				;

			norm_type_count = wt_type_count - (counter - type_count);

			if (max_type_count < norm_type_count)
			{
				max_type_count = norm_type_count;

				if (StarGen::Stargen::isVerbose(0x10000))
					fprintf (sgErr, "System %ld - %s (-s%ld -%c%d) has %d types out of %d planets. [%d]\n",
							flag_seed,
							system_name,
							flag_seed,
							flag_char,
							sys_no,
							type_count,
							counter,
							norm_type_count);
			}
		}

		total_habitable += habitable;
		total_earthlike += earthlike;

		if ((!(only_habitable || only_multi_habitable || only_jovian_habitable || only_earthlike))
		 || (only_habitable && (habitable > 0))
		 || (only_multi_habitable && (habitable > 1))
		 || (only_jovian_habitable && (habitable_jovians > 0))
		 || (only_earthlike && (earthlike > 0))
		 )
		{
			char	system_url[300] = "";
			char	svg_url[300]	= "";

			if (sgOut == NULL)
			{
				sprintf (system_url,
						 "%s%s%s%s",
						 url_path,
						 subdir,
						 file_name,
						 ".html");

				sprintf (svg_url,
						 "%s%s%s%s",
						 url_path,
						 subdir,
						 file_name,
						 ".svg");
			}
			else
			{

				sprintf (system_url,
						 "/cgi-bin/StarGen.pl?Catalog=%s&Dole=%d&SolStation=%d&Mass=%LG&Output=all&Seed=%ld&Count=1&Incr=1&Gas=%s&Moon=%s&SVG=%s",
						 (cat_arg == NULL) ? "none" : cat_arg->arg,
						 sys_no,
						 sys_no,
						 sun.mass,
						 flag_seed,
						 (do_gases)					? "on" : "off",	// one of ("on", "off")
						 (do_moons)					? "on" : "off",	// one of ("on", "off")
						 (this->graphic_format == SVG)	? "on" : "off"	// one of ("on", "off")
						);

				sprintf (svg_url,
						 "/cgi-bin/StarGen.pl?Catalog=%s&Dole=%d&SolStation=%d&Mass=%LG&Output=all&Seed=%ld&Count=1&Incr=1&Gas=%s&Moon=%s&SVG=%s&DoIt=SVG",
						 (cat_arg == NULL) ? "none" : cat_arg->arg,
						 sys_no,
						 sys_no,
						 sun.mass,
						 flag_seed,
						 (do_gases)					? "on" : "off",	// one of ("on", "off")
						 (do_moons)					? "on" : "off",	// one of ("on", "off")
						 (this->graphic_format == SVG)	? "on" : "off"	// one of ("on", "off")
						);
			}

			switch (this->out_format)
			{
				case fSVG:
					create_svg_file (sgOut, innermost_planet, path, file_name, ".svg", prognam);
				break;

				case HTML:
					if ((this->graphic_format == SVG) && (sgOut == NULL))
					{
						create_svg_file (NULL, innermost_planet, path, file_name, ".svg", prognam);
					}

					if (thumbnails != NULL)
						html_thumbnails(innermost_planet, thumbnails,
										system_name,
										url_path, system_url, svg_url, file_name,
										false, true, false, do_moons, (this->graphic_format == SVG));

 					if ((system_count == 1) || (sgOut == NULL))
 					{
						if ((system_count == 1) && (sgOut != NULL))
							html_file = open_html_file (system_name, flag_seed, path, url_path, file_name, ".html",
														prognam, sgOut);
						else
							html_file = open_html_file (system_name, flag_seed, path, url_path, file_name, ".html",
														prognam, NULL);

						if (NULL != html_file)
						{
							html_thumbnails(innermost_planet, html_file,
											system_name,
											url_path, system_url, svg_url, file_name,
											true, false, true, do_moons, (this->graphic_format == SVG));
							html_describe_system(innermost_planet, do_gases, url_path, html_file);
							close_html_file(html_file);
						}
						else
						{
							fprintf(sgErr, "Could not open file %s%s%s\n",
									path, file_name, ".html");
							exit(0);
						}
					}
				break;

				case TEXT:
					text_describe_system(innermost_planet, do_gases, flag_seed);
				break;

				case CSV:
				case CSVdl:
					if (csv_file != NULL)
						csv_describe_system(csv_file, innermost_planet, do_gases, flag_seed);
				break;

				case CELESTIA:
					if (in_celestia != false)
					{
						if (has_known_planets && !use_known_planets)
							fprintf (sgErr, "Skipping %s -- Has planets in Celestia already\n",
									designation);
						else
							celestia_describe_system(innermost_planet, designation);
					}
				break;
			}
			if ((habitable > 1) &&
				StarGen::Stargen::isVerbose(0x0001))
				fprintf (sgErr, "System %ld - %s (-s%ld -%c%d) has %d planets with breathable atmospheres.\n",
						flag_seed,
						system_name,
						flag_seed,
						flag_char,
						sys_no,
						habitable);
		}

		if (! ((use_solar_system) && (index == 0)))
			flag_seed += seed_increment;

		if (reuse_solar_system)
			earth.mass += (EM(inc_mass));

		free_atmosphere (innermost_planet);

		// Free the dust and planets created by accrete:
		free_generations ();
	}

	if (StarGen::Stargen::isVerbose(0x0001) || StarGen::Stargen::isVerbose(0x0002))
	{
		fprintf (sgErr, "Earthlike planets: %d\n", total_earthlike);
		fprintf (sgErr, "Breathable atmospheres: %d\n", total_habitable);
		fprintf (sgErr, "Breathable g range: %4.2Lf -  %4.2Lf\n",
				 min_breathable_g,
				 max_breathable_g);
		fprintf (sgErr, "Terrestrial g range: %4.2Lf -  %4.2Lf\n",
				 min_breathable_terrestrial_g,
				 max_breathable_terrestrial_g);
		fprintf (sgErr, "Breathable pressure range: %4.2Lf -  %4.2Lf\n",
				 min_breathable_p,
				 max_breathable_p);
		fprintf (sgErr, "Breathable temp range: %+.1Lf C -  %+.1Lf C\n",
				 min_breathable_temp - EARTH_AVERAGE_KELVIN,
				 max_breathable_temp - EARTH_AVERAGE_KELVIN);
		fprintf (sgErr, "Breathable illumination range: %4.2Lf -  %4.2Lf\n",
				 min_breathable_l,
				 max_breathable_l);
		fprintf (sgErr, "Terrestrial illumination range: %4.2Lf -  %4.2Lf\n",
				 min_breathable_terrestrial_l,
				 max_breathable_terrestrial_l);
		fprintf (sgErr, "Max moon mass: %4.2Lf\n",
				 max_moon_mass * SUN_MASS_IN_EARTH_MASSES);
	}

	if (system_count > 1)
	{
		if (do_gases)
			html_thumbnail_totals(thumbnails);

		close_html_file(thumbnails);
	}
	if (csv_file != NULL)
	{
		fflush (csv_file);
		fclose (csv_file);
	}

	return(0);
}
} // End StarGen namespace
