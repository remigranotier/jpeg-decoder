#include <stdlib.h>
#include <stdio.h>
#include "../include/ecriture_ppm.h"

int main(){
    uint32_t largeur = 10;
    uint32_t hauteur = 10;
    uint32_t ni = (hauteur >> 3) + (hauteur%8 > 0 ? 1 : 0);
    uint32_t nj = (largeur >> 3) + (largeur%8 > 0 ? 1 : 0);
    uint8_t ***tab_test = malloc(3*ni*nj*sizeof(**tab_test));
    for (uint32_t i = 0; i< 3*nj*ni; i++) {
        tab_test[i] = malloc(8*sizeof(*tab_test));
        for (uint32_t j = 0; j< 8; j++) {
            tab_test[i][j] = malloc(8*sizeof(uint8_t));
        }
    }
    for (uint32_t i = 0; i < nj*ni; i++) {
        for (uint32_t k = 0; k < ((i>=(ni-1)*nj) ? ((hauteur%8==0) ? 8 : hauteur%8) : 8); k++) {
            for (uint32_t l = 0; l < ((i%nj == nj-1) ? ((largeur%8==0) ? 8 : largeur%8) : 8); l++) {
                tab_test[3*i][k][l] = 250;
                tab_test[3*i+1][k][l] = 250;
                tab_test[3*i+2][k][l] = 250;
            }
        }
    }
    for (uint32_t i = 0; i < nj*ni; i++) {
        for (uint32_t k = 0; k < 8; k++) {
            for (uint32_t l = 0; l < 8; l++) {
                printf("%u ", tab_test[i][k][l]);
            }
            printf("\n");
        }
        printf("\n");
    }
    ecriture_ppm(RGB, "images/test", largeur, hauteur, tab_test);

}
