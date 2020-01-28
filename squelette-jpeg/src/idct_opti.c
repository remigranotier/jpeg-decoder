#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/idct_opti.h"

static void transposee(float **tableau)
{
    float temp1, temp2;
    for (uint8_t i = 1; i < 8; i++) {
        for (uint8_t j = 0; j < i; j++) {
            temp1 = tableau[i][j];
            temp2 = tableau[j][i];
            tableau[i][j] = temp2;
            tableau[j][i] = temp1;
        }
    }
}

static void idct_reduit(float *col, float v[8])
{
    // Renommage des variables pour la lisibilité
    float sqrt2, cos1, sin1, cos3, sin3, cos6, sin6, sqrt8;
    sqrt2 = v[0];
    cos1 = v[1];
    sin1 = v[2];
    cos3 = v[3];
    sin3 = v[4];
    cos6 = v[5];
    sin6 = v[6];
    sqrt8 = v[7];
    for (uint8_t i = 0; i < 8; i++) {
         col[i] *= sqrt8;
    }
    float e_3_4[8];
    float e_2_3[8];
    float e_1_2[8];
    float jacky;

    // ETAPE 4 -> 3
    e_3_4[0] = col[0];
    e_3_4[1] = col[4];
    e_3_4[2] = col[2];
    e_3_4[3] = col[6];
    e_3_4[4] = (col[1] - col[7]) / 2;
    e_3_4[5] = col[3] / sqrt2;
    e_3_4[6] = col[5] / sqrt2;
    e_3_4[7] = (col[1] + col[7]) / 2;

    // ETAPE 3 -> 2
    e_2_3[0] = (e_3_4[0] + e_3_4[1]) / 2;
    e_2_3[1] = (e_3_4[0] - e_3_4[1]) / 2;
    jacky = cos6 * (e_3_4[2] + e_3_4[3]); // Optimisation des opérations
    e_2_3[2] = (jacky - e_3_4[3] * (cos6 + sin6)) / sqrt2;
    e_2_3[3] = (jacky + e_3_4[2] * (sin6 - cos6)) / sqrt2;
    e_2_3[4] = (e_3_4[4] + e_3_4[6]) / 2;
    e_2_3[5] = (e_3_4[7] - e_3_4[5]) / 2;
    e_2_3[6] = (e_3_4[4] - e_3_4[6]) / 2;
    e_2_3[7] = (e_3_4[7] + e_3_4[5]) / 2;

    // ETAPE 2 -> 1
    e_1_2[0] = (e_2_3[0] + e_2_3[3]) / 2;
    e_1_2[1] = (e_2_3[1] + e_2_3[2]) / 2;
    e_1_2[2] = (e_2_3[1] - e_2_3[2]) / 2;
    e_1_2[3] = (e_2_3[0] - e_2_3[3]) / 2;
    jacky = cos3 * (e_2_3[4] + e_2_3[7]);
    e_1_2[4] = jacky - e_2_3[7] * (cos3 + sin3);
    e_1_2[7] = jacky + e_2_3[4] * (sin3 - cos3);
    jacky = cos1 * (e_2_3[5] + e_2_3[6]);
    e_1_2[5] = jacky - e_2_3[6] * (cos1 + sin1);
    e_1_2[6] = jacky + e_2_3[5] * (sin1 - cos1);

    // IT'S THE FINAL COUNTDOWN
    col[0] = (e_1_2[0] + e_1_2[7]) / 2;
    col[7] = (e_1_2[0] - e_1_2[7]) / 2;
    col[1] = (e_1_2[1] + e_1_2[6]) / 2;
    col[6] = (e_1_2[1] - e_1_2[6]) / 2;
    col[2] = (e_1_2[2] + e_1_2[5]) / 2;
    col[5] = (e_1_2[2] - e_1_2[5]) / 2;
    col[3] = (e_1_2[3] + e_1_2[4]) / 2;
    col[4] = (e_1_2[3] - e_1_2[4]) / 2;
}

uint8_t **idct_opti(int16_t **phi, float v[7])
{
    float **travail = malloc(8 * sizeof(*travail));
    for (uint8_t i = 0; i < 8; i++) {
        travail[i] = malloc(8 * sizeof(float));
        for (uint8_t j = 0; j < 8; j++) {
            travail[i][j] = (float) phi[i][j]; // TRANSPOSEE
        }
    }
    uint8_t **resultat = malloc(8 * sizeof(*resultat));
    for (uint8_t i = 0; i < 8; i++) {
        resultat[i] = malloc(8 * sizeof(uint8_t));
    }

    for (uint8_t i = 0; i < 8; i++) {
        idct_reduit(travail[i], v);
    }

    transposee(travail);

    for (uint8_t i = 0; i < 8; i++) {
        idct_reduit(travail[i], v);
    }

    transposee(travail);

    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            travail[i][j] += 128;
            travail[i][j] = travail[i][j] < 0 ? 0 : travail[i][j];
            travail[i][j] = travail[i][j] > 255 ? 255 : travail[i][j];
            resultat[i][j] = (uint8_t) round(travail[i][j]);
        }
    }
    for (uint8_t i = 0; i < 8; i++) {
        free(phi[i]);
    }
    free(phi);
    for (uint8_t i = 0; i < 8; i++) {
        free(travail[i]);
    }
    free(travail);
    return resultat;
}
