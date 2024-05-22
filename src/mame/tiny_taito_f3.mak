###########################################################################
#
#   tiny_taito_f3.mak
#
#   tiny MAME Wii build for 'Taito F3 System' driver makefile
#	Use 'make SUBTARGET=tiny_taito_f3' to build
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
SOUNDS += ES5506
SOUNDS += OKIM6295



#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in tiny.c
#-------------------------------------------------

DRVLIBS = \
	$(MAMEOBJ)/tiny_taito_f3.o \
	$(MACHINE)/ticket.o \
	$(DRIVERS)/taito_f3.o $(VIDEO)/taito_f3.o $(AUDIO)/taito_en.o \



#-------------------------------------------------
# layout dependencies
#-------------------------------------------------
