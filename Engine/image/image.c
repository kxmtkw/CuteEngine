#include <stdio.h>
#include <stdlib.h>

#include "common/instructions.h"

#include "image/image.h"



void 
ct_image_free(CtImage* img)
{
	img->header.instruction_count = 0;
	img->header.procedure_count = 0;

	if (img->procedure_table != NULL) {
		free(img->procedure_table);
	}

	if (img->instruction_pool != NULL) {
		free(img->instruction_pool);
	}
};


CtImageStatus 
ct_image_write(CtImage *img, const char *filepath) {

	FILE *fp = fopen(filepath, "wb");
	if (!fp) {return CT_IMAGE_STATUS_FILE_NOT_FOUND;}

	img->header.magic_id = ct_magic_id;

	u_int32_t items_written;

	items_written = fwrite(&img->header, sizeof(CtImageHeader), 1, fp);
	if (items_written != 1) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}

	items_written = fwrite(img->procedure_table, sizeof(CtImageProcedure), img->header.procedure_count, fp);
	if (items_written != img->header.procedure_count) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}

	items_written = fwrite(img->instruction_pool, sizeof(CtInstrSize), img->header.instruction_count, fp);
	if (items_written != img->header.instruction_count) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}

	fclose(fp);
	return CT_IMAGE_STATUS_SUCCESS;
}


CtImageStatus 
ct_image_read(CtImage *img, const char *filepath) {

	FILE *fp = fopen(filepath, "rb");
	if (!fp) {return CT_IMAGE_STATUS_FILE_NOT_FOUND;}

	uint32_t items_read;

	items_read = fread(&img->header, sizeof(CtImageHeader), 1, fp);
	if (items_read != 1) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}

	if (img->header.magic_id != ct_magic_id) {
		return CT_IMAGE_CORRUPTED_FILE;
	}

	img->procedure_table = malloc(sizeof(CtImageProcedure) * img->header.procedure_count);
	items_read = fread(img->procedure_table, sizeof(CtImageProcedure), img->header.procedure_count, fp);
	if (items_read != img->header.procedure_count) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}

	img->instruction_pool = malloc(sizeof(CtInstrSize) * img->header.instruction_count);
	items_read = fread(img->instruction_pool, sizeof(CtInstrSize), img->header.instruction_count, fp);
	if (items_read != img->header.instruction_count) {return CT_IMAGE_STATUS_READ_WRITE_FAILURE;}
	
	fclose(fp);
	return CT_IMAGE_STATUS_SUCCESS;
};
