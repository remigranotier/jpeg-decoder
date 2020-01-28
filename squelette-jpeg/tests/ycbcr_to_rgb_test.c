#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/ycbcr_to_rgb.h"

int main() {
    uint32_t largeur = 10;
    uint32_t hauteur = 10;
    uint32_t ni = (hauteur >> 3) + (hauteur%8 > 0 ? 1 : 0);
    uint32_t nj = (largeur >> 3) + (largeur%8 > 0 ? 1 : 0);
    // ALLOCATION
    uint8_t ***tab_test = malloc(3 * ni * nj * sizeof(**tab_test));
    for (uint32_t i = 0; i < 3*ni*nj; i++) {
        tab_test[i] = malloc(8 * sizeof(*tab_test));
        for (uint32_t j = 0; j < 8; j++) {
            tab_test[i][j] = malloc(8 * sizeof(uint8_t));
        }
    }
    // REMPLISSAGE
    for (uint32_t i = 0; i < ni*nj; i ++) {
        for (uint32_t j = 0; j < 8; j++) {
            for (uint32_t k = 0; k < 8; k++) {
                tab_test[3*i][j][k] = 0;
                tab_test[3*i+1][j][k] = 0;
                tab_test[3*i+2][j][k] = 0;
            }
        }
    }
    ycbcr_to_rgb(largeur, hauteur, tab_test);
    // AFFICHAGE
    for (uint32_t i = 0; i < ni*nj; i++) {
        for (uint32_t j = 0; j < 8; j++) {
            for (uint32_t k = 0; k < 8; k++) {
                printf("%d, %d, %d |", tab_test[3*i][j][k], tab_test[3*i+1][j][k], tab_test[3*i+2][j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    // LIBERATION MEMOIRE
    for (uint32_t i = 0; i < 3*ni*nj; i++) {
        for (uint32_t j = 0; j < 8; j++) {
            free(tab_test[i][j]);
        }
        free(tab_test[i]);
    }
    free(tab_test);
}
