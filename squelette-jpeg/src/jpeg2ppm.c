#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/bitstream_perso.h"
#include "../include/huffman_perso.h"
#include "../include/jpeg_reader_perso.h"
#include "../include/extraction.h"
#include "../include/zig_zag.h"
#include "../include/quantification.h"
#include "../include/idct_opti.h"
#include "../include/ecriture_ppm.h"
#include "../include/ycbcr_to_rgb.h"
#include "../include/upsampling.h"

int main(int argc, char **argv)
{
	// Test basique si on a bien entré un fichier
	if (argc != 2) {
		printf("Utilisation : %s fichier.jpeg \n", argv[0]);
		exit(1);
	}

	char filename[strlen(argv[1])];
	strcpy(filename, argv[1]);
	//Creation et récupération de la structure jped_desc
	struct jpeg_desc *jdesc = read_jpeg(filename);
	struct bitstream *stream = get_bitstream(jdesc);

	// Extraction des données brutes, effectue ihuffman et irle
	int16_t **element = extraction_totale(stream, jdesc);

	//Recuperation de données
	uint16_t horizontale = get_image_size(jdesc, DIR_H);
	uint16_t verticale   = get_image_size(jdesc, DIR_V);

	uint8_t couleur = get_nb_components(jdesc);

	//Constante necessaire pour le IDCT optimisé
	float v[8];
	v[0] = sqrt(2);
	v[1] = cos(M_PI / 16);
	v[2] = sin(M_PI / 16);
	v[3] = cos(3 * M_PI / 16);
	v[4] = sin(3 * M_PI / 16);
	v[5] = cos(6 * M_PI / 16);
	v[6] = sin(6 * M_PI / 16);
	v[7] = sqrt(8);

	if (couleur == 1) {
	// Si l'image est en noir et blanc
		//Nombre de mcu
		uint32_t total_mcu = ((horizontale >> 3) + (horizontale%8 > 0 ? 1 : 0))
			           * ((verticale   >> 3) + (verticale%8   > 0 ? 1 : 0));
		//Allocation de la mémoire pour les tableaux de mcu
		uint8_t ***Y = malloc(sizeof(int16_t**)*total_mcu);
		uint8_t *quantification1 = get_quantization_table(jdesc, 0);
		for (uint32_t i = 0; i < total_mcu; i++) {
			// Pour chaque vecteur, on multiplie par la table de quantification, zig-zag, idct, et on
			// libère l'adresse qu'on avait alloué dans extraction_total
			quantification_fct(element[i], quantification1);
			Y[i] = idct_opti(zig_zag_inverse(element[i]), v);
			free(element[i]);
		}
		//On ecrit dans le fichier
		ecriture_ppm(NB, filename, horizontale , verticale, Y);
		//Phase de libération
		free(element);
	} else if (couleur == 3){
	//Si l'image est en couleur
		uint8_t samp_glob[3];
		uint8_t samp[6];
		// Ce calcul donne pour chaque MCU, le nombre de bloc de Y, Cb, Cr la composant
		for (uint8_t i = 0; i < 3; i++) {
			samp[2*i]     = get_frame_component_sampling_factor(jdesc, DIR_H, i);
			samp[2*i + 1] = get_frame_component_sampling_factor(jdesc, DIR_V, i);
			samp_glob[i] = samp[2*i]*samp[2*i + 1];
		}
		// Total_mcu vaut maintenant le vrai nombre de mcu
		//total_mcu = total_mcu / samp_glob[0];
		uint32_t total_mcu = ((horizontale / (8*samp[0])) + (horizontale%(8*samp[0]) > 0 ? 1 : 0))
			  * ((verticale   / (8*samp[1])) + (verticale  %(8*samp[1]) > 0 ? 1 : 0));
		//Récupération de toutes les tables de quantification
		uint8_t **quantification = malloc(sizeof(uint8_t *) * 3);
		for (uint8_t i = 0; i < 3; i++) {
			quantification[i] = get_quantization_table(jdesc, get_frame_component_quant_index (jdesc, i));
		}
		//Allocation de la taille pour notre gros tableau
		uint8_t  ***YCbCr = malloc(total_mcu*sizeof(int16_t*)*(samp_glob[0] + samp_glob[1] + samp_glob[2]));

		uint8_t *quanti;
		uint32_t compteur = 0;

		for (uint32_t i = 0; i < total_mcu; i++) {
		//Itération sur chaque MCU
			for (int j = 0; j < 3; j++) {
			//iteration sur chaque composante
				quanti = quantification[j];
				//Recuperation pour la composante j de la bonne table de quantification
				for (int k = 0; k < samp_glob[j]; k++) {
				//Iteration sur le nombre de fois qu'apparait la composante j dans chaque MCU
					quantification_fct(element[compteur], quanti);
					YCbCr[compteur] = idct_opti(zig_zag_inverse(element[compteur]), v);
					// On multiplie par quantification, on applique zig-zag, et idct
					// On libère ensuite l'espace alloué dans extraction_total()
					free(element[compteur]);
					compteur++;
				}
			}
		}
		if (get_frame_component_sampling_factor(jdesc, DIR_V, 0) *
		    get_frame_component_sampling_factor(jdesc, DIR_H, 0) != 1) {
			//Cas ou on doit faire de l'upsampling
			//Recuperation facteur d'échantillonage
			uint8_t samp_fact[6];
			for (uint8_t i = 0; i < 3; i ++) {
				samp_fact[2*i] = get_frame_component_sampling_factor(jdesc, DIR_H, i);
				samp_fact[2*i + 1] = get_frame_component_sampling_factor(jdesc, DIR_V, i);
			}
			uint8_t ***upsamp = upsampling(YCbCr, samp_fact, horizontale, verticale);
			//Du YCbCr en RGB
			ycbcr_to_rgb(horizontale, verticale, upsamp);
			//Ecriture dans le fichier
			ecriture_ppm(RGB, filename, horizontale , verticale, upsamp);
		} else {
			//Cas sans upsampling
			// Transformation du YCBCR en RGB
			ycbcr_to_rgb(horizontale, verticale, YCbCr);

			// Ecriture dans le fichier
			ecriture_ppm(RGB, filename, horizontale , verticale, YCbCr);
		}
		//Liberation de l'espace
		free(element);
		free(quantification);
	} else {
		perror("Le nombre de composants n'est pas valide");
		exit(1);
	}
	//Libération finale de notre jpeg
	close_jpeg(jdesc);
	return(EXIT_SUCCESS);
}
