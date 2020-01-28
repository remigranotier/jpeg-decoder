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
			printf("%d, ",  MCU1[6][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[7][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[8][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[9][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[10][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[11][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[12][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[13][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[14][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[15][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[16][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[17][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[18][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[19][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[20][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[21][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[22][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[23][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[24][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[25][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[26][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[27][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[28][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[29][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[30][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[31][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[32][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[33][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[34][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[35][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[36][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[37][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[38][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[39][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[40][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[41][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[42][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[43][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[44][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[45][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[46][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[47][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[48][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[49][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[50][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[51][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[52][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[53][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[54][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[55][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[56][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[57][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[58][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[59][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[60][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[61][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[62][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[63][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[64][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[65][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[66][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[67][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[68][j][k]);
        	}
        	printf("\n");
    	}
	printf("\n\n");
       	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t k = 0; k < 8; k++) {
			printf("%d, ",  MCU1[69][j][k]);
        	}
        	printf("\n");
    	}
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
        	for (uint8_t k = 0; k < 8; k++) {
                	printf("%d, ",  MCU1[70][j][k]);
        	}
        	printf("\n");
        }
        printf("\n\n");
        for (uint8_t j = 0; j < 8; j++) {
    	        for (uint8_t k = 0; k < 8; k++) {
            		printf("%d, ",  MCU1[71][j][k]);
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
