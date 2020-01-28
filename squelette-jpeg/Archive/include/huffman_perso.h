#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream_perso.h"


struct huff_table {
    uint8_t val;
    struct huff_table *fils_0;
    struct huff_table *fils_1;
    bool present;
};

extern struct huff_table *load_huffman_table(struct bitstream *stream,
                                             uint16_t *nb_byte_read);
/*
Renvoie une table de huffman créée à partir du stream <stream> placé au bon
endroit.
*/

extern uint8_t next_huffman_value(struct huff_table *table,
                                 struct bitstream *stream);
/*
Lit bit par bit dans le bitstream <stream> jusqu'à qu'une valeur soit trouvée
dans l'arbre. (Lire un 0 = descendre à gauche et inversement)
*/

extern uint8_t next_huffman_value_count(struct huff_table *table,
                                 struct bitstream *stream,
                                 uint8_t *nb_bits_read);
/*
Lit bit par bit dans le bitstream <stream> jusqu'à qu'une valeur soit trouvée
dans l'arbre. (Lire un 0 = descendre à gauche et inversement).
De plus, modifie *nb_bits_read
*/

extern void free_huffman_table(struct huff_table *table);
/*
Libère la mémoire associé à la huff_table <table>
*/

#endif
