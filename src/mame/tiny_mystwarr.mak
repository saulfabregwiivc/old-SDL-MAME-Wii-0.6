###########################################################################
#
#   tiny_mystwarr.mak
#
#   tiny MAME Wii build for Konami 'Mystic Warriors' driver makefile
#	Use 'make SUBTARGET=tiny_mystwarr' to build
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
SOUNDS += K054539



#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in tiny.c
#-------------------------------------------------

DRVLIBS = \
	$(MAMEOBJ)/tiny_mystwarr.o \
	$(MACHINE)/ticket.o \
	$(MACHINE)/konamigx.o \
	$(VIDEO)/konamiic.o \
	$(DRIVERS)/mystwarr.o $(VIDEO)/mystwarr.o \



#-------------------------------------------------
# layout dependencies
#-------------------------------------------------
