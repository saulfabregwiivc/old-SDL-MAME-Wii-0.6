/******************************************************************************

    tiny_namcos2.c

    mamedriv.c substitute file for tiny MAME build for Namco System 2.

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
#include "tiny_namcos2.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#define DRIVER(NAME) &GAME_NAME(NAME),
const game_driver * const drivers[] =
{
#include "tiny_namcos2.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* Namco System 2 games */
	DRIVER( finallap )	/* 87.12 Final Lap */
	DRIVER( finallapd )	/* 87.12 Final Lap */
	DRIVER( finallapc )	/* 87.12 Final Lap */
	DRIVER( finallapjc )/* 87.12 Final Lap */
	DRIVER( finallapjb )/* 87.12 Final Lap */
	DRIVER( assault )	/* (c) 1988 */
	DRIVER( assaultj )	/* (c) 1988 (Japan) */
	DRIVER( assaultp )	/* (c) 1988 (Japan) */
	DRIVER( metlhawk )	/* (c) 1988 */
	DRIVER( metlhawkj )	/* (c) 1988 */
	DRIVER( ordyne )	/* (c) 1988 */
	DRIVER( ordynej )	/* (c) 1988 */
	DRIVER( mirninja )	/* (c) 1988 (Japan) */
	DRIVER( phelios )	/* (c) 1988 (Japan) */
	DRIVER( dirtfoxj )	/* (c) 1989 (Japan) */
	DRIVER( fourtrax )	/* 89.11 */
	DRIVER( valkyrie )	/* (c) 1989 (Japan) */
	DRIVER( finehour )	/* (c) 1989 (Japan) */
	DRIVER( burnforc )	/* (c) 1989 (Japan) */
	DRIVER( burnforco )	/* (c) 1989 (Japan) */
	DRIVER( marvland )	/* (c) 1989 (US) */
	DRIVER( marvlandj )	/* (c) 1989 (Japan) */
	DRIVER( kyukaidk )	/* (c) 1990 (Japan) */
	DRIVER( kyukaidko )	/* (c) 1990 (Japan) */
	DRIVER( dsaber )	/* (c) 1990 */
	DRIVER( dsaberj )	/* (c) 1990 (Japan) */
	DRIVER( finalap2 )	/* 90.8  Final Lap 2 */
	DRIVER( finalap2j )	/* 90.8  Final Lap 2 (Japan) */
	DRIVER( gollygho )	/* 91.7  Golly Ghost */
	DRIVER( rthun2 )	/* (c) 1990 */
	DRIVER( rthun2j )	/* (c) 1990 (Japan) */
	DRIVER( sgunner )	/* (c) 1990 */
	DRIVER( sgunnerj )	/* (c) 1990 */
	DRIVER( sgunner2 )	/* (c) 1991 (US) */
	DRIVER( sgunner2j )	/* (c) 1991 (Japan) */
	DRIVER( cosmogng )	/* (c) 1991 (US) */
	DRIVER( cosmogngj )	/* (c) 1991 (Japan) */
	DRIVER( bubbletr )	/* (c) 1992 */
	DRIVER( finalap3 )	/* 92.9  Final Lap 3 */
	DRIVER( finalap3j )	/* 92.9  Final Lap 3 */
	DRIVER( luckywld )	/* (c) 1992 */
	DRIVER( luckywldj )	/* (c) 1992 */
	DRIVER( suzuka8h )	/* (c) 1992 (World) */
	DRIVER( suzuka8hj )	/* (c) 1992 (Japan) */
	DRIVER( sws )		/* (c) 1992 (Japan) */
	DRIVER( sws92 )		/* (c) 1992 (Japan) */
	DRIVER( sws92g )	/* (c) 1992 (Japan) */
	DRIVER( suzuk8h2 )	/* (c) 1993 (World) */
	DRIVER( sws93 )		/* (c) 1993 (Japan) */

#endif	/* DRIVER_RECURSIVE */
