Enigma cracker
==============

A C program that attempts to find the wheel settings for the Military Enigma by a similar method to the Bombe machine.


The Enigma machine
------------------

The Enigma cipher machine was widely adopted by the German Military during World War II.
There were many different versions, but the basic Military Enigma consisted of three removable rotors, each with 26 connectors on each side, one reflector and a plugboard mounted on a typewriter like device.

More information can be found on the [Wikipedia page](https://en.wikipedia.org/wiki/Enigma_machine).


The Bombe
---------

The Bombe was an electro-mechanical device used by the allies to break the Enigma cipher. The basic idea behind it is to start with some cipher-text and a 'crib' (guessed plain-text) and test every combination of rotor positions to see if could plausibly have produced this output.
This program acts in a similar manner to the Bombe in its attempt to find possible rotor positions.


The code
--------

The interesting part is is in `src/plug\_break.c` - it contains the code that actually loops through the possibilities, it's called from `main.c`.  
`src/crack\_commercial.c` contains code to find the rotor positions for a machine without a plugboard (a Commercial Enigma) and is left here, but not used.


Installation and use
--------------------

Navigate to the `/src` folder and `make`.
The program can then be run as `./enigma`.  
The text, rotors and position used can be adjusted in `plug_break.c`.
