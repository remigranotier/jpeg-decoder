#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ecriture_ppm.h"

void ecriture_ppm(enum magic_number type, char *nom_fichier,
                  uint32_t largeur, uint32_t hauteur, uint8_t ***tab_pixels)
{

    // NOM DE L'IMAGE
    int i = 0;
    while ((i == 0 || nom_fichier[i] != '.') && nom_fichier[i] != '\0') {
        i++;
    }
    char nom_partiel[i+5];
    strncpy(nom_partiel, nom_fichier, i);
    nom_partiel[i] = '\0'; // Pour que strcat fonctionne
    strcat(nom_partiel, ".ppm");

    // OUVERTURE DU FICHIER ET REMPLISSAGE DE L'ENTETE
    FILE *image_ppm = fopen(nom_partiel, "w");
    fprintf(image_ppm, "%s\n", type == RGB ? "P6" : "P5");
    fprintf(image_ppm, "%d %d\n", largeur, hauteur);
    fprintf(image_ppm, "255\n");

    // REMPLISSAGE DU .PPM
    uint32_t ni = (hauteur >> 3) + (hauteur%8 > 0 ? 1 : 0); // Nb de lignes de blocs
    uint32_t nj = (largeur >> 3) + (largeur%8 > 0 ? 1 : 0); // Nb de colonnes de blocs
    for (uint32_t i = 0; i < ni; i++) {
        for (uint32_t k = 0; k < ((i == (ni-1)) ? ((hauteur%8 == 0) ? 8 : hauteur%8) : 8); k++) {
            for (uint32_t j = 0; j < nj; j++) {
                for (uint32_t l = 0; l < ((j == (nj-1)) ? ((largeur%8 == 0) ? 8 : largeur%8) : 8); l++) {
                    if (type == RGB) {
                        fprintf(image_ppm, "%c", tab_pixels[3*(i*nj+j)][k][l]);
                        fprintf(image_ppm, "%c", tab_pixels[3*(i*nj+j)+1][k][l]);
                        fprintf(image_ppm, "%c", tab_pixels[3*(i*nj+j)+2][k][l]);
                    } else if (type == NB) {
                        fprintf(image_ppm, "%c", tab_pixels[i*nj+j][k][l]);
		            }
                }
            }
        }
    // LIBERATION DE LA MEMOIRE
        if (type == NB) {
            for (uint32_t j = 0; j < nj; j++) {
                for (uint32_t l = 0; l < 8; l++) {
                    free(tab_pixels[i*nj+j][l]);
                }
                free(tab_pixels[i*nj+j]);
            }
        } else if (type == RGB){
            for (uint32_t j = 0; j < nj; j++) {
                for (uint32_t l = 0; l < 8; l++) {
                    free(tab_pixels[3*(i*nj+j)][l]);
                    free(tab_pixels[3*(i*nj+j)+1][l]);
                    free(tab_pixels[3*(i*nj+j)+2][l]);
                }
                free(tab_pixels[3*(i*nj+j)]);
                free(tab_pixels[3*(i*nj+j)+1]);
                free(tab_pixels[3*(i*nj+j)+2]);
            }
        }
    }
    free(tab_pixels);
    fclose(image_ppm);
}
