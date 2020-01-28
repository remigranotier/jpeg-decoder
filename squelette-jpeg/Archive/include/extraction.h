#ifndef __EXTRACTION__
#define __EXTRACTION__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


#include "bitstream_perso.h"
#include "huffman_perso.h"
#include "jpeg_reader_perso.h"


extern int16_t **extraction_totale(struct bitstream *stream, struct jpeg_desc *jpeg);
// Récupère dans le bon ordre (YCbCr), l'intégralité des vecteurs du stream
// On récupère les valeurs des fréquences, comme elles étaient avant huffman et rle
// N'effectue pas d'upsampling

#endif
