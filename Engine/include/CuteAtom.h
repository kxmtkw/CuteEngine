
#ifndef CUTE_ATOM_H
#define CUTE_ATOM_H

#include <stdbool.h>
#include <stdint.h>


typedef enum {
	ctAtomType_NoneType   = 0x0,
	ctAtomType_Primitive  = 0x1,
	ctAtomType_Object     = 0x2
} ctAtomType;


typedef uint8_t ctAtomTypeSize;

struct _ctObject;

// Main atom definition. Its just 64 bits.
typedef union {
	uint64_t             raw;
	int64_t              as_int;
	uint64_t             as_uint;
	double               as_float;
	uint64_t             as_bool;
	struct _ctObject*    as_object;
} ctAtom;


typedef struct {
	ctAtomTypeSize type;
	ctAtom         atom;
} ctTypedAtom;


#endif // CUTE_ATOM_H