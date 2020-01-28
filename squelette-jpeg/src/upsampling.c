#include <stdint.h>
#include <stdlib.h>
#include "../include/upsampling.h"
#include <stdio.h>

static void freeligne(uint32_t ligne_MCU, uint8_t ***MCUs, uint32_t nbrMCU)
{
	for (uint32_t i = 0; i < nbrMCU; i++){
		for (uint8_t j = 0; j < 8; j++){
			free(MCUs[ligne_MCU*nbrMCU + i][j]);
		}
		free(MCUs[ligne_MCU*nbrMCU + i]);
	}
}

static void clonage(uint8_t **retour, uint8_t **base)
{
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			retour[i][j] = base[i][j];
			//crétion d'un clone de la cellule
		}
	}
}

static void division(uint8_t **retour, uint8_t **base,
              uint8_t largeur , uint8_t hauteur,
              uint8_t X       , uint8_t Y)
{
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t j = 0; j < 8; j++){
			retour[i][j] = base[(Y * hauteur) + (i * hauteur / 8)]
			[(X * largeur) + (j * largeur / 8)];
			//création d'un clone avec multiplication de la cellule
		}
	}
}

uint8_t  ***upsampling(uint8_t ***MCUs, uint8_t hv[6],
                       uint32_t plignes, uint32_t pcolonnes)
{
/*déséchantillone la liste de MCU et retourne une nouvelle liste déséchantilloné
arg: liste_des_mcu, liste_de_hi_vi, pixels par lignes, par colonnes */
	uint8_t h1=hv[0], v1=hv[1], h2=hv[2], v2=hv[3], h3=hv[4], v3=hv[5];
	//extraction des hi et vi
	uint8_t taille = h1*v1 + h2*v2 + h3*v3;
	//taille d'une MCU échantillonée
  uint8_t error = 0;
  if (h1 > 4){
    fprintf(stderr,"h1: %d", h1);
    perror("la valeur de h1 est trop grande");
    error = 1;
  }
  if (h2 > 4){
    fprintf(stderr,"h2: %d", h2);
    perror("la valeur de h2 est trop grande");
    error = 1;
  }
  if (h3 > 4){
    fprintf(stderr,"h3: %d", h3);
    perror("la valeur de h3 est trop grande");
    error = 1;
  }
  if (v1 > 4){
    fprintf(stderr,"v1: %d", v1);
    perror("la valeur de v1 est trop grande");
    error = 1;
  }
  if (v2 > 4){
    fprintf(stderr,"v2: %d", v2);
    perror("la valeur de v2 est trop grande");
    error = 1;
  }
  if (v3 > 4){
    fprintf(stderr,"v3: %d", v3);
    perror("la valeur de v3 est trop grande");
    error = 1;
  }
  if (taille > 10){
    fprintf(stderr,"h1: %d ,h2: %d, h3: %d \nv1: %d ,v2: %d ,v3: %d",
    h1, h2, h3, v1, v2, v3);
    perror("la somme des hi*vi est supèrieur à 10");
    error = 1;
  }
  if (h1 % h2 != 0){
    fprintf(stderr,"h1: %d", h2);
    perror("h2 ne divise pas parfaitement h1");
    error = 1;
  }
  if (h1 % h3 != 0){
    fprintf(stderr,"h1: %d", h3);
    perror("h3 ne divise pas parfaitement h1");
    error = 1;
  }
  if (v1 % v2 != 0){
    fprintf(stderr,"h1: %d", v2);
    perror("v2 ne divise pas parfaitement v1");
    error = 1;
  }
  if (v1 % v3 != 0){
    fprintf(stderr,"h1: %d", v3);
    perror("v3 ne divise pas parfaitement v1");
    error = 1;
  }

  if (error == 1){
    exit(1);
  }
	uint8_t taillef = h1 * v1 * 3;
	//taille d'une MCU déséchantillonée
	uint32_t mcuparlignes = plignes /(8*h1) + (plignes%(8*h1) == 0 ? 0 : 1);
	//nombre de MCU par ligne (sur l'image)
	uint32_t mcuparcolonnes = pcolonnes/(8*v1)+(pcolonnes%(8*v1) == 0 ? 0 : 1);
	//par colonnes
	uint32_t blockparcolonnes = pcolonnes / 8 + (pcolonnes%8 == 0 ? 0 : 1);
	uint32_t blockparlignes = plignes / 8 + (plignes%8 == 0 ? 0 : 1);
 	uint32_t nbr_YCbCr = blockparcolonnes*blockparlignes;
	//nombre total de MCU
	uint8_t ***retour = malloc(nbr_YCbCr * taillef * sizeof(**retour));
	//vecteur contenant les Y Cb Cr déséchantilloné
	for (uint32_t i = 0; i < nbr_YCbCr * 3; i++){
	//allocation mémoire
		retour[i] = malloc(8 * sizeof(*retour));
		for (uint32_t j = 0; j < 8; j++){
			retour[i][j] = malloc(8 * sizeof(uint8_t));
		}
	}
  uint32_t fin, fin2, fin3;
  fin = 0;	//indice de la fin de "retour"
	for (uint32_t ligne_MCU = 0; ligne_MCU < mcuparcolonnes; ligne_MCU++){
	//la ligne que nous somme en train d'étudier
		for (uint8_t nLigne = 0; nLigne < v1; nLigne++){
		//la ligne de la MCU étudier
			for (uint32_t nMCU = 0; nMCU < mcuparlignes; nMCU++){
			//la MCU étudier
				fin2 = (nMCU + ligne_MCU * mcuparlignes) * taille;
				//indice pour se placer au début de la MCU utilisé
				for (uint8_t nBlock = 0; nBlock < h1; nBlock++){
				//le Block de la ligne de la MCU étudie
					if ((nMCU * h1 + nBlock < blockparlignes) &&
					   (ligne_MCU * v1 + nLigne < blockparcolonnes)){
						clonage(retour[fin], MCUs[fin2 + nLigne * h1 + nBlock]);
						//clone Y dans retour (Y n'est jamais déséchantilloné
						division(retour[fin + 1],
						MCUs[fin2+h1*v1 + h2*((nLigne*v2)/v1) + (nBlock*h2)/h1],
						h2*8/h1, v2*8/v1,
						nBlock%(h1/h2), nLigne%(v1/v2));
						//déséchantillonage de Cb
						fin3 = fin2 + h1*v1 + h2*v2;
						//indice debut des Cr
						division(retour[fin + 2],
						MCUs[fin3 + h3*((nLigne*v3)/v1) + (nBlock*h3)/h1],
						h3*8/h1, v3*8/v1,
						nBlock%(h1/h3), nLigne%(v1/v3));
						//déséchantillonage de Cr
/* le tableau 8*8 où on enregistre le déséchantillonement,
le taleau qu'on déséchantillone,
la largeur et la hauteur de la zone a déséchantillonée,
les coordonées de la zone a déséchantillonée */
						fin += 3;   //incrémente l'indice de fin
					}
				}
			}
		}
		freeligne(ligne_MCU, MCUs, mcuparlignes * taille);
	}
	free(MCUs);
	return(retour);
}
