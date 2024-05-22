/******************************************************************************

    tiny_deco32.c

    mamedriv.c substitute file for tiny MAME build for Data East DECO32.

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
#include "tiny_deco32.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#define DRIVER(NAME) &GAME_NAME(NAME),
const game_driver * const drivers[] =
{
#include "tiny_deco32.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* Data East DECO32 (32-bit) games */
	DRIVER( captaven )	/* MAN (c) 1991 Data East Corporation (Asia) */
	DRIVER( captavena )	/* MAN (c) 1991 Data East Corporation (Asia) */
	DRIVER( captavene )	/* MAN (c) 1991 Data East Corporation (UK) */
	DRIVER( captavenu )	/* MAN (c) 1991 Data East Corporation (US) */
	DRIVER( captavenuu )	/* MAN (c) 1991 Data East Corporation (US) */
	DRIVER( captavenua )	/* MAN (c) 1991 Data East Corporation (US) */
	DRIVER( captavenj )	/* MAN (c) 1991 Data East Corporation (Japan) */
	DRIVER( dragngun )	/* MAR (c) 1992 Data East Corporation (US) */
	DRIVER( fghthist )	/* MBF (c) 1993 Data East Corporation (World) */
	DRIVER( fghthistu )	/* MBF (c) 1993 Data East Corporation (US) */
	DRIVER( fghthista )	/* MBF (c) 1993 Data East Corporation (US) */
	DRIVER( fghthistj )	/* MBF (c) 1993 Data East Corporation (Japan) */
	DRIVER( lockload )	/* MBM (c) 1994 Data East Corporation (US) */
	DRIVER( lockloadu )	/* MBM (c) 1994 Data East Corporation (US) */
	DRIVER( nslasher )	/* MBH (c) 1993 Data East Corporation */
	DRIVER( nslasherj )	/* MBH (c) 1993 Data East Corporation (Japan)*/
	DRIVER( nslashers )	/* MBH (c) 1993 Data East Corporation (Japan)*/
	DRIVER( tattass )	/* prototype (c) 1994 Data East Pinball (US) */
	DRIVER( tattassa )	/* prototype (c) 1994 Data East Pinball (Asia) */

#endif	/* DRIVER_RECURSIVE */
