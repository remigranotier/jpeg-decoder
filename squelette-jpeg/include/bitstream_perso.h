#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct bitstream {
    uint8_t prev; // previous byte value (for byte stuffing)
    uint8_t byte; // current byte
    uint8_t pos; // stream pointer position in byte : next bit to be read (76543210)
    FILE *fichier;
};

extern struct bitstream *create_bitstream(const char *filename);
/*
Alloue la mémoire et initialise un bitstream et ses attributs au début du
fichier <filename>, qui a été ouvert en mode lecture.
Si l'allocation mémoire ou l'ouverture du fichier s'est mal passée, la
fonction renvoie NULL.
*/

extern void close_bitstream(struct bitstream *stream);
/*
Ferme le fichier ouvert par le stream, et libère la mémoire associée au stream
*/

extern uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing);
/*
Place les <nb_bits> prochains bits du stream <stream> dans *dest.
Renvoie le nombre de bits parcourus dans la fonction.
<discard_byte_stuffying> permet de choisir si l'on veut ignorer les octets 00
placès juste après un octet FF.
*/

extern bool end_of_bitstream(struct bitstream *stream);
/*
Renvoie un booléen valant true si l'on est à la fin du bitstream <stream>
*/

extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);
/*
Fait se placer le bitstream au niveau du prochain octet <byte> non encore lu.
Si l'octet <byte> ne se trouve pas dans la suite du stream, on fait se placer
le bitstream à la fin du fichier.
*/

#endif
