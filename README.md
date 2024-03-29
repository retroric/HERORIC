# HERORIC - H.E.R.O. game port project for Oric Atmos

This is a small demo of a very early work in progress, based on my early  (and very naive) experimentations with the OSDK in 2015 at animating simple sprites in C on an Oric Atmos.

The goal of the project is to develop a clone of the original Atari VCS 2600 H.E.R.O. game for the Oric, since this game has been ported to virtually all mainstream micros of the era (C64, Amstrad CPC, and even the ZX Spectrum!!) at some time.

Presently, this program only displays an animated sprite with colour attributes. The sprite can be moved around the screen using arrows, and program execution can be stopped by pressing the SPACE key.

Actually, using colour attributes for a real implementation of the game would entail many problems, so an alternative would be to use AIC mode (Alternate Inverting Colors) as in recent Oric productions.

Note that this draft project has been at a standstill since its inception in 2015. I may be going forward with development at a later point in time, but I currently have other projects to cater.


## Building and Running

To build and execute this program, you need to install the Oric Software Developement Kit, available  from http://osdk.defence-force.org/.

The recommended stable version of the OSDK at this time is  v1.13 (v1.14 and v1.15 have issues in -O3 optimization mode).

To compile and execute the program in the Oricutron emulator bundled with the OSDK, two batch files are provided: "build.bat", and "execute.bat".

There are also some test programs in BASIC in the "BASIC" folder. To compile, use the OSDK's  "txt2bas" and "bas2tap" utilities to compile the .txt source files to .bas format then generate the tape files in .tap format. 


## Oric / OSDK Compatibility

This demo currently only targets the Oric Atmos.

It was first developed with OSDK 1.5, and successfully tested with OSDK v1.11 and v1.13

I currently use this program to do some regression testing work for new versions of the OSDK . As it stands, while it compiles and runs fine with OSDK 1.13 both in -O2 and -O3 optimization modes, it crashes with OSDK 1.15 when compiled in -O3 mode (crash to Oricutron monitor right after program start).

(R)etroric
