#include <stdio.h>
#include "Enigma.h"
#include <time.h>

#define ENIGMA_TIME_RUN
//#define ACCEPT_ARGS



int main(int argc, char** argv) {
	unsigned char RevRotorI[26];
	unsigned char RevRotorII[26];
	unsigned char RevRotorIII[26];


	invert_rotor(RotorI, RevRotorI);
	invert_rotor(RotorII, RevRotorII);
	invert_rotor(RotorIII, RevRotorIII);
	const unsigned char * Rotors[6] = { RotorI, RotorII, RotorIII, RevRotorIII, RevRotorII, RevRotorI  };

	int i;
	
	//Set up a plaintext message (13 characters)
	unsigned char plain_text[13];
	string_to_enigma_input("ATTACKATTACKA", plain_text);
	unsigned char cipher_text[13];

	#if defined(ACCEPT_ARGS)
	if(argc >= 2)
		string_to_enigma_input(argv[1], plain_text);
	#endif	

	
	
	//Set up an enigma for encryption
	//As it stands the rotors are in 0, 1, 2 order.
	struct s_enigma enig;
	enig.rotors = Rotors;
	enig.reflector = ReflectorB;
	enig.fast_pos = 1;
	enig.mid_pos =  1;
	enig.slow_pos = 1;
	
	//And encrypt our example text
	//Note when we encrypt we assume only the fast rotor moves, as we do during decryption
	//to counter this a guessed message is split up into two adjacent parts of length <=13,
	//such that the middle rotor would turn in at most one of them (barring double step, which was rare).
	encipher_multiple(plain_text, 13, cipher_text, enig);

	#ifdef ENIGMA_TIME_RUN
	clock_t begin, end;
	double time_spent;
	begin = clock();
	#endif
	
	//Set up for decryption
	int perms[6][3];
	rotor_permutations(perms);
	unsigned char initial[2] = {0, 0};
	
	//Try each possible rotor order
	for(i = 0; i < 6; i++)
	{
		//It would be nice to have >3 rotors usable
		Rotors[ perms[i][0] ] = RotorI;
		Rotors[ 5-perms[i][0] ] = RevRotorI;
		Rotors[ perms[i][1] ] = RotorII;
		Rotors[ 5-perms[i][1] ] = RevRotorII;
		Rotors[ perms[i][2] ] = RotorIII;
		Rotors[ 5-perms[i][2] ] = RevRotorIII;
		printf("\nTrying rotor combination %d, %d, %d.\n", perms[i][0], perms[i][1], perms[i][2]);
		break_enigma(initial, Rotors, ReflectorB, plain_text, cipher_text);
	}
	#ifdef ENIGMA_TIME_RUN
	end = clock();
	time_spent = ((double) (end-begin)) / CLOCKS_PER_SEC;
	printf("Time for run: %Gs.\n", time_spent);
	#endif

	return 0;
}
