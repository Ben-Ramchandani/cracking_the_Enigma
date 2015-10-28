#include "Enigma.h"
#include <stdio.h>

const unsigned char RotorI[] = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9}; //'EKMFLGDQVZNTOWYHXUSPAIBRCJ'
const unsigned char RotorII[] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4}; //'AJDKSIRUXBLHWTMCQGZNPYFVOE'
const unsigned char RotorIII[] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14}; //'BDFHJLCPRTXVZNYEIWGAKMUSQO'
const unsigned char ReflectorB[] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19}; //'YRUHQSLDPXNGOKMIEBFZCWVJAT'



void printCharArray(const unsigned char * arr, int length) {
        int i;
        for(i=0; i<length; i++)
                printf("%c, ", arr[i]+65);
        printf("\n");
}

//Changes the second array to be the mirror image of the first.
void invert_rotor(const unsigned char rotor[26], unsigned char rev_rotor[26])
{
    unsigned char i;

    for(i=0; i<26; i++)
    {
        rev_rotor[ rotor[i] ] = i;
    }
    return;
}


int rotor_permutations(int perm[6][3])
{
    int i;
    int j;
    int k;
    int perm_count = 0;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(j != i)
            {
                for(k = 0; k < 3; k++)
                {
                    if(k != i && k != j)
                    {
                        perm[perm_count][0] = i;
                        perm[perm_count][1] = j;
                        perm[perm_count][2] = k;
                        perm_count++;
                        //printf("Permutation: %i, %i, %i\n", i, j, k);
                    }
                }
            }
        }
    }
    return(0);
}
