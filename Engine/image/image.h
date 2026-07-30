#ifndef CUTE_IMAGE_H
#define CUTE_IMAGE_H

#include <stdint.h>

#include "common/instructions.h"

static const uint32_t ct_magic_id = 0x12345678; 

typedef struct {
	uint32_t magic_id;
	uint32_t padding;
	uint32_t procedure_count;
	uint32_t instruction_count;
	uint32_t procedure_table_offset;
	uint32_t instruction_pool_offset; 
} CtImageHeader;


typedef struct {
	uint32_t bytecode_index;
	uint32_t arg_count;
} CtImageProcedure;


typedef struct {
	CtImageHeader       header;
	CtImageProcedure*   procedure_table;
	CtInstrSize*        instruction_pool;
} CtImage;


typedef enum {
	CT_IMAGE_STATUS_SUCCESS,
	CT_IMAGE_STATUS_FILE_NOT_FOUND,
	CT_IMAGE_STATUS_READ_WRITE_FAILURE,
	CT_IMAGE_CORRUPTED_FILE
} CtImageStatus;


// Write an already initialized image to a file
CtImageStatus
ct_image_write(CtImage *img, const char *filepath);

// Set an image from a file
CtImageStatus
ct_image_read(CtImage *img, const char *filepath);

// Free the image's resources.
void 
ct_image_free(CtImage *img);


#endif // 