#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bitstream_perso.h"
#include "../include/extraction.h"
#include "../include/huffman_perso.h"
#include "../include/jpeg_reader_perso.h"


static int16_t magnitude(uint8_t m, int16_t decallage, struct bitstream *stream)
{
	//Pour m sa magnitude, retourne la valeur du coefficient DC suivant dans le stream
	// Decalle de la valeur de "decallage"
	uint32_t byte = 0;
	int8_t nb = 0;
	uint32_t *ptr = &byte;
	nb = read_bitstream(stream, m, ptr, true);
	int16_t final_byte = (int16_t) byte;
	if ( final_byte <  (1 << (m - 1))) {
		//Valeur négative
		final_byte = -(1 << m) + final_byte + 1;
	}

	return (final_byte + decallage);
}

static int16_t *un_seul(uint8_t m, struct bitstream *stream, struct huff_table *huffman_ac, int16_t decallage_dc)
{
	//Récupère l'intégralité des valeurs d'un vecteur 64, en appliquant "ihuffman", "RLE" et "magnitude"
	//liste contiendra les coefficients, que j'initialise donc
	int16_t *liste;
	liste = malloc(sizeof(int16_t)*64);
	//Ici, on récupère le coefficient DC
	liste[0] = magnitude(m, decallage_dc, stream);
	int8_t element;
	uint8_t poids_faible;
	uint8_t poids_fort;
	int nb_element = 1;
	while (nb_element < 64) {
		//Recuperation d'une valeur
		element = next_huffman_value(huffman_ac, stream);
		if (element == 0x0000){break;} //Balise indiquant que la suite n'est que constitué de 0
		//Avec le RLE, poids_fort represente le nombre de 0 avant notre coefficient
		//Poids faible la magnitude du prochain coefficient non nul
		poids_fort = 0x0F &(element >> 4);
		poids_faible = (element & 0x0F);
		for (uint8_t i = 0; i < poids_fort; i++) {
			//On ajoute "poids fort" nombre de 0
			liste[nb_element++] = 0;
		}
		//On ajoute à notre liste le coefficient non nul de magnitude poids faible
		liste[nb_element++] = magnitude(poids_faible, 0, stream);
	}
	for (uint8_t i = nb_element; i < 64; i++) {
		//On ajoute les 0 restant en fin de vecteur
		liste[nb_element++] = 0;
	}
	return (liste);
}

static int recuperation_huffman(struct jpeg_desc *jpeg, int N_sof)
{
	//On récupère la valeur de la composante N_sos, dans la section SOS, tel que
	//Le ic qu'on récupère avec get_frame_component depuis la section SOF lié à la composante N_sof
	//Doit être le même ic qu'on récupère dans SOS avec la valeur sos.
	uint8_t Ic_sof = get_frame_component_id(jpeg, N_sof);
	int N_sos = -1;
	while(true) {
		uint8_t Ic_sos = get_scan_component_id(jpeg, ++N_sos);
		if (Ic_sos == Ic_sof){break;}
	}
	return(N_sos);
}

static uint8_t *trie(uint8_t *liste)
{
	///Rend les indices de la liste dans l'ordre croissant des valeurs
	uint8_t *liste_trie = malloc(sizeof(uint8_t)*3);
	uint8_t min    = liste[0];
	uint8_t milieu = liste[1];
	uint8_t max    = liste[2];

	if ((min < milieu) & (min < max)) {
		liste_trie[0] = 0;
		if (milieu < max) {
			liste_trie[1] = 1;
			liste_trie[2] = 2;
		} else {
			liste_trie[1] = 2;
			liste_trie[2] = 1;
		}
	} else if ((milieu < min) & (milieu < max)) {
		liste_trie[0] = 1;
		if (min < max) {
			liste_trie[1] = 0;
			liste_trie[2] = 2;
		} else {
			liste_trie[1] = 2;
			liste_trie[2] = 0;
		}
	} else {
		liste_trie[0] = 2;
		if (min < milieu) {
			liste_trie[1] = 0;
			liste_trie[2] = 1;
		} else {
			liste_trie[1] = 1;
			liste_trie[2] = 0;
		}
	}
	return (liste_trie);
}

int16_t **extraction_totale(struct bitstream *stream, struct jpeg_desc *jpeg)
{
	uint8_t nb_composant = get_nb_components(jpeg);
	uint16_t horizontale = get_image_size(jpeg, DIR_H);
	uint16_t verticale   = get_image_size(jpeg, DIR_V);
	//On va stocker ici la liste des vecteurs de 64 élements
	int16_t **joseph;

	if (nb_composant == 1){
		//Si on est en noir et blanc
		// Mcu de taille 1x1
		uint32_t total_mcu = ((horizontale >> 3) + (horizontale%8 > 0 ? 1 : 0)) 
		  	     	   * ((verticale   >> 3) + (verticale  %8 > 0 ? 1 : 0));
		joseph = malloc(sizeof(int16_t*) * total_mcu);

		//Recuperation des tables de huffman
		int sos_NB = recuperation_huffman(jpeg, 0);

		uint8_t index_huff_DC = get_scan_component_huffman_index(jpeg, DC, sos_NB);
		struct huff_table *huff_NB_DC = get_huffman_table(jpeg, DC, index_huff_DC);

		uint8_t index_huff_AC = get_scan_component_huffman_index(jpeg, AC, 0);
		struct huff_table *huff_NB_AC = get_huffman_table(jpeg, AC, index_huff_AC);

		int precedent = 0;
		for (uint32_t compteur = 0; compteur < total_mcu; compteur++) {
			//Récupération et ihuffman sur l'amplitude du DC
			int8_t m = next_huffman_value(huff_NB_DC, stream);
			//Puis on recupère les 64 valeurs constituant un vecteur avec la fonction un_seul
			int16_t *element = un_seul(m, stream, huff_NB_AC, precedent);
			joseph[compteur] = element;
			precedent = element[0];
		}
	}
	else if (nb_composant == 3) {
	//Si couleur
		struct huff_table **huff_global = malloc(sizeof(struct huff_table*)*6);

		//Récupération des tables de Huffman:
		int id;
		uint8_t index_huff;

		for (uint8_t i = 0; i < 3; i++) {
			id = recuperation_huffman(jpeg, i);
			//Composante DC
			index_huff = get_scan_component_huffman_index(jpeg, DC, id);
			huff_global[2*i] = get_huffman_table(jpeg, DC, index_huff);
			//Composante AC
			index_huff = get_scan_component_huffman_index(jpeg, AC, id);
			huff_global[2*i + 1] = get_huffman_table(jpeg, AC, index_huff);
		}
		//On récupère les facteurs d'échantillonage
		//Et les valeurs ic lié au composante
		uint8_t samp_glob[3];
		uint8_t samp[6];
		uint8_t ic_glob[3];
		for (uint8_t i = 0; i < 3; i++) {
			samp[2*i] = get_frame_component_sampling_factor(jpeg, DIR_H, i);
			samp[2*i + 1] =get_frame_component_sampling_factor(jpeg, DIR_V, i);
			samp_glob[i] = samp[2*i] * samp[2*i + 1];
			ic_glob[i] = get_frame_component_id(jpeg, i);
		}
		uint8_t somme_samp = samp_glob[0] + samp_glob[1] + samp_glob[2];
		//Les informations arriveront dans l'ordre des ic croissant
		// On effectue donc la fonction trie pour connaitre cet ordre
		uint8_t *ordre = trie(ic_glob);

		//Nombre de mcu
		uint32_t total_mcu = ((horizontale / (8*samp[0])) + (horizontale%(8*samp[0]) > 0 ? 1 : 0)) 
			  	   * ((verticale   / (8*samp[1])) + (verticale%  (8*samp[1]) > 0 ? 1 : 0));

		int16_t **joseph = malloc(total_mcu*sizeof(int16_t*) * somme_samp);

		uint16_t precedent_0 = 0;
		uint16_t precedent_1 = 0;
		uint16_t precedent_2 = 0;

		uint8_t coeff;
		int16_t *element;
		int8_t m;
		for (uint32_t mcu_actuel = 0; mcu_actuel < total_mcu; mcu_actuel++) {
			//Raisonnement "non trivial" ici. On traite bien les vecteurs dans l'ordre croissant des ic, ordre trié dans la liste ordre.
			//Mais on remet dans le bon ordre les composants Y, Cb, Cr en les placant au bon endroit
			//On aura donc chaque MCU, avec d'abord tout les Y, puis tout les Cb, puis tout les Cr
			coeff = ordre[0];
			for (int i = 0; i < samp_glob[coeff] ; i++) {
				m = next_huffman_value(huff_global[coeff*2], stream);
				element = un_seul(m, stream, huff_global[coeff*2 + 1], precedent_0);
				joseph[mcu_actuel * somme_samp + i + (coeff > 0)*(samp_glob[0]) + (coeff > 1)*(samp_glob[1])] = element;
				precedent_0 = element[0];
			}
			coeff = ordre[1];
			for (int i = 0; i < samp_glob[coeff] ; i++) {
				m = next_huffman_value(huff_global[coeff*2], stream);
				element = un_seul(m, stream, huff_global[coeff*2 + 1], precedent_1);
				joseph[mcu_actuel * somme_samp  + i + (coeff > 0)*(samp_glob[0]) + (coeff > 1)*(samp_glob[1])] = element;
				precedent_1 = element[0];
			}
			coeff = ordre[2];
			for (int i = 0; i < samp_glob[coeff] ; i++) {
				m = next_huffman_value(huff_global[coeff*2], stream);
				element = un_seul(m, stream, huff_global[coeff*2 + 1], precedent_2);
				joseph[mcu_actuel * somme_samp + i + (coeff > 0)*(samp_glob[0]) + (coeff > 1)*(samp_glob[1])] = element;
				precedent_2 = element[0];
			}
		}
		//Libération de l'espace alloué
		free(ordre);
		free(huff_global);
		return(joseph);
	} else {
		perror(" Nombre de composante invalide ");
		exit(1);
	}
	return (joseph);
}
