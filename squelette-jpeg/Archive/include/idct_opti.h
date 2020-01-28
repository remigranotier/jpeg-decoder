#ifndef __IDCT_OPTI_H__
#define __IDCT_OPTI_H__

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

extern uint8_t **idct_opti(int16_t **phi, float v[7]);
//Effectue l'idct du tableau phi, renvoie un tableau résultat
//Libère la mémoire associée à phi
//v est un tableau de constante, évitant les calculs répétitifs
#endif
