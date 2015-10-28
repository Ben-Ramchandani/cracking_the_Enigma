#ifndef ENIGMA_HEADER
#define ENIGMA_HEADER

struct s_enigma {
	const unsigned char** rotors;
	const unsigned char * reflector;
	unsigned char fast_pos;
	unsigned char mid_pos;
	unsigned char slow_pos;
};

//From Enigma.c
unsigned char encipher(const unsigned char * rotors[6], const unsigned char *reflector, unsigned char fast,unsigned char mid, unsigned char slow, unsigned char n);

unsigned char rotor_encipher(unsigned char n, unsigned char position, const unsigned char *encode_array);//n is a number between 0 and 25, representing a number between A and Z.

void encipher_multiple(const unsigned char* input, int inputLen, unsigned char* result, struct s_enigma enig);


//From enigma_helper.c
void printCharArray(const unsigned char * arr, int length);

void invert_rotor(const unsigned char rotor[26], unsigned char rev_rotor[26]);

int rotor_permutations(int perm[6][3]);


//Rotor definitions
extern const unsigned char RotorI[26];
extern const unsigned char RotorII[26];
extern const unsigned char RotorIII[26];
extern const unsigned char ReflectorB[26];

#endif
