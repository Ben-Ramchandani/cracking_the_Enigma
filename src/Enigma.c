// Functions to mimic a 3 rotor Enigma with no plugboard.
// Assumes only the outer (fast) rotor moves.

#include <stdio.h>
#include <stdlib.h>
#include "Enigma.h"

unsigned char rotor_encipher(unsigned char n, unsigned char position, const unsigned char *encode_array)//n is a number between 0 and 25, representing a number between A and Z.
{
    n = (n + position) % 26;
    n = encode_array[n];
    //% doesn't work here because of wraparound, -1 becomes 255 etc.
    n = n - position;
    if(n > 25)
    {
        n += 26;
    }
    return n;
}

//3 rotors and 3 mirror rotors for reverse run through.
//position: [fast rotor, middle, slow], all inner rotor positions.
unsigned char encipher(const unsigned char * rotors[6], const unsigned char *reflector, unsigned char fast,unsigned char mid, unsigned char slow, unsigned char n)
{
    n = rotor_encipher(n, fast, rotors[0]);
    n = rotor_encipher(n, mid, rotors[1]);
    n = rotor_encipher(n, slow, rotors[2]);
    n = reflector[n];
    n = rotor_encipher(n, slow, rotors[3]);
    n = rotor_encipher(n, mid, rotors[4]);
    n = rotor_encipher(n, fast, rotors[5]);
    return n;
}

//Assume inner rotors don't move
void encipher_multiple(const unsigned char* input, int inputLen, unsigned char* result, struct s_enigma enig) {
	int i;
	int f_pos = enig.fast_pos;
	for(i=0; i<inputLen; i++) {
		result[i] = encipher(enig.rotors, enig.reflector, f_pos, enig.mid_pos, enig.slow_pos, input[i]);
		f_pos = (f_pos + 1)%26;
	}
}
