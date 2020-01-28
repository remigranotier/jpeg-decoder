#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/huffman_perso.h"
#include "../include/bitstream_perso.h"

static struct huff_table *find_slot(struct huff_table **huff, uint8_t profondeur)
{
    struct huff_table *resultat, *copie, *temp;
    if (*huff == NULL) { // Si on doit descendre mais qu'il n'y a encore rien
        *huff = malloc(sizeof(struct huff_table));
    	(*huff)->fils_1 = NULL;
    	(*huff)->present = false;
    	(*huff)->fils_0 = NULL;
    	(*huff)->val = 0;
        temp = *huff; // On garde le pointeur en mémoire pour ne pas le modifier en sortie
        for (uint8_t i = 0; i < profondeur; i++) {
            /* On se décale dans le fils gauche jusqu'a ce que la profondeur
            soit adéquate */
            (*huff)->fils_0 = malloc(sizeof(struct huff_table));
            (*huff) = (*huff)->fils_0;
	        (*huff)->fils_0 = NULL;
	        (*huff)->fils_1 = NULL;
	        (*huff)->present = false;
	        (*huff)->val = 0;
        }
        copie = (*huff); // On va renvoyer la copie de l'endroit "disponible"
        (*huff) = temp;
        return copie;
    } else if (profondeur == 0) {
        // Si on est à la bonne profondeur mais qu'il n'y a pas de place
        return NULL;
    } else if ((*huff)->present){
        // Si on a atteint une valeur, on ne descend pas plus dans l'arbre
        return NULL;
    }
    // On cherche récursivement à gauche
    resultat = find_slot(&((*huff)->fils_0), profondeur-1);
    if (resultat) {
        // Si on a trouvé à gauche, on renvoie le résultat
        return resultat;
    } else {
        // Sinon, on cherche récursivement à droite
        return find_slot(&((*huff)->fils_1), profondeur-1);
    }
}

struct huff_table *load_huffman_table(struct bitstream *stream,
                                      uint16_t *nb_byte_read)
{
    uint8_t profondeurs[16];
    uint32_t dest;
    struct huff_table *destination;
    struct huff_table *huff = NULL;
    *nb_byte_read = 0;
    // On remplit le tableau des profondeurs
    for (uint8_t i = 0; i < 16; i++) {
        *nb_byte_read += read_bitstream(stream, 8, &dest, false);
        profondeurs[i] = dest;
    }
    // On remplit la table huff en fonction de ce que l'on lit
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < profondeurs[i]; j++) {
            *nb_byte_read += read_bitstream(stream, 8, &dest, false);
            destination = find_slot(&huff, i+1);
            destination->val = dest%(1<<8); // Au cas où
            destination->present = true;
        }
    }
    return huff;
}

uint8_t next_huffman_value(struct huff_table *table, struct bitstream *stream)
{
    uint32_t dest;
    while (table && !(table->present)) { // vérifications, au cas où
        read_bitstream(stream, 1, &dest, true);
        // On descend à gauche où à droite en fonction de ec que l'on lit
        if (dest == 1) {
            table = table -> fils_1;
        } else {
            table = table -> fils_0;
        }
    }
    if (table == NULL) { // En cas d'erreur
        perror("La table est vide : y'a sûrement un problème ^^");
    }
    return table->val;
}

uint8_t next_huffman_value_count(struct huff_table *table,
                                 struct bitstream *stream,
                                 uint8_t *nb_bits_read)
{
    uint32_t dest;
    while (table && !(table->present)) {
        *nb_bits_read += read_bitstream(stream, 1, &dest,  true);
        if (dest) {
            table = table -> fils_1;
        } else {
            table = table -> fils_0;
        }
    }
    if (table == NULL) {
        perror("La table est vide : y'a surement un problème ^^");
    }
    return table->val;
}

void free_huffman_table(struct huff_table *table)
{
    if (table == NULL) {
        return;
    }
    if (table->fils_0) {
        free_huffman_table(table->fils_0);
    }
    if (table->fils_1) {
        free_huffman_table(table->fils_1);
    }
    free(table);
}
