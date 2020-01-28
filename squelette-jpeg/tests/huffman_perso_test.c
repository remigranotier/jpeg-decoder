#include <stdio.h>
#include <stdlib.h>
#include "../include/huffman_perso.h"
#include "../include/bitstream_perso.h"

static void print_huffman(struct huff_table *table, uint8_t profondeur) {
    if (table == NULL) {
        return;
    }
    if ((table->fils_0 == NULL) && (table->fils_1 == NULL)) {
        printf("%x, %u\n", table->val, profondeur);
    } else {
        print_huffman(table->fils_0, profondeur + 1);
        print_huffman(table->fils_1, profondeur + 1);
    }
}

int main() {
    uint16_t nb_bytes_read = 0;
    struct bitstream *test = create_bitstream("images/gris.jpg");
    skip_bitstream_until(test, 0x42);
    skip_bitstream_until(test, 0x00);
    struct huff_table *resultat = load_huffman_table(test, &nb_bytes_read);
    print_huffman(resultat, 0);
    //printf("%d\n", nb_bytes_read);


    struct bitstream *position = create_bitstream("tests/fichier");
    printf("%x\n", next_huffman_value(resultat, position));
    free_huffman_table(resultat);
    close_bitstream(test);
    close_bitstream(position);
}
