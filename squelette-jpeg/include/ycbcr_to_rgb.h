#ifndef __YCBCR_TO_RGB_H__
#define __YCBCR_TO_RGB_H__
#include <stdint.h>

/* Transforme en place un tableau sous forme YCbCr en RGB */
extern void ycbcr_to_rgb(uint32_t largeur, uint32_t hauteur, uint8_t ***vecteur_de_blocs);

#endif
