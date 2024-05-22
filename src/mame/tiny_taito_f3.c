/******************************************************************************

    tiny_taito_f3.c

    mamedriv.c substitute file for tiny MAME build for Taito F3 System.

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
#include "tiny_taito_f3.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#define DRIVER(NAME) &GAME_NAME(NAME),
const game_driver * const drivers[] =
{
#include "tiny_taito_f3.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* Taito F3 games */
	DRIVER( ringrage )	/* 1992.?? D21 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( ringragej )	/* 1992.09 D21 (c) 1992 Taito Corporation (Japan) */
	DRIVER( ringrageu )	/* 1992.02 D21 (c) 1992 Taito America Corporation (US) */
	DRIVER( arabianm )	/* 1992.?? D29 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( arabianmj )	/* 1992.09 D29 (c) 1992 Taito Corporation (Japan) */
	DRIVER( arabianmu )	/* 1992.10 D29 (c) 1992 Taito America Corporation (US) */
	DRIVER( ridingf )	/* 1992.?? D34 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( ridingfj )	/* 1992.12 D34 (c) 1992 Taito Corporation (Japan) */
	DRIVER( ridingfu )	/* 1992.?? D34 (c) 1992 Taito America Corporation (US) */
	DRIVER( gseeker )	/* 1992.?? D40 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( gseekerj )	/* 1992.12 D40 (c) 1992 Taito Corporation (Japan) */
	DRIVER( gseekeru )	/* 1992.?? D40 (c) 1992 Taito America Corporation (US) */
	DRIVER( hthero93 )	/* 1993.03 D49 (c) 1992 Taito Corporation (Japan) */
						/* 1993.04 D49 (US) */
	DRIVER( cupfinal )	/* 1993.?? D49 (c) 1993 Taito Corporation Japan (World) */
	DRIVER( trstar )	/* 1993.?? D53 (c) 1993 Taito Corporation Japan (World) */
	DRIVER( trstarj )	/* 1993.07 D53 (c) 1993 Taito Corporation (Japan) */
	DRIVER( prmtmfgt )	/* 1993.08 D53 (c) 1993 Taito Corporation (US) */
	DRIVER( prmtmfgto )	/* 1993.08 D53 (c) 1993 Taito Corporation (US) */
	DRIVER( trstaro )	/* 1993.?? D53 (c) 1993 Taito Corporation (World) */
	DRIVER( trstaroj )	/* 1993.07 D53 (c) 1993 Taito Corporation (Japan) */
	DRIVER( gunlock )	/* 1994.?? D66 (c) 1993 Taito Corporation Japan (World) */
	DRIVER( rayforcej )	/* 1994.02 D66 (c) 1993 Taito Corporation (Japan) */
	DRIVER( rayforce )	/* 1994.?? D66 (c) 1993 Taito America Corporation (US) */
	DRIVER( scfinals )	/* 1994.?? D68 (c) 1993 Taito Corporation Japan (World) */
	DRIVER( intcup94 )	/* 1994.?? D78 (c) 1994 Taito (World) */
	DRIVER( hthero94 )	/* 1994.09 D78 (c) 1994 Taito (US) */
	DRIVER( lightbr )	/* 1994.03 D69 (c) 1993 Taito Corporation (Japan) */
	DRIVER( dungeonm )	/* 1994.?? D69 (c) 1993 Taito Corporation Japan (World) */
	DRIVER( dungeonmu )	/* 1994.?? D69 (c) 1993 Taito America Corporation (US) */
	DRIVER( kaiserkn )	/* 1994.?? D84 (c) 1994 Taito Corporation Japan (World) */
	DRIVER( kaiserknj )	/* 1994.08 D84 (c) 1994 Taito Corporation (Japan) */
	DRIVER( gblchmp )	/* 1994.10 D84 (c) 1994 Taito America Corporation (US) */
	DRIVER( dankuga )	/* 1994.?? D84? (c) 1994 Taito Corporation (Japan) */
	DRIVER( dariusg )	/* 1994.?? D87 (c) 1994 Taito Corporation Japan (World) */
	DRIVER( dariusgj )	/* 1994.09 D87 (c) 1994 Taito Corporation (Japan) */
	DRIVER( dariusgu )	/* 1994.11 D87 (c) 1994 Taito America Corporation (US) */
	DRIVER( dariusgx )	/* 1994.?? D87 (c) 1994 Taito Corporation */
	DRIVER( bublbob2 )	/* 1994.?? D90 (c) 1994 Taito Corporation Japan (World) */
	DRIVER( bubsymphe )	/* 1994.?? D90 (c) 1994 Taito Corporation Japan (Europe) */
	DRIVER( bubsymphu )	/* 1994.10 D90 (c) 1994 Taito America Corporation (US) */
	DRIVER( bubsymphj )	/* 1994.10 D90 (c) 1994 Taito Corporation (Japan) */
	DRIVER( bubsymphb )	/* bootleg */
	DRIVER( spcinvdj )	/* 1994.09 D93 (c) 1994 Taito Corporation (Japan) */
	DRIVER( pwrgoal )	/* 1994.?? D94 (c) 1995 Taito Corporation Japan (World) */
	DRIVER( hthero95 )	/* 1994.11 D94 (c) 1995 Taito Corporation (Japan) */
	DRIVER( hthero95u )	/* 1995.05 D94 (c) 1995 Taito America Corporation (US) */
	DRIVER( qtheater )	/* 1995.01 D95 (c) 1994 Taito Corporation (Japan) */
	DRIVER( elvactr )	/* 1995.?? E02 (c) 1994 Taito Corporation Japan (World) */
	DRIVER( elvactrj )	/* 1995.03 E02 (c) 1994 Taito Corporation (Japan) */
	DRIVER( elvact2u )	/* 1995.05 E02 (c) 1994 Taito America Corporation (US) */
	DRIVER( spcinv95 )	/* 1995.?? E06 (c) 1995 Taito Corporation Japan (World) */
	DRIVER( spcinv95u )	/* 1995.05 E06 (c) 1995 Taito America Corporation (US) */
	DRIVER( akkanvdr )	/* 1995.07 E06 (c) 1995 Taito Corporation (Japan) */
	DRIVER( twinqix )	/* 1995.03 ??? (c) 1995 Taito America Corporation (US) */
	DRIVER( quizhuhu )	/* 1995.07 E08 (c) 1995 Taito Corporation (Japan) */
	DRIVER( pbobble2 )	/* 1995.?? E10 (c) 1995 Taito Corporation Japan (World) */
	DRIVER( pbobble2o )	/* 1995.?? E10 (c) 1995 Taito Corporation Japan (World) */
	DRIVER( pbobble2j )	/* 1995.09 E10 (c) 1995 Taito Corporation (Japan) */
	DRIVER( pbobble2u )	/* 1995.11 E10 (c) 1995 Taito America Corporation (US) */
	DRIVER( pbobble2x )	/* 1995.12 E10 (c) 1995 Taito Corporation (Japan) */
	DRIVER( gekirido )	/* 1995.11 E11 (c) 1995 Taito Corporation (Japan) */
	DRIVER( tcobra2 )	/* 1996.?? E15 (c) 1995 Taito Corporation (World) */
	DRIVER( tcobra2u )	/* 1996.?? E15 (c) 1995 Taito Corporation (US) */
	DRIVER( ktiger2 )	/* 1996.02 E15 (c) 1995 Taito Corporation (Japan) */
	DRIVER( bubblem )	/* 1996.?? E21 (c) 1995 Taito Corporation Japan (World) */
	DRIVER( bubblemj )	/* 1996.04 E21 (c) 1995 Taito Corporation (Japan) */
	DRIVER( cleopatr )	/* 1996.10 E28 (c) 1996 Taito Corporation (Japan) */
	DRIVER( pbobble3 )	/* 1996.?? E29 (c) 1996 Taito Corporation (World) */
	DRIVER( pbobble3u )	/* 1996.11 E29 (c) 1996 Taito Corporation (US) */
	DRIVER( pbobble3j )	/* 1996.11 E29 (c) 1996 Taito Corporation (Japan) */
	DRIVER( arkretrn )	/* 1997.03 E36 (c) 1997 Taito Corporation (Japan) */
	DRIVER( kirameki )	/* 1997.09 E44 (c) 1997 Taito Corporation (Japan) */
	DRIVER( puchicar )	/* 1997.?? E46 (c) 1997 Taito Corporation (World) */
	DRIVER( puchicarj )	/* 1997.12 E46 (c) 1997 Taito Corporation (Japan) */
	DRIVER( pbobble4 )	/* 1998.?? E49 (c) 1997 Taito Corporation (World) */
	DRIVER( pbobble4j )	/* 1998.02 E49 (c) 1997 Taito Corporation (Japan) */
	DRIVER( pbobble4u )	/* 1998.?? E49 (c) 1997 Taito Corporation (US) */
	DRIVER( popnpop )	/* 1998.?? E51 (c) 1997 Taito Corporation (World) */
	DRIVER( popnpopj )	/* 1998.03 E51 (c) 1997 Taito Corporation (Japan) */
	DRIVER( popnpopu )	/* 1998.?? E51 (c) 1997 Taito Corporation (US) */
	DRIVER( landmakr )	/* 1998.08 E61 (c) 1998 Taito Corporation (Japan) */
	DRIVER( landmakrp )	/* 1998.?? E61 (c) 1998 Taito Corporation (World, prototype) */
	DRIVER( recalh )	/* prototype */
	DRIVER( commandw )	/* prototype */

#endif	/* DRIVER_RECURSIVE */
