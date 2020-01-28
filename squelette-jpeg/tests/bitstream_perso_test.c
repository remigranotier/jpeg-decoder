#include <stdio.h>
#include "../include/bitstream_perso.h"

int main() {
    struct bitstream *test = create_bitstream("images/invader.ppm");
    printf("INITIALISATION :\nPrev : %#x, Byte : %#x, Pos : %d\n\n", test->prev, test->byte, test->pos);
    uint32_t dest;
    uint8_t nb_bits = 32;
    uint8_t nb = read_bitstream(test, nb_bits, &dest, false);
    for (int i = 0; i < 20; i++) {
        nb = read_bitstream(test, nb_bits, &dest, false);
        printf("READ_BITSTREAM de %d bits :\nPrev : %#x, Byte : %#x, Pos : %d, NbBitsLus : %d, Dest : %#x\n", nb_bits, test->prev, test->byte, test->pos, nb, dest);
        printf("Fini ? %s \n\n", end_of_bitstream(test) ? "Oui" : "Non");
    }
    /* skip_bitstream_until(test, 0xee);
    printf("Après skip:\nPrev : %#x, Byte : %#x, Pos : %d\n\n", test->prev, test->byte, test->pos);
    printf("Fini ? %d \n", end_of_bitstream(test)); */
    close_bitstream(test);
}
