#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/bitstream_perso.h"

struct bitstream *create_bitstream(const char *filename)
{
    struct bitstream *creation = malloc(sizeof(struct bitstream));
    if (creation) { // Si l'allocation mémoire s'est bien déroulée
        creation->fichier = fopen(filename, "r");
        if (creation->fichier == NULL) { // Si l'ouverture s'est bien déroulée
            return NULL;
        }
        // On place le bitstream au niveau du premier octet
        fread(&(creation->byte), 1, 1, creation->fichier);
        creation->pos = 7; // 7 correspond au début de l'octet
        creation->prev = 0; // L'octet précédent est choisi nul par défaut
        return creation;
    } else {
        return NULL;
    }
}

void close_bitstream(struct bitstream *stream)
{
    fclose(stream->fichier);
    free(stream);
}

uint8_t read_bitstream(struct bitstream *stream, uint8_t nb_bits,
                       uint32_t *dest, bool discard_byte_stuffing)
{
    *dest = 0; // On initialise la valeur lue à 0
    uint8_t current_offset, mask;
    uint8_t nb_parcourus = 0;
    // Tant qu'il faut toujours lire des bits et qu'on n'est pas à la fin du fichier
    while (nb_bits > 0 && !end_of_bitstream(stream)) {
        // En cas de discard byte stuffing
        if (discard_byte_stuffing && stream->prev == 255 && stream->byte == 0) {
            // On ignore le 00
            stream->prev = 0;
            fread(&(stream->byte), 1, 1, stream->fichier);
            continue; // On continue de lire après cela
        }
        // Si on continuera de lire après cet octet
        if (nb_bits > stream->pos) {
            current_offset = stream->pos + 1; // Nombre de bits lus sur cet octet
            mask = (1 << current_offset) - 1; // Masque associé à la lecture
            *dest <<= current_offset; // On décale vers la gauche ce qu'on a déja lu
            *dest += (stream->byte & mask); // On rajoute ce qu'on vient de lire
            // On se place au début du prochain octet
            stream->pos = 7;
            stream->prev = stream->byte;
            fread(&(stream->byte), 1, 1, stream->fichier);
        } else { // Si les derniers bits à lire sont dans cet octet
            current_offset = nb_bits;
            // On crée le masque associé à la lecture
            mask = ((1 << (stream->pos + 1)) - 1);
            mask -= ((1 << (stream->pos + 1 - current_offset)) - 1);
            *dest <<= current_offset; // On décale à gauche ce qu'on a déja lu
            // On rajoute ce qu'on vient de lire
            *dest += (stream->byte & mask) >> (stream->pos + 1 - current_offset);
            stream->pos -= current_offset; // OMàj de la position dans l'octet
        }
        nb_bits -= current_offset; // Màj du nombre de bits à lire
        nb_parcourus += current_offset; // Màj du nombre de bits lus
    }
    return nb_parcourus;
}

bool end_of_bitstream(struct bitstream *stream)
{
    return feof(stream->fichier) && stream->pos == 7;
}

void skip_bitstream_until(struct bitstream *stream, uint8_t byte)
{
    // Si on était en train de lire un octet au milieu
    if (stream->pos < 7) {
        fread(&stream->byte, 1, 1, stream->fichier);
    }
    // Si on a pas le bon octet, on avance d'un octet
    while (stream->byte != byte && !feof(stream->fichier)) {
        fread(&stream->byte, 1, 1, stream->fichier);
    }
    // Si on est bien sortis de la boucle après avoir lu le bon octet, on se
    // place au début de cet octet
    if (stream->byte == byte) {
        stream->pos = 7;
    }
    // En l'occurence, notre parser n'est pas censé skip jusqu'a la fin d'un
    // fichier, il correspond donc à une erreur
    if (feof(stream->fichier)) {
        perror("skip_bitstream_until : skip à la fin du fichier");
        exit(1);
    }
}
