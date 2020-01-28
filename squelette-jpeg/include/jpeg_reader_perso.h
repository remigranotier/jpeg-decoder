#ifndef __JPEG_DESC_H__
#define __JPEG_DESC_H__

#include <stdint.h>
#include <stdbool.h>

#include "bitstream_perso.h"

enum direction {
    DIR_H,
    DIR_V,
    /* sentinelle */
    DIR_NB
};

enum acdc {
    DC = 0,
    AC = 1,
    /* sentinelle */
    ACDC_NB
};


struct jpeg_desc;


// Fonction générale
extern struct jpeg_desc *read_jpeg(const char *filename);
extern void close_jpeg(struct jpeg_desc *jpeg);
extern const char *get_filename(const struct jpeg_desc *jpeg);

// Stream, au début des données brutes
extern struct bitstream *get_bitstream(const struct jpeg_desc *jpeg);

// Information issue de DQT
extern uint8_t get_nb_quantization_tables(const struct jpeg_desc *jpeg);
extern uint8_t *get_quantization_table(const struct jpeg_desc *jpeg,
                                       uint8_t index);

// Information issue de DHT
extern uint8_t get_nb_huffman_tables(const struct jpeg_desc *jpeg,
                                     enum acdc acdc);
extern struct huff_table *get_huffman_table(const struct jpeg_desc *jpeg,
                                            enum acdc acdc, uint8_t index);

// Information issue de  SOF0
extern uint16_t get_image_size(struct jpeg_desc *jpeg, enum direction dir);
extern uint8_t get_nb_components(const struct jpeg_desc *jpeg);

extern uint8_t get_frame_component_id(const struct jpeg_desc *jpeg,
                                      uint8_t frame_comp_index);
extern uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jpeg,
                                                   enum direction dir,
                                                   uint8_t frame_comp_index);
extern uint8_t get_frame_component_quant_index(const struct jpeg_desc *jpeg,
                                               uint8_t frame_comp_index);

// Information issue de  SOS
extern uint8_t get_scan_component_id(const struct jpeg_desc *jpeg,
                                     uint8_t scan_comp_index);
extern uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg,
                                                enum acdc,
                                                uint8_t scan_comp_index);

#endif
