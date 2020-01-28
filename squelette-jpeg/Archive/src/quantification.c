#include <stdint.h>

#include "../include/quantification.h"

void quantification_fct(int16_t tab_frequentiel[64], uint8_t mat_quantification[64])
{
	for (int16_t i = 0; i < 64; i++) {
		//Pour chaque valeur, multiplique par la valeur correspondante dans la table de quantification
		tab_frequentiel[i] *= mat_quantification[i];
	}
}
