
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitstream_perso.h"
#include "huffman_perso.h"
#include "jpeg_reader_perso.h"

struct jpeg_desc {
	const char *filename;
	//Stream
	struct bitstream *stream;
	//Table de quantification
	uint8_t nb_quanti_table;
	uint8_t ***quanti_table;
	//Table de huffman
	uint8_t nb_huff_table;
	uint8_t **caract_huffman;
	struct huff_table **huffman_table;
	//Information sur l'image
	uint16_t *taille_image;
	uint8_t nb_composant;
	uint8_t **donnees_composante;
	//Start of scan
	uint8_t **component_info;
	uint32_t nb_composante_sos;
};

uint16_t recuperation_DQT(struct bitstream *stream, struct jpeg_desc *jpeg)
{
	//Recuperation du champ DQT
	uint8_t nb_table_quanti = jpeg->nb_quanti_table;
	uint8_t nb_lue;
	uint32_t donnees;

	while (true) {
		uint32_t taille_section;
		nb_lue = read_bitstream(stream, 16, &taille_section, false);
		taille_section = (taille_section - 2)/(65);
		// On a potentiellement plusieurs champs DQT, on doit reallouer de la mémoire avec la bonne taille
		jpeg->quanti_table = realloc(jpeg->quanti_table, sizeof(uint8_t **)*(nb_table_quanti + taille_section));
		for (uint32_t i = 0; i < taille_section; i++) {
			//On alloue la mémoire pour chaque tableau de quantification
			jpeg->quanti_table[i + nb_table_quanti] = malloc(sizeof(uint8_t *)*2);
			jpeg->quanti_table[i + nb_table_quanti][0] = malloc(sizeof(uint8_t));
			jpeg->quanti_table[i + nb_table_quanti][1] = malloc(sizeof(uint8_t)*64);
			nb_lue = read_bitstream(stream, 4, &donnees, false);
			nb_lue = read_bitstream(stream, 4, &donnees, false);
			//On écrit la valeur de iq
			jpeg->quanti_table[i + nb_table_quanti][0][0] = (uint8_t) donnees;
			for (int k = 0; k < 64; k++){
				//Recuperation de chaque valeur de la table
				nb_lue = read_bitstream(stream, 8, &donnees, false);
				jpeg->quanti_table[i + nb_table_quanti][1][k] = (uint8_t) donnees;
			}
		}
		nb_table_quanti += taille_section;
		nb_lue = read_bitstream(stream, 16, &donnees, false);
		//Condition d'arrêt, si l'entête est celle de DQT on continue
		if ( ( (uint16_t) donnees) != 0xffdb) {
			break;
		}

	}
	jpeg->nb_quanti_table = (uint8_t) nb_table_quanti;
	//On retourne l'entête du prochain champ
	return( (uint16_t) donnees);
}

void recuperation_SOF(struct bitstream *stream, struct jpeg_desc *jpeg)
{
	uint32_t longueur_section;
	uint32_t donnees;
	uint8_t nb_lue = read_bitstream(stream, 16, &longueur_section, false);
	nb_lue = read_bitstream(stream, 8, &donnees, false);
	//Allocation de la mémoire, écriture des informations de base
	jpeg->taille_image = malloc(sizeof(uint16_t)*2);

	nb_lue = read_bitstream(stream, 16, &donnees, false);
	jpeg->taille_image[DIR_V] = (uint16_t) donnees;
	nb_lue = read_bitstream(stream, 16, &donnees, false);
	jpeg->taille_image[DIR_H] = (uint16_t) donnees;

	nb_lue = read_bitstream(stream, 8, &donnees, false);
	jpeg->nb_composant = (uint8_t) donnees;

	jpeg->donnees_composante = malloc(jpeg->nb_composant*sizeof(uint8_t *));
	for (uint32_t i = 0; i < jpeg->nb_composant; i++) {
		//Recuperation de l'information sur chaque composante
		jpeg->donnees_composante[i] = malloc(sizeof(uint8_t)*4);

		nb_lue = read_bitstream(stream, 8, &donnees, false);
		jpeg->donnees_composante[i][0] = (uint8_t) donnees; //Ic

		nb_lue = read_bitstream(stream, 4, &donnees, false);
		jpeg->donnees_composante[i][1] = (uint8_t) donnees; //facteur echantillonnage horizon

		nb_lue = read_bitstream(stream, 4, &donnees, false);
		jpeg->donnees_composante[i][2] = (uint8_t) donnees; //facteur echantillonnage vertical

		nb_lue = read_bitstream(stream, 8, &donnees, false);
		jpeg->donnees_composante[i][3] = (uint8_t) donnees; //table iq
	}
}

uint16_t  recuperation_DHT(struct bitstream *stream, struct jpeg_desc *jpeg)
{
	uint32_t donnees;
	uint8_t nb_lues = read_bitstream(stream, 19, &donnees, false);
	uint16_t inutile = 0;

	while (true) {
		// On ne connait pas la taille précise que va "absorber" load huffman
		//  on tourne tout le temps en attendant une condition d'arrêt
		uint8_t nb = jpeg->nb_huff_table;
		jpeg->caract_huffman = realloc(jpeg->caract_huffman ,(1+nb)*sizeof(uint8_t *));
		jpeg->caract_huffman[nb] = malloc(sizeof(uint8_t)*2);
		jpeg->huffman_table = realloc(jpeg->huffman_table, (1+nb)*(sizeof(struct huff_table *)));

		nb_lues = read_bitstream(stream, 1, &donnees, false);
		jpeg->caract_huffman[nb][0] = (uint8_t) donnees;
		nb_lues = read_bitstream(stream, 4, &donnees, false);
		jpeg->caract_huffman[nb][1] = (uint8_t) donnees;

		jpeg->huffman_table[nb] = load_huffman_table(stream, &inutile);
		jpeg->nb_huff_table++;

		nb_lues = read_bitstream(stream, 3, &donnees, false);
		// On sait que les trois premiers bites doivent être 000, donc on utilise cette valeur comme condition d'arrêt
		if (donnees != 0) {
			break;
		}
	}
	uint32_t donnees_supplementaire;
	nb_lues = read_bitstream(stream, 13, &donnees_supplementaire, false);
	//On retourne la valeur des 16 bites, permettant de connaitre l'entête du futur champ
	return( (uint16_t) ((donnees_supplementaire) | (donnees << 13) ) );
}

void recuperation_SOS(struct bitstream *stream, struct jpeg_desc *jpeg)
{
	uint32_t donnees;
	uint32_t nb_composante;
	uint8_t nb_lue = read_bitstream(stream, 16, &donnees, false);
	nb_lue = read_bitstream(stream, 8, &nb_composante, false);
	jpeg->nb_composante_sos = nb_composante;
	//Allocation mémoire
	jpeg->component_info = malloc(nb_composante*sizeof(uint8_t *));
	for (uint32_t i = 0; i < nb_composante; i++) {

		jpeg->component_info[i] = malloc(sizeof(uint8_t)*3);
		nb_lue = read_bitstream(stream, 8, &donnees, false);
		//Identifiant Ic
		jpeg->component_info[i][0] = (uint8_t) donnees;
		//Indice de Huffman DC
		nb_lue = read_bitstream(stream, 4, &donnees, false);
		jpeg->component_info[i][1] = (uint8_t) donnees;
		//Indice de Huffman AC
		nb_lue = read_bitstream(stream, 4, &donnees, false);
		jpeg->component_info[i][2] = (uint8_t) donnees;

	}
	//Saut de valeur inutile pour non-progressif
	nb_lue = read_bitstream(stream, 24, &donnees, false);
}

void test_APPx(struct bitstream *stream)
{
	uint8_t nb_lue;
	uint32_t donnees_JFIF;
	uint32_t donnees_0;
	uint8_t  donnees_0_8;
	nb_lue = read_bitstream(stream, 16, &donnees_JFIF, false);
	nb_lue = read_bitstream(stream, 32, &donnees_JFIF, false);
	nb_lue = read_bitstream(stream, 8, &donnees_0, false);
	donnees_0_8 = (uint8_t) donnees_0;
	if ((donnees_JFIF != 0x4a464946) || (donnees_0_8 != 0x00)){
		perror("Le champ APPx ne possède pas la valeur JFIF ");
		exit(1);
	}
}

struct jpeg_desc *read_jpeg(const char *filename) 
{
	//Initialisation
	struct jpeg_desc *jpeg = malloc(sizeof(struct jpeg_desc));
	jpeg->quanti_table = NULL;
	jpeg->caract_huffman = NULL;
	jpeg->huffman_table = NULL;
	jpeg->filename = filename;
	jpeg->nb_quanti_table = 0;
	jpeg->nb_huff_table = 0;

	struct bitstream *stream =  create_bitstream(filename);
	uint8_t nb_lue;
	uint32_t donnees;

	nb_lue = read_bitstream(stream, 16, &donnees, false);
	uint16_t donnees_16 = (uint16_t) donnees;
	if (donnees_16 != 0xffd8) {
		perror("L'entête du JPEG n'est pas bon.");
		exit(1);
	}
	while (nb_lue == 16) {
		//A chaque étape, on regarde la valeur de l'entête, et en fonction on rentre dans la bonne fonction
		//Le dernier champ étant SOS, on s'arrête après l'avoir traité
		donnees_16 = (uint16_t) donnees;
		if (donnees_16 == 0xffdb) {
			donnees = recuperation_DQT(stream, jpeg);
		} else if (donnees_16 == 0xffe0) {
			test_APPx(stream);
			nb_lue = read_bitstream(stream, 16, &donnees, false);
		} else if (donnees_16 == 0xffc0) {
			recuperation_SOF(stream, jpeg);
			nb_lue = read_bitstream(stream, 16, &donnees, false);
		} else if (donnees_16 == 0xffc4) {
			donnees = recuperation_DHT(stream, jpeg);
		} else if ( donnees_16 == 0xffda) {
			recuperation_SOS(stream, jpeg);
			break;
		} else {
			skip_bitstream_until(stream, 0xff);
			nb_lue = read_bitstream(stream, 16, &donnees, false);
		}
	}
	if (nb_lue != 16) {
		perror(" On est arrivé à la fin du fichier sans lire de SOS");
		exit(1);
	}
	jpeg->stream = stream;
	return (jpeg);
}

//Ensemble de fonction de retour de valeur

const char *get_filename(const struct jpeg_desc *jpeg)
{	
	return (jpeg->filename);
}

struct bitstream *get_bitstream(const struct jpeg_desc *jpeg)
{
	return (jpeg->stream);
}

uint8_t get_nb_quantization_tables(const struct jpeg_desc *jpeg)
{
	return (jpeg->nb_quanti_table);
}

uint8_t *get_quantization_table(const struct jpeg_desc *jpeg, uint8_t index)
{
	uint8_t nb = (int16_t) jpeg->nb_quanti_table;
	for (int16_t i = nb - 1; i > -1 ; i--) {
		//On lit à l'envers, car potentiellement on a deux tables avec le même valeur d'index
		//On choisit donc celle qui est arrivé le plus tard dans notre ensemble de table de quantitification
		if (jpeg->quanti_table[i][0][0] == index) {
			return (jpeg->quanti_table[i][1]);
		}
	}
	exit(1);
}

uint8_t get_nb_huffman_tables(const struct jpeg_desc *jpeg,enum acdc acdc)
{
	uint8_t compteur = 0;
	for (uint8_t i = 0; i < jpeg->nb_huff_table; i++) {
		if (jpeg->caract_huffman[i][0] == acdc) {
			compteur++;
		}
	}
	return (compteur);
}

struct huff_table *get_huffman_table(const struct jpeg_desc *jpeg,enum acdc acdc, uint8_t index)
{
	for (uint8_t i = 0; i < jpeg->nb_huff_table; i++) {
		if ((jpeg->caract_huffman[i][0] == acdc) & (jpeg->caract_huffman[i][1] == index)) {
			//On teste si on a le bon index, et le bon acdc
			return (jpeg->huffman_table[i]);
		}
	}
	exit(1);
}

uint16_t get_image_size(struct jpeg_desc *jpeg, enum direction dir)
{
	return(jpeg->taille_image[dir]);
}

uint8_t get_nb_components(const struct jpeg_desc *jpeg)
{
	return(jpeg->nb_composant);
}

uint8_t get_frame_component_id(const struct jpeg_desc *jpeg,uint8_t frame_comp_index)
{
	return(jpeg->donnees_composante[frame_comp_index][0]);
}

uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jpeg, enum direction dir, uint8_t frame_comp_index){
	return(jpeg->donnees_composante[frame_comp_index][1 + (dir == DIR_V)]);
}

uint8_t get_frame_component_quant_index(const struct jpeg_desc *jpeg, uint8_t frame_comp_index)
{
	return(jpeg->donnees_composante[frame_comp_index][3]);
}

uint8_t get_scan_component_id(const struct jpeg_desc *jpeg, uint8_t scan_comp_index)
{
	return(jpeg->component_info[scan_comp_index][0]);
}

uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg, enum acdc acdc, uint8_t scan_comp_index)
{
	return(jpeg->component_info[scan_comp_index][1 + acdc]);
}

void close_jpeg(struct jpeg_desc *jpeg)
{
	//libération SOS
	for (uint8_t i = 0; i < jpeg->nb_composante_sos; i++) {
		free(jpeg->component_info[i]);
	}
	free(jpeg->component_info);

	//libération DHT
	for (uint32_t i = 0; i < jpeg->nb_huff_table; i++) {
		free(jpeg->caract_huffman[i]);
		free_huffman_table(jpeg->huffman_table[i]);
	}
	free(jpeg->caract_huffman);
	free(jpeg->huffman_table);

	//libération DQT
	for (uint8_t i = 0; i < jpeg->nb_quanti_table; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			free(jpeg->quanti_table[i][j]);
		}
		free(jpeg->quanti_table[i]);
	}
	free(jpeg->quanti_table);

	//libération SOF
	free(jpeg->taille_image);
	for (uint32_t i = 0; i < jpeg->nb_composant; i++) {
		free(jpeg->donnees_composante[i]);
	}
	free(jpeg->donnees_composante);
	close_bitstream(jpeg->stream);
	free(jpeg);
}
