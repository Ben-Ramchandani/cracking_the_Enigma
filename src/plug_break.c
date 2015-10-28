#include <stdio.h>
#include "Enigma.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define TEXT_LEN 13
#define ENIGMA_TIME_RUN
//#define DEBUG
//#define VERBOSE
//#define ACCEPT_ARGS

static struct s_enigma enigma_s;

void zero_char_array(char* arr, int length)
{
    int i;

    for(i = 0; i < length; i++)
        arr[i] = 0;

    return;
}

void zero_int_array(int* arr, int length) {
    int i;

    for(i = 0; i < length; i++)
        arr[i] = 0;

    return;
}


char get_tri_matrix(char matrix[26][26], int i, int j) {
	return matrix[i][j];
}

void set_tri_matrix(char matrix[26][26], int i, int j) {
	matrix[i][j] = 1;
	matrix[j][i] = 1;
}

char get_encipherment(int fast, int mid, int slow, char c) {
	return encipher(enigma_s.rotors, enigma_s.reflector, fast, mid, slow, c);
}



void break_enigma(	unsigned char initial[2],
			const unsigned char * rotors[6],
			const unsigned char reflector[26],
			unsigned char plain_text[TEXT_LEN],
			unsigned char cipher_text[TEXT_LEN]) {
	
	//List of all the possible connections and whether they are there or not.
	char matrix[26][26] = { { 0 } };
	//If any of these reach 26 we have a full contradiction and can stop.
	int count[26] = { 0 };
	
	//Have we made progress this run through?
	int  progress = 2;//We need to check both plain and cipher text, decrement on no progress
	char full_contradiction ;


	int i, j, enj;
	unsigned char c, d;
	int fast, mid, slow;

	enigma_s.rotors = rotors;
	enigma_s.reflector = reflector;

	unsigned char* text = plain_text;
	unsigned char* other_text = cipher_text;
	unsigned char* tmp_text;
	#if defined(DEBUG) || defined(VERBOSE)
	printf("Plaintext: ");
	printCharArray(plain_text, TEXT_LEN);
	printf("Ciphertext: ");
	printCharArray(cipher_text, TEXT_LEN);
	printf("Initial: ");
	printCharArray(initial, 2);
	sleep(2u);
	#endif


		
	for(slow=0; slow<26; slow++) {
	for(mid=0;  mid <26; mid ++) {
	for(fast=0; fast<26; fast++) {
		zero_char_array((char*) matrix, 26*26);
		zero_int_array(count, 26);
		#ifdef DEBUG
		printf("Trying position: %d, %d, %d.\n", fast, mid, slow);
		#endif
		
		progress = 2;
		full_contradiction = 0;
		set_tri_matrix(matrix, initial[0], initial[1]);
		count[initial[0]] = 1;
		if(initial[0] == initial[1]) {count[initial[1]] = 1;}
		while(progress && !full_contradiction) {
			progress--;
			for(i=0; i<TEXT_LEN; i++) {
				c = text[i];
				d = other_text[i];
				for(j=0; j<26; j++) {
					if(get_tri_matrix(matrix, c, j)) {
						enj = get_encipherment((fast+i)%26, mid, slow, j);
						if(!get_tri_matrix(matrix, enj, d)) {
							set_tri_matrix(matrix, enj, d);
							progress = 2;
							count[enj] += 1;
							if(enj != d) {count[d] += 1;}
							#ifdef DEBUG
							printf("Encipherment %c -> %c -> %c -> %c gives ", c+65, j+65, enj+65, d+65);
							printf("new link: %c, %c\n", enj+65, d+65);
							printf("Counters for %c, %c are: %d, %d.\n\n", enj+65, d+65, count[enj], count[d]);
							#endif
						}
					}
				}
			}
			for(i=0; i<26; i++) {
				if(count[i] == 26) {
					#ifdef DEBUG
					printf("Full contradiction on %c.\n", i+65);
					#endif
					full_contradiction = 1;
				}
			}
			tmp_text   = other_text;
			other_text = text;
			text 	   = tmp_text;
		}
		if(!full_contradiction) {
			printf("Potential position: %d, %d, %d\n", fast, mid, slow);
			//Optional extra check
			j = 1;
			for(i=0; i<26; i++)
				if(!(count[i] == 25 || count[i] == 1))
					j = 0;
			if(j==1) {printf("Position near certain");}
		}
		#ifdef DEBUG
		printf("Progress: %d\n", progress);
		sleep(3u);
		//if(fast == 1) {exit(0);}
		#endif
	}
	}
	}
}

int main(int argc, char** argv) {
	unsigned char RevRotorI[26];
	unsigned char RevRotorII[26];
	unsigned char RevRotorIII[26];


	invert_rotor(RotorI, RevRotorI);
	invert_rotor(RotorII, RevRotorII);
	invert_rotor(RotorIII, RevRotorIII);
	const unsigned char * Rotors[6] = { RotorI, RotorII, RotorIII, RevRotorIII, RevRotorII, RevRotorI  };

	int i;

	unsigned char plain_text[13];
	unsigned char cipher_text[13];

	#if defined(ACCEPT_ARGS)
	if(argc >= 2) {
		for(i=0; i<13; i++) {
			if(argv[1][i] < 'A' || argv[1][i] > 'Z') {
				printf("Warn: First argument contains an invalid character at %d: %c, replaced with 'X'.\n", i, argv[1][i]);
				plain_text[i] = 'X'-'A';
			} else if(argv[1][i] == '\0') {
				printf("Fatal: first argument must be of length 13 or more.");
				exit(2);
			} else {
				plain_text[i] = argv[1][i] - 'A';
			}
		}
	} else {
		for(i=0; i<13; i++) {
			plain_text[i] = 0;
		}
	}
	#else
	for(i=0; i<13; i++) {
		plain_text[i] = i%3;
	}
	#endif	


	int perms[6][3];
	rotor_permutations(perms);
	struct s_enigma enig;
	enig.rotors = Rotors;
	enig.reflector = ReflectorB;
	enig.fast_pos = 1;//try 19 25 7
	enig.mid_pos =  1;
	enig.slow_pos = 1;
	
	unsigned char initial[2] = {0, 0};
	encipher_multiple(plain_text, 13, cipher_text, enig);
	#ifdef ENIGMA_TIME_RUN
	clock_t begin, end;
	double time_spent;
	
	begin = clock();
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
