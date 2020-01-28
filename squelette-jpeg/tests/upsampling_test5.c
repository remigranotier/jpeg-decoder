#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/upsampling.h"

int main() {
	uint8_t ***MCU1 = malloc(10*sizeof(**MCU1));
	for (uint8_t i = 0; i < 10; i++){						// MCU1 2*4; dégradé 
		MCU1[i] = malloc(8*sizeof(*MCU1));
		for (uint8_t j = 0; j < 8; j++){
			MCU1[i][j] = malloc(8*sizeof(uint8_t));
		}
	}
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			for (uint8_t k = 0; k < 8; k++){
				MCU1[k][i][j] = k;
			}
			MCU1[8][i][j]=i*8+j;
			MCU1[9][i][j]=i*8+j+64;
        	}
	}
    // AFFICHAGE
	MCU1 = upsampling(MCU1, 2, 4, 1, 1, 1, 1, 15, 30);
       	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++) {
			for (uint8_t k = 0; k < 8; k++) {
				printf("%d, ",  MCU1[3*i+1][j][k]);
        		}
       		 	printf("\n");
    		}
        	printf("\n\n");
        	for (uint8_t j = 0; j < 8; j++) {
        		for (uint8_t k = 0; k < 8; k++) {
                		printf("%d, ",  MCU1[3*i+2][j][k]);
        		}
        		printf("\n");
        	}
        	printf("\n\n");
	}
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
