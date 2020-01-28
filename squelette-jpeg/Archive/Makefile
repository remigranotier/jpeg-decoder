# Repertoires du projet

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TESTS_DIR = tests

# Options de compilation/édition des liens

CC = clang
LD = clang
INC = -I$(INC_DIR)

CFLAGS += $(INC) -Wall -std=c99 -O0 -g  -Wextra -fstack-protector-all -D_GNU_SOURCE
LDFLAGS = -lm

# Liste des fichiers objet

OBJ_FILES = $(OBJ_DIR)/ecriture_ppm.o $(OBJ_DIR)/extraction.o $(OBJ_DIR)/idct_opti.o $(OBJ_DIR)/quantification.o $(OBJ_DIR)/ycbcr_to_rgb.o $(OBJ_DIR)/zig_zag.o  $(OBJ_DIR)/jpeg2ppm.o $(OBJ_DIR)/jpeg_reader_perso.o $(OBJ_DIR)/bitstream_perso.o $(OBJ_DIR)/huffman_perso.o $(OBJ_DIR)/upsampling.o

# Cible par défaut

TARGET = $(BIN_DIR)/jpeg2ppm

# Règles

all: $(TARGET) # make

obj: $(OBJ_DIR)/$(OBJET).o # make obj OBJET=<nom_fichier>

test: $(OBJ_DIR)/$(OBJET).o # make test OBJET=<nom_fichier>
	$(CC) $(CFLAGS) -c $(TESTS_DIR)/$(OBJET)_test.c -o $(TESTS_DIR)/$(OBJET)_test.o
	$(LD) $(LDFLAGS) $(TESTS_DIR)/$(OBJET)_test.o $(OBJ_DIR)/$(OBJET).o $(AUX) -o $(TESTS_DIR)/$(OBJET)_test

# Linkage

$(TARGET): $(OBJ_FILES)
	@echo
	@echo "Petit Jésus, fais que ça compile stp."
	@echo
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o $(TARGET)

# Compilation des différents fichiers objets

$(OBJ_DIR)/jpeg2ppm.o: $(SRC_DIR)/jpeg2ppm.c $(INC_DIR)/bitstream_perso.h $(INC_DIR)/huffman_perso.h $(INC_DIR)/jpeg_reader_perso.h $(INC_DIR)/extraction.h $(INC_DIR)/quantification.h $(INC_DIR)/zig_zag.h $(INC_DIR)/idct_opti.h $(INC_DIR)/ycbcr_to_rgb.h $(INC_DIR)/ecriture_ppm.h $(INC_DIR)/upsampling.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg2ppm.c -o $(OBJ_DIR)/jpeg2ppm.o

$(OBJ_DIR)/ecriture_ppm.o: $(SRC_DIR)/ecriture_ppm.c $(INC_DIR)/ecriture_ppm.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ecriture_ppm.c -o $(OBJ_DIR)/ecriture_ppm.o

$(OBJ_DIR)/extraction.o: $(SRC_DIR)/extraction.c $(INC_DIR)/bitstream_perso.h $(INC_DIR)/huffman_perso.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/extraction.c -o $(OBJ_DIR)/extraction.o

$(OBJ_DIR)/idct_opti.o: $(SRC_DIR)/idct_opti.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/idct_opti.c -o $(OBJ_DIR)/idct_opti.o

$(OBJ_DIR)/quantification.o: $(SRC_DIR)/quantification.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/quantification.c -o $(OBJ_DIR)/quantification.o

$(OBJ_DIR)/ycbcr_to_rgb.o: $(SRC_DIR)/ycbcr_to_rgb.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ycbcr_to_rgb.c -o $(OBJ_DIR)/ycbcr_to_rgb.o

$(OBJ_DIR)/zig_zag.o: $(SRC_DIR)/zig_zag.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/zig_zag.c -o $(OBJ_DIR)/zig_zag.o

$(OBJ_DIR)/bitstream_perso.o: $(SRC_DIR)/bitstream_perso.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/bitstream_perso.c -o $(OBJ_DIR)/bitstream_perso.o

$(OBJ_DIR)/upsampling.o: $(SRC_DIR)/upsampling.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/upsampling.c -o $(OBJ_DIR)/upsampling.o

$(OBJ_DIR)/jpeg_reader_perso.o: $(SRC_DIR)/jpeg_reader_perso.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg_reader_perso.c -o $(OBJ_DIR)/jpeg_reader_perso.o

$(OBJ_DIR)/huffman_perso.o: $(SRC_DIR)/huffman_perso.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/huffman_perso.c -o $(OBJ_DIR)/huffman_perso.o

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES)
