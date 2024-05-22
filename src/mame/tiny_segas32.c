/******************************************************************************

    tiny_segas32.c

    mamedriv.c substitute file for tiny MAME build for SEGA System 32.

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

    The list of used drivers. Drivers have to be included here to be recognized
    by the executable.

    To save some typing, we use a hack here. This file is recursively #included
    twice, with different definitions of the DRIVER() macro. The first one
    declares external references to the drivers; the second one builds an array
    storing all the drivers.

******************************************************************************/

#include "driver.h"

#ifndef DRIVER_RECURSIVE

#define DRIVER_RECURSIVE

/* step 1: declare all external references */
#define DRIVER(NAME) GAME_EXTERN(NAME);
#include "tiny_segas32.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#define DRIVER(NAME) &GAME_NAME(NAME),
const game_driver * const drivers[] =
{
#include "tiny_segas32.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* Sega System 32 games */
	DRIVER( radm )		/* 1991.?? Rad Mobile (World) */
	DRIVER( radmu )		/* 1991.03 Rad Mobile (US) */
						/* 1991.02 Rad Mobile (Japan) */
	DRIVER( radr )		/* 1991.?? Rad Rally (World) */
	DRIVER( radru )		/* 1991.09 Rad Rally (US) */
						/* 1991.07 Rad Rally (Japan) */
	DRIVER( spidman )	/* 1991.?? Spiderman (World) */
	DRIVER( spidmanu )	/* 1991.09 Spiderman (US) */
						/* 1991.09 Spiderman (Japan) */
	DRIVER( f1en )		/* 1991.?? F-1 Exhaust Note (World) */
						/* 1992.01 F-1 Exhaust Note (US) */
						/* 1991.11 F-1 Exhaust Note (Japan) */
	DRIVER( arabfgt )	/* 1992.?? Arabian Fight (World) */
	DRIVER( arabfgtu )	/* 1992.03 Arabian Fight (US) */
	DRIVER( arabfgtj )	/* 1992.03 Arabian Fight (Japan) */
	DRIVER( arescue )	/* 1992.03 Air Rescur (US) */
						/* 1992.04 Air Rescue (Japan) */
	DRIVER( ga2 )		/* 1992.?? Golden Axe II (World) */
	DRIVER( ga2u )		/* 1992.09 Golden Axe II (US) */
	DRIVER( ga2j )		/* 1992.10 Golden Axe Death Adder no Fukusyuu (Japan) */
	DRIVER( holo )		/* 1992.?? Hologram Holosseum (US) */
						/* 1992.11 Hologram Holosseum (Japan) */
	DRIVER( darkedge )	/* 1993.?? Dark Edge (World) */
						/* 1993.?? Dark Edge (US) */
	DRIVER( darkedgej )	/* 1993.03 Dark Edge (Japan) */
	DRIVER( brival )	/* 1993.?? Burning Rival (World) */
	DRIVER( brivalj )	/* 1993.08 Burning Rival (Japan) */
	DRIVER( f1lap )		/* 1993.?? F-1 Super Lap (World) */
						/* 1993.09 F-1 Super Lap (Japan) */
	DRIVER( alien3 )	/* 1993.?? Aliens 3 (World) */
	DRIVER( alien3u )	/* 1993.11 Aliens 3 (US) */
						/* 1993.09 Aliens 3 the Gun (Japan) */
	DRIVER( sonic )		/* 1993.09 Sonic the Hedgehog (Japan) */
	DRIVER( sonicp )	/* 1993.?? Sonic the Hedgehog (proto, Japan) */
	DRIVER( kokoroj2 )	/* 1993.12 Soreike! Kokoroji 2 (Japan) */
	DRIVER( jpark )		/* 1994.?? Jurassic Park (World)  */
						/* 1994.?? Jurassic Park (US) */
						/* 1994.02 Jurassic Park (Japan) */
	DRIVER( dbzvrvs )	/* 1994.03 Dragon Ball Z V.R.V.S. (Japan) */
	DRIVER( jleague )	/* 1994.07 The J League 1994 (Japan) */
	DRIVER( svf )		/* 1994.?? Super Visual Football */
	DRIVER( svs )		/* 1994.?? Super Visual Soccer */
	DRIVER( slipstrm )	/* 1995.?? Slip Stream (Capcom) */
	DRIVER( slipstrmh )

	/* Sega Multi System 32 games */
						/* 1993.05 Outrunners (Japan) */
	DRIVER( orunners )	/* 1993.?? Outrunners (World) */
	DRIVER( orunnersu )	/* 1993.06 Outrunners (US) */
	DRIVER( harddunkj )	/* 1994.04 Hard Dunk 3on3 (Japan) */
	DRIVER( harddunk )	/* 1994.?? Hard Dunk 3on3 (World) */
						/* 1993.03 Title Fight (Japan) */
	DRIVER( titlef )	/* 1993.?? Title Fight (World) */
	DRIVER( titlefu )	/* 1993.04 Title Fight (US) */
						/* 1992.07 Stadium Cross (Japan) */
	DRIVER( scross )	/* 1992.?? Stadium Cross (World) */
	DRIVER( scrossu )	/* 1992.09 Stadium Cross (US) */

#endif	/* DRIVER_RECURSIVE */
