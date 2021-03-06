#ifndef ENIGMA_HEADER
#define ENIGMA_HEADER

#define TEXT_LEN 13

//This model ignores the ring positions as we assume the inner rotors do not step
struct s_enigma {
	const unsigned char** rotors;
	const unsigned char * reflector;
	unsigned char fast_pos;
	unsigned char mid_pos;
	unsigned char slow_pos;
};

//From Enigma.c
unsigned char encipher(const unsigned char * rotors[6], const unsigned char *reflector, unsigned char fast,unsigned char mid, unsigned char slow, unsigned char n);

unsigned char rotor_encipher(unsigned char n, unsigned char position, const unsigned char *encode_array);//n is a number between 0 and 25, representing a character between A and Z.

void encipher_multiple(const unsigned char* input, int inputLen, unsigned char* result, struct s_enigma enig);


//From enigma_helper.c
void zero_char_array(char *arr, int length);

void zero_int_array(int* arr, int length);

void string_to_enigma_input(char *str, unsigned char *text);

void printCharArray(const unsigned char * arr, int length);

void invert_rotor(const unsigned char rotor[26], unsigned char rev_rotor[26]);

int rotor_permutations(int perm[6][3]);


//Rotor definitions, defined in enima_helper.c
extern const unsigned char RotorI[26];
extern const unsigned char RotorII[26];
extern const unsigned char RotorIII[26];
extern const unsigned char ReflectorB[26];

//From plug_break.c

void break_enigma(      unsigned char initial[2],
                        const unsigned char * rotors[6],
                        const unsigned char reflector[26],
                        unsigned char plain_text[TEXT_LEN],
                        unsigned char cipher_text[TEXT_LEN]);

#endif
