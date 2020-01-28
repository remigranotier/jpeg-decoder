#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/upsampling.h"

int main() {
	uint8_t ***MCU1 = malloc(48*sizeof(**MCU1));           				// échantillonage horizontal en Cb: verticale en Cr
	for (uint8_t i = 0; i < 48; i++){						// Mcu 2*2; 2MCU par ligne: 3MCU par colonnes
		MCU1[i] = malloc(8*sizeof(*MCU1));
		for (uint8_t j = 0; j < 8; j++){
			MCU1[i][j] = malloc(8*sizeof(uint8_t));
		}
	}
	for (uint8_t i = 0; i < 2; i++){
		for (uint8_t j = 0; j < 3; j++){
			for (uint8_t k = 0; k < 8; k++){
				for (uint8_t l = 0; l < 8; l++){
					MCU1[0 + (i + 2 * j) * 8][k][l] = 0;
					MCU1[1 + (i + 2 * j) * 8][k][l] = 3;
					MCU1[2 + (i + 2 * j) * 8][k][l] = 12;
					MCU1[3 + (i + 2 * j) * 8][k][l] = 15;
					MCU1[4 + (i + 2 * j) * 8][k][l] = 1 + 3 * (l / 4);
					MCU1[5 + (i + 2 * j) * 8][k][l] = 13 + 3 * (l / 4);
					MCU1[6 + (i + 2 * j) * 8][k][l] = 2 + 12 * (k / 4);
					MCU1[7 + (i + 2 * j) * 8][k][l] = 5 + 12 * (k / 4);
					for (uint8_t m = 0; m < 8; m++){
						MCU1[m + (i + 2 * j) * 8][k][l] += (j * 24 + i * 6);
					}
				}
			}
        	}
	}
    // AFFICHAGE
	MCU1 = upsampling(MCU1, 2, 2, 1, 2, 2, 1, 30, 40);
       	for (uint8_t i = 0; i < 72; i++) {
		printf("%d, ", MCU1[i][0][0]);
    	}
        printf("\n\n");
       
           // LIBERATION MEMOIRE
	/*
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 8; j++) {
	    
            free(MCUs[0][i][j]);
        }
        free(MCUs[0][i]);
    }
    free(MCUs[0]);
    free(MCUs);*/
}
