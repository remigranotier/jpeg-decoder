#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__
#include <stdint.h>


extern uint8_t ***upsampling(uint8_t ***MCUs, uint8_t Liste_des_hi_vi[6], uint32_t pixels_par_lignes, uint32_t pixels_par_colonnes);
/*déséchantillone la liste de MCU et retourne une nouvelle liste déséchantilloné
arg: liste_des_mcu, liste_de_hi_vi, pixels par lignes, par colonnes */


#endif
