#pragma once

typedef struct dust_record	*dust_pointer;
typedef struct planets_record  *planet_pointer;
typedef struct gen *gen_pointer;

typedef enum planet_type {
	tUnknown,
	tRock,
	tVenusian,
	tTerrestrial,
	tGasGiant,
	tMartian,
	tWater,
	tIce,
	tSubGasGiant,
	tSubSubGasGiant,
	tAsteroids,
	t1Face
} planet_type;

typedef struct gas {
  	int         num;
	long double	surf_pressure;		/* units of millibars (mb)			 */
	} gas;

typedef struct Sun {
	long double	luminosity;
	long double	mass;
	long double life;
	long double age;
	long double r_ecosphere;
	const char	*name;
	} Sun;

typedef struct planets_record {
	int			planet_no;
	long double a;					/* semi-major axis of solar orbit (in AU)*/
	long double e;					/* eccentricity of solar orbit		 */
	long double	axial_tilt;			/* units of degrees					 */
	long double mass;				/* mass (in solar masses)			 */
	bool		gas_giant;			/* TRUE if the planet is a gas giant */
	long double	dust_mass;			/* mass, ignoring gas				 */
	long double	gas_mass;			/* mass, ignoring dust				 */
									/*   ZEROES start here               */
	long double moon_a;				/* semi-major axis of lunar orbit (in AU)*/
	long double moon_e;				/* eccentricity of lunar orbit		 */
	long double	core_radius;		/* radius of the rocky core (in km)	 */
	long double radius;				/* equatorial radius (in km)		 */
	int 		orbit_zone;			/* the 'zone' of the planet			 */
	long double density;			/* density (in g/cc)				 */
	long double orb_period;			/* length of the local year (days)	 */
	long double day;				/* length of the local day (hours)	 */
	bool		resonant_period;	/* TRUE if in resonant rotation		 */
	long double	esc_velocity;		/* units of cm/sec					 */
	long double	surf_accel;			/* units of cm/sec2					 */
	long double	surf_grav;			/* units of Earth gravities			 */
	long double	rms_velocity;		/* units of cm/sec					 */
	long double molec_weight;		/* smallest molecular weight retained*/
	long double	volatile_gas_inventory;
	long double	surf_pressure;		/* units of millibars (mb)			 */
	bool	 	greenhouse_effect;	/* runaway greenhouse effect?		 */
	long double	boil_point;			/* the boiling point of water (Kelvin)*/
	long double	albedo;				/* albedo of the planet				 */
	long double	exospheric_temp;	/* units of degrees Kelvin			 */
	long double estimated_temp;     /* quick non-iterative estimate (K)  */
	long double estimated_terr_temp;/* for terrestrial moons and the like*/
	long double	surf_temp;			/* surface temperature in Kelvin	 */
	long double	greenhs_rise;		/* Temperature rise due to greenhouse */
	long double high_temp;			/* Day-time temperature              */
	long double low_temp;			/* Night-time temperature			 */
	long double max_temp;			/* Summer/Day						 */
	long double min_temp;			/* Winter/Night						 */
	long double	hydrosphere;		/* fraction of surface covered		 */
	long double	cloud_cover;		/* fraction of surface covered		 */
	long double	ice_cover;			/* fraction of surface covered		 */
	Sun*		sun;
	int			gases;				/* Count of gases in the atmosphere: */
	gas*		atmosphere;
	planet_type type;				/* Type code						 */
									/*   ZEROES end here                 */
	bool		minor_moons;
	planet_pointer first_moon;
	planet_pointer next_planet;
	} planets;

/*	Define the solar system for comparisons, etc. */
#define ZEROES 0,0,0,0,0,0,0,0,false,0,0,0,0,0,0,0,false,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NULL,0,NULL,tUnknown

typedef struct dust_record {
	long double inner_edge;
	long double outer_edge;
	bool		dust_present;
	bool		gas_present;
	dust_pointer next_band;
	 } dust;

typedef struct star {
	long double		luminosity;
	long double		mass;
	long double		m2;
	long double		e;
	long double		a;
	planet_pointer	known_planets;
	const char		*desig;
	bool			in_celestia;
	const char		*name;
	} star;

typedef struct catalog {
	int				count;
	const char*		arg;
	star			*stars;
	} catalog;

typedef	struct gen
{
	dust_pointer	dusts;
	planet_pointer	planets;
	gen_pointer		next;
} generation;

// From Keris

typedef struct ChemTableS
{
  int         	num;
  const char	*symbol;
  const char	*html_symbol;
  const char	*name;
  long double	weight;
  long double	melt;
  long double	boil;
  long double	density;
  long double	abunde;
  long double	abunds;
  long double	reactivity;
  long double	max_ipp;	// Max inspired partial pressure im millibars
} ChemTable;

