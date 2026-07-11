#ifndef CUTE_INSTR_H
#define CUTE_INSTR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {

    instrNull    = 0x00,
    instrHalt    = 0x01,
    instrAssert  = 0x02,

    instrOut     = 0x10,
    instrOutBits = 0x11,
    
    instrMov     = 0x20,
    instrSetI    = 0x21,
    instrSetU    = 0x22,
    instrSetF    = 0x23,
    instrSetB    = 0x24,
    instrSetN    = 0x25,

    instrAddI    = 0x30,
    instrSubI    = 0x31,
    instrMulI    = 0x32,
    instrDivI    = 0x33,
    instrModI    = 0x34,
    instrNegI    = 0x35,
    instrAbsI    = 0x36,
    instrIncI    = 0x37,
    instrDecI    = 0x38,

    instrAddU    = 0x40,
    instrSubU    = 0x41,
    instrMulU    = 0x42,
    instrDivU    = 0x43,
    instrModU    = 0x44,
    instrIncU    = 0x45,
    instrDecU    = 0x46,

    instrAddF    = 0x50,
    instrSubF    = 0x51,
    instrMulF    = 0x52,
    instrDivF    = 0x53,
    instrNegF    = 0x54,
    instrAbsF    = 0x55,    

    instrLogicAnd    = 0x60,
    instrLogicOr     = 0x61,
    instrLogicNot    = 0x62,
    instrLogicXor    = 0x63,

    instrBitAnd      = 0x70,
    instrBitOr       = 0x71,
    instrBitNot      = 0x73,
    instrBitXor      = 0x74,
    instrBitShl      = 0x75,
    instrBitShr      = 0x76, 

    instrCmpI        = 0x80,
    instrCmpU        = 0x81,
    instrCmpF        = 0x82,

    instrEq          = 0x90,
    instrNotEq       = 0x91,
    instrLess        = 0x92,
    instrLessEq      = 0x93,
    instrGreater     = 0x94,
    instrGreaterEq   = 0x95,

    instrJmp         = 0xA0,
    instrJmpIf       = 0xA1,
    instrJmpIfNot    = 0xA2,
    instrJmpAbs          = 0xA3,
    instrJmpAbsIf        = 0xA4,
    instrJmpAbsIfNot     = 0xA5,

    instrCall        = 0xB0,
    instrReturn      = 0xB1,
    instrReturnVal   = 0xB2,
    instrModCall     = 0xBA,

    instrConNew         = 0xC0,
    instrConDel         = 0xC1,
    instrConGet         = 0xC2,
    instrConSet         = 0xC3,
    instrConSize        = 0xC4,
    instrConCopy        = 0xC5,
    instrConDeepCopy    = 0xC6,
    instrConResize      = 0xC7,
    instrConLoad        = 0xC8

} ctInstruction;

typedef uint8_t ctInstructionSize;


static const uint32_t ctMagicId = 0x12345678; 

typedef struct {
    uint32_t magic_id;
    uint32_t procedure_count;
    uint32_t instruction_count;
    uint32_t procedure_table_offset;
    uint32_t instruction_pool_offset; 
} ctImageHeader;


typedef struct {
    uint32_t id;
    uint32_t bytecode_index;
    uint32_t arg_count;
} ctImageProcedure;


typedef struct {
    ctImageHeader      header;
    ctImageProcedure* procedure_table;
    ctInstructionSize* instruction_pool;
} ctImage;


typedef enum {
    ctImageCode_Success,
    ctImageCode_FileNotFound,
    ctImageCode_ReadWriteFailure,
    ctImageCode_InvalidImage
} ctImageCode;


// Write an already initialized image to a file
ctImageCode 
ct_image_write(ctImage *img, const char *filepath);

// Set an image from a file
ctImageCode 
ct_image_read(ctImage *img, const char *filepath);

// Free the image's resources.
void 
ct_image_free(ctImage *img);

#ifdef __cplusplus
}
#endif

#endif // CUTE_INSTR_H