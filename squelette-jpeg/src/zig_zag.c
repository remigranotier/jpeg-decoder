#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/zig_zag.h"


int16_t **zig_zag_inverse(int16_t *vecteur_freq)
{
	int i = 0;
	int j = 0;
	//Valeur indiquant la trajectoire
	int i_decallage = 0;
	int j_decallage = 0;
	int element = 0;
	//Allocation mémoire
	int16_t **tableau = malloc(sizeof(*tableau)*8);
	for (int i = 0; i < 8; i++){
		tableau[i] = malloc(sizeof(int16_t)*8);
	}
	while (element < 64){
		i += i_decallage;
		j += j_decallage;
		tableau[j][i] = vecteur_freq[element];
		element ++;

		if (j == 0 || i == 7) {
			//Si on atteint une extrémité, on effectue un changement de trajectoire
			if (j==0) {
				i++;
			} else {
				j++;
			}
			i_decallage = -1;
			j_decallage = 1;
			tableau[j][i] = vecteur_freq[element];
			element ++;
		} else if (i == 0 || j == 7) {
			//Si on atteint une extrémité, on effectue un changement de trajectoire
			if (j==7) {
				i++;
			} else {
				j++;
			}
			i_decallage = 1;
			j_decallage = -1;
			tableau[j][i] = vecteur_freq[element];
			element ++;
		}
	}
	return tableau;
}
