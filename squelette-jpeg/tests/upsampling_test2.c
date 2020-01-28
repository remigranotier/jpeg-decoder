#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/upsampling.h"

int main() {
	uint8_t ***MCU1 = malloc(4*sizeof(**MCU1));
	uint8_t ***MCU2 = malloc(4*sizeof(**MCU2));          				// échantillonage horizontal de MCU1: verticale de MCU2
	for (uint8_t i = 0; i < 4; i++){						// MCU1 2*1; MCU2 1*2 
		MCU1[i] = malloc(8*sizeof(*MCU1));
		MCU2[i] = malloc(8*sizeof(*MCU2));
		for (uint8_t j = 0; j < 8; j++){
			MCU1[i][j] = malloc(8*sizeof(uint8_t));
			MCU2[i][j] = malloc(8*sizeof(uint8_t));
		}
	}
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			MCU1[0][i][j]=0;
			MCU1[1][i][j]=3;
			MCU1[2][i][j]=1 + 3 * (j / 4);
			MCU1[3][i][j]=2 + 3 * (j / 4);

			MCU2[0][i][j]=0;
			MCU2[1][i][j]=3;
			MCU2[2][i][j]=1 + 3 * (i/4);
			MCU2[3][i][j]=2 + 3 * (i/4);
        	}
	}
    // AFFICHAGE
	MCU1 = upsampling(MCU1, 2, 1, 1, 1, 1, 1, 8, 8);
	MCU2 = upsampling(MCU2, 1, 2, 1, 1, 1, 1, 8, 8);
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[0][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[1][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[2][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[3][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[4][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[5][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU2[0][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU2[1][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU2[2][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU2[3][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU2[4][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU2[5][j][k]);
        	}
        	printf("\n");
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
