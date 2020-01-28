#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/idct_opti.h"

int main() {
    int16_t **phi = malloc(8 * sizeof(*phi));
    for (int i = 0; i < 8; i++) {
        phi[i] = malloc(8 * sizeof(int16_t));
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            phi[i][j] = 10;
        }
    }
    float v[7];
    v[0] = sqrt(2);
    v[1] = cos(M_PI / 16);
    v[2] = sin(M_PI / 16);
    v[3] = cos(3 * M_PI / 16);
    v[4] = sin(3 * M_PI / 16);
    v[5] = cos(6 * M_PI / 16);
    v[6] = sin(6 * M_PI / 16);

    uint8_t **S = idct_opti(phi, v);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", S[i][j]);
        }
        printf("\n");
    }
    for (uint8_t i = 0; i < 8; i++) {
        free(S[i]);
    }
    free(S);
}
