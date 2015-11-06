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

//This is just so I don't have to pass the rotors and reflector around the whole time - the positions aren't used.
static struct s_enigma enigma_s;



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


/* This is the core of the code that finds possible rotor positions.
 * The code takes a guess (initial[2]), then tries every rotor combitation in turn
 * and evaluating whether there is a plugboard setup that could give this encipherment.
 * If there is it prints it out, otherwise it carries on.
 */


void break_enigma(	unsigned char initial[2],
			const unsigned char * rotors[6],
			const unsigned char reflector[26],
			unsigned char plain_text[TEXT_LEN],
			unsigned char cipher_text[TEXT_LEN]) {
	
	//List of all the possible connections and whether they are there or not.
	char matrix[26][26];
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
			//It would be possible to get a rating on how likely a position is correct
			//by counting the number of 1s/25s that arise.
			printf("Potential position: %d, %d, %d\n", fast, mid, slow);
		}
		#ifdef DEBUG
		printf("Progress: %d\n", progress);
		sleep(3u);
		#endif
	}
	}
	}
}


//This program uses arrays of unsigned chars 0<=c<26, so convert.
void string_to_enigma_input(char *str, unsigned char *text) {
	int i;
	for(i=0; str[i] != '\0'; i++) {
		if(str[i] < 'A' || str[i] > 'Z') {
			fprintf(stderr, "Warn: string_to_enigma_input: Input string contains invalid characters - replaced with 'X'.");
			text[i] = 'A' - 'X'; // Replace bad characters with X
		} else {
			text[i] = str[i] - 'A';
		}
	}
	if(i != 13)//Length taken by cracker
		fprintf(stderr, "Warn: string_to_enigma_input: Input string was not of length 13.");
	return;
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
