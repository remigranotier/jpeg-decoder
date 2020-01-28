#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/ycbcr_to_rgb.h"

void ycbcr_to_rgb(uint32_t largeur, uint32_t hauteur,
                  uint8_t ***vecteur_de_blocs)
{
    uint32_t ni = (hauteur >> 3) + (hauteur%8 > 0 ? 1 : 0); // Nb de lignes de blocs
    uint32_t nj = (largeur >> 3) + (largeur%8 > 0 ? 1 : 0); // Nb de colonnes de blocs
    for (uint32_t i = 0; i < ni*nj; i++) {
        // On itère sur les lignes qui sont bien dans l'image
        for (uint32_t k = 0; k < ((i>=(ni-1)*nj) ? ((hauteur%8==0) ? 8 : hauteur%8) : 8); k++) {
            // On itère sur les colonnes qui sont bien dans l'image
            for (uint32_t l = 0; l < ((i%nj == nj-1) ? ((largeur%8==0) ? 8 : largeur%8) : 8); l++) {
                // Transformation des composantes YCbCr en RGB
                float Y  = (float) vecteur_de_blocs[3*i][k][l];
                float Cb = (float) vecteur_de_blocs[3*i+1][k][l];
                float Cr = (float) vecteur_de_blocs[3*i+2][k][l];
                float r1 = Y - 0.0009267 * (Cb - 128) + 1.4016868 * (Cr - 128);
                float r2 = Y - 0.3436954 * (Cb - 128) - 0.7141690 * (Cr - 128);
                float r3 = Y + 1.7721604 * (Cb - 128) + 0.0009902 * (Cr - 128);
                // Saturation des résultats et changement des valeurs dans le tableau
                vecteur_de_blocs[3*i][k][l]   = r1 >= 0   ? (uint8_t) r1 : 0;
                vecteur_de_blocs[3*i][k][l]   = r1 <= 255 ? (uint8_t) vecteur_de_blocs[3*i][k][l] : 255;
                vecteur_de_blocs[3*i+1][k][l] = r2 >= 0   ? (uint8_t) r2 : 0;
                vecteur_de_blocs[3*i+1][k][l] = r2 <= 255 ? (uint8_t) vecteur_de_blocs[3*i+1][k][l] : 255;
                vecteur_de_blocs[3*i+2][k][l] = r3 >= 0   ? (uint8_t) r3 : 0;
                vecteur_de_blocs[3*i+2][k][l] = r3 <= 255 ? (uint8_t) vecteur_de_blocs[3*i+2][k][l] : 255;
            }
        }
    }
}
