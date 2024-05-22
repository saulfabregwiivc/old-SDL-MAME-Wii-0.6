###########################################################################
#
#   tiny_namcos2.mak
#
#   tiny MAME Wii build for 'Namco System 2' driver makefile
#	Use 'make SUBTARGET=tiny_namcos2' to build
#
#   Copyright Nicola Salmoria and the MAME Team.
#   Visit  http://mamedev.org for licensing and usage restrictions.
#
###########################################################################

MAMESRC = $(SRC)/mame
MAMEOBJ = $(OBJ)/mame

AUDIO = $(MAMEOBJ)/audio
DRIVERS = $(MAMEOBJ)/drivers
LAYOUT = $(MAMEOBJ)/layout
MACHINE = $(MAMEOBJ)/machine
VIDEO = $(MAMEOBJ)/video

OBJDIRS += \
	$(AUDIO) \
	$(DRIVERS) \
	$(LAYOUT) \
	$(MACHINE) \
	$(VIDEO) \



#-------------------------------------------------
# Specify all the CPU cores necessary for the
# drivers referenced in tiny.c.
#-------------------------------------------------

CPUS += M680X0
CPUS += M6809
CPUS += M6805
CPUS += Z80
CPUS += MCS48



#-------------------------------------------------
# Specify all the sound cores necessary for the
# drivers referenced in tiny.c.
#-------------------------------------------------

SOUNDS += CUSTOM
SOUNDS += SAMPLES
SOUNDS += DAC
SOUNDS += DISCRETE
SOUNDS += NAMCO
SOUNDS += RF5C68
SOUNDS += C140
SOUNDS += YM2151



#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in tiny.c
#-------------------------------------------------

DRVLIBS = \
	$(MAMEOBJ)/tiny_namcos2.o \
	$(MACHINE)/ticket.o \
	$(DRIVERS)/namcoic.o \
	$(DRIVERS)/namcos2.o $(MACHINE)/namcos2.o $(VIDEO)/namcos2.o \
	$(DRIVERS)/namcos21.o $(VIDEO)/namcos21.o \
	$(AUDIO)/targ.o \



#-------------------------------------------------
# layout dependencies
#-------------------------------------------------

$(DRIVERS)/namcos2.o:	$(LAYOUT)/finallap.lh
