#ifndef CUTE_INSTR_H
#define CUTE_INSTR_H

// Main Cute Instruction Set.
#include <stdint.h>

typedef enum {

    CT_INSTR_NULL         = 0x00,
    CT_INSTR_HALT         = 0x01,
    CT_INSTR_OUT          = 0x02,
    
    CT_INSTR_MOV          = 0x20,
    CT_INSTR_SETI         = 0x21,
    CT_INSTR_SETU         = 0x22,
    CT_INSTR_SETF         = 0x23,

    CT_INSTR_CAST_I2F     = 0x2A,
    CT_INSTR_CAST_F2I     = 0x2B,
    CT_INSTR_CAST_U2F     = 0x2C,
    CT_INSTR_CAST_F2U     = 0x2D,

    CT_INSTR_ADDI         = 0x30,
    CT_INSTR_SUBI         = 0x31,
    CT_INSTR_MULI         = 0x32,
    CT_INSTR_DIVI         = 0x33,
    CT_INSTR_MODI         = 0x34,
    CT_INSTR_NEGI         = 0x35,
    CT_INSTR_ABSI         = 0x36,
    CT_INSTR_INCI         = 0x37,
    CT_INSTR_DECI         = 0x38,

    CT_INSTR_ADDU         = 0x40,
    CT_INSTR_SUBU         = 0x41,
    CT_INSTR_MULU         = 0x42,
    CT_INSTR_DIVU         = 0x43,
    CT_INSTR_MODU         = 0x44,
    CT_INSTR_INCU         = 0x45,
    CT_INSTR_DECU         = 0x46,

    CT_INSTR_ADDF         = 0x50,
    CT_INSTR_SUBF         = 0x51,
    CT_INSTR_MULF         = 0x52,
    CT_INSTR_DIVF         = 0x53,
    CT_INSTR_NEGF         = 0x54,
    CT_INSTR_ABSF         = 0x55,    

    CT_INSTR_LOGIC_AND    = 0x60,
    CT_INSTR_LOGIC_OR     = 0x61,
    CT_INSTR_LOGIC_NOT    = 0x62,
    CT_INSTR_LOGIC_XOR    = 0x63,

    CT_INSTR_BIT_AND      = 0x70,
    CT_INSTR_BIT_OR       = 0x71,
    CT_INSTR_BIT_NOT      = 0x73,
    CT_INSTR_BIT_XOR      = 0x74,
    CT_INSTR_BIT_SHL      = 0x75,
    CT_INSTR_BIT_SHR      = 0x76, 
    CT_INSTR_BIT_SHRA     = 0x77, 

    CT_INSTR_CMPI         = 0x80,
    CT_INSTR_CMPU         = 0x81,
    CT_INSTR_CMPF         = 0x82,

    CT_INSTR_EQ           = 0x90,
    CT_INSTR_NOT_EQ       = 0x91,
    CT_INSTR_LESS         = 0x92,
    CT_INSTR_LESS_EQ      = 0x93,
    CT_INSTR_GREATER      = 0x94,
    CT_INSTR_GREATER_EQ   = 0x95,

    CT_INSTR_JMP          = 0xA0,
    CT_INSTR_JMP_EQ       = 0xA1,
    CT_INSTR_JMP_NE       = 0xA2,
    CT_INSTR_JMP_GT       = 0xA3,
    CT_INSTR_JMP_GE       = 0xA4,
    CT_INSTR_JMP_LT       = 0xA5,
    CT_INSTR_JMP_LE       = 0xA6,
	CT_INSTR_JMP_IF       = 0xA7,
	CT_INSTR_JMP_IFNOT    = 0xA8,

    CT_INSTR_CALL         = 0xB0,
    CT_INSTR_RETURN       = 0xB1,
    CT_INSTR_RETURN_VAL   = 0xB2,
    CT_INSTR_MOD_CALL     = 0xBA,

    CT_INSTR_CON_NEW      = 0xC0,
    CT_INSTR_CON_GET      = 0xC1,
    CT_INSTR_CON_SET      = 0xC2,
    CT_INSTR_CON_SIZE     = 0xC3,
    CT_INSTR_CON_COPY     = 0xC4,

} CtInstr;

// All instructions should be able to fit inside this. Allows for 256 different instructions
typedef uint8_t CtInstrSize;


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


#endif // CUTE_INSTR_H