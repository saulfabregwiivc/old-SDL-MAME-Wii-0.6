###########################################################################
#
#   tiny_deco32.mak
#
#   tiny MAME Wii build for 'Data East DECO32' driver makefile
#	Use 'make SUBTARGET=tiny_deco32' to build
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
CPUS += ARM
CPUS += H6280
CPUS += M6809
CPUS += MCS48



#-------------------------------------------------
# Specify all the sound cores necessary for the
# drivers referenced in tiny.c.
#-------------------------------------------------

SOUNDS += CUSTOM
SOUNDS += SAMPLES
SOUNDS += DAC
SOUNDS += DISCRETE
SOUNDS += YM2151
SOUNDS += OKIM6295
SOUNDS += BSMT2000



#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in tiny.c
#-------------------------------------------------

DRVLIBS = \
	$(MAMEOBJ)/tiny_deco32.o \
	$(MACHINE)/ticket.o \
	$(MACHINE)/decoprot.o \
	$(MACHINE)/decocrpt.o \
	$(MACHINE)/deco156.o \
	$(VIDEO)/deco16ic.o \
	$(DRIVERS)/deco32.o $(VIDEO)/deco32.o \



#-------------------------------------------------
# layout dependencies
#-------------------------------------------------

