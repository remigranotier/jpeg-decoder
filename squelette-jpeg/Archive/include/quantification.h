#ifndef __QUANTIFICATION_H__
#define __QUANTIFICATION_H__
#include <stdint.h>

extern void quantification_fct(int16_t tab_frequentiel[64],
                               uint8_t mat_quantification[64]);
/*
Modifie la valeur de tab_frequentiel, en la mulitpliant par mat_quantification
(multiplication terme à terme)
*/


#endif
