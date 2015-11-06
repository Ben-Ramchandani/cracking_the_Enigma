#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "Enigma.h"


//This is a much simpler proposition than in plug_break.c, we don't have a plugboard at all.
//It's what plug_break is based on, and I've just kept it in case anyone is interested.
//Is not mentioned in the Makefile.
bool crack_commercial(const unsigned char * rotors[6], const unsigned char reflector[26], const unsigned char plain_text[13], const unsigned char cipher_text[13], unsigned char answer[3])
{
    unsigned char n;
    unsigned char i;
    unsigned char j;
    unsigned char k;
    unsigned char c;
    bool bad_match;


    for(k = 0; k < 26; k++)
    {
        for(j = 0; j < 26; j++)
        {
            for(i = 0; i < 26; i++)
            {
            	bad_match = false;
                for(c = 0; c < 13; c++)
                {
                    n = encipher(rotors, reflector, (i + c)%26, j, k, plain_text[c]);
                    if( n != cipher_text[c] )
                    {
                        bad_match = true;
                        break;
                    }
                }

                if(!bad_match)
                {
                    printf("Match at: %d, %d, %d.\n", i, j, k);
                    answer [0] = i;
                    answer [1] = j;
                    answer [2] = k;
                    return( true );
                }
            }
        }
    }
    return(false);
}



int main()
{

    unsigned char RevRotorI[26];
    unsigned char RevRotorII[26];
    unsigned char RevRotorIII[26];


    invert_rotor(RotorI, RevRotorI);
    invert_rotor(RotorII, RevRotorII);
    invert_rotor(RotorIII, RevRotorIII);
    const unsigned char * Rotors[6] = { RotorI, RotorII, RotorIII, RevRotorIII, RevRotorII, RevRotorI  };

    int i;

    const unsigned char plain_text[13] = { 0 };
    unsigned char cipher_text[13];

    unsigned char answer[3];

    int perms[6][3];
    rotor_permutations(perms);
    struct s_enigma enig;
    enig.rotors = Rotors;
    enig.reflector = ReflectorB;
    enig.fast_pos = 3;
    enig.mid_pos = 1;
    enig.slow_pos = 25;
    
    encipher_multiple(plain_text, 13, cipher_text, enig);
    //printCharArray(cipher_text, 13);
    
#ifdef CE_TIME_RUN
    struct timeval stop, start;
    gettimeofday(&start, NULL);
#endif

    for(i = 0; i < 6; i++)
    {
        //This could be better, but there is the problem of separate variables for the Rotors.
        //Need to add support for >3 rotors.
        Rotors[ perms[i][0] ] = RotorI;
        Rotors[ 5-perms[i][0] ] = RevRotorI;
        Rotors[ perms[i][1] ] = RotorII;
        Rotors[ 5-perms[i][1] ] = RevRotorII;
        Rotors[ perms[i][2] ] = RotorIII;
        Rotors[ 5-perms[i][2] ] = RevRotorIII;
        printf("Trying rotor combination %d, %d, %d.\n", perms[i][0], perms[i][1], perms[i][2]);
        crack_commercial(Rotors, ReflectorB, plain_text, cipher_text, answer);


    }
#ifdef CE_TIME_RUN
    gettimeofday(&stop, NULL);
    printf("Run took %lu miliseconds\n", (stop.tv_usec - start.tv_usec)/1000);
#endif


    return 0;
}
