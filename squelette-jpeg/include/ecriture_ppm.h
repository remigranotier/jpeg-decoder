#ifndef __ECRITURE_PPM_H__
#define __ECRITURE_PPM_H__
#include <stdint.h>

enum magic_number {
    RGB,
    NB
};

extern void ecriture_ppm(enum magic_number type,
                         char *nom_fichier,
                         uint32_t largeur,
                         uint32_t hauteur,
                         uint8_t ***tab_pixels);
/*
Crée un fichier .ppm à partir d'un tableau <tab_pixels> de la forme :
[Y00, Cb00, Cr00, Y01, Cb01, Cr01, ...]
où chaque élément est un bloc 8x8 (de type uint8_t**)
La mémoire correspondante au tableau tab_pixels est libérée.

<type> représente le type d'image : RGB ou Noir et Blanc (NB)
<nom_fichier> est le nom du fichier .jpeg que l'on veut transformer en .ppm
<largeur> et <hauteur> représentent la taille en pixels de l'image_ppm
*/

#endif
