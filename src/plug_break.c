//The main block of code that runs through the rotor positions and finds possible positions.

#include <stdio.h>
#include "Enigma.h"
#include <unistd.h> //For sleep()

//#define DEBUG
//#define VERBOSE

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
		
		//TODO: Is this necessary?
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



