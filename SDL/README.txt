To save memory, SDL MAME Wii uses a custom build of SDL-Wii. Simply replace
the folder SDL with the one here, replacing both files in it, and compile/
install it. SDL MAME Wii might be able to run with an unedited SDL, but a
chunk of memory will be used up, which can cause some larger games to fail to
load.

If downloading the SDL source and compiling it yourself isn't you thing, just
take libSDLnovid.a from this directory and stick in in your devkitPro
environment. You will still need the SDL header files installed to compile
SDL MAME Wii.
