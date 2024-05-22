###########################################################################
#
#   tiny_segas32.mak
#
#   tiny MAME Wii build for 'SEGA System 32 / SEGA Multi System 32' driver makefile
#	Use 'make SUBTARGET=tiny_segas32' to build
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

CPUS += Z80
CPUS += V60
CPUS += NEC
CPUS += MCS48



#-------------------------------------------------
# Specify all the sound cores necessary for the
# drivers referenced in tiny.c.
#-------------------------------------------------

SOUNDS += CUSTOM
SOUNDS += SAMPLES
SOUNDS += DAC
SOUNDS += DISCRETE
SOUNDS += YM2612
SOUNDS += RF5C68
SOUNDS += MULTIPCM



#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in tiny.c
#-------------------------------------------------

DRVLIBS = \
	$(MAMEOBJ)/tiny_segas32.o \
	$(MACHINE)/ticket.o \
	$(DRIVERS)/segas32.o $(MACHINE)/segas32.o $(VIDEO)/segas32.o \



#-------------------------------------------------
# layout dependencies
#-------------------------------------------------

$(DRIVERS)/segas32.o:	$(LAYOUT)/radr.lh
