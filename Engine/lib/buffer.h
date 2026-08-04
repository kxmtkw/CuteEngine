#ifndef CT_LIB_BUFFER_H
#define CT_LIB_BUFFER_H

#include "common/atom.h"
#include "common/error.h"

#include "objects/object.h"


typedef struct {
	CtObject __object__;
	uint8_t* data;
	uint32_t size;
} CtBufferObject;


// Initialize a buffer object.
CtBufferObject*
ct_lib_buffer_new(CtObjectManager* manager, uint32_t size);

// Delete method for the buffer object.
void
ct_lib_buffer_del(CtObjectManager* manager, CtObject* obj);

// Resize a buffer. Truncates if new size is less.
void
ct_lib_buffer_resize(CtBufferObject* obj, uint32_t new_size);

// Get a byte at a specific index in the buffer
void
ct_lib_buffer_get_byte(CtBufferObject* obj, uint32_t index, uint8_t* outbyte);

// Set a byte at a specific index in the buffer.
void
ct_lib_buffer_set_byte(CtBufferObject* obj, uint32_t index, uint8_t byte);

// Set n bytes starting from index
void
ct_lib_buffer_set_bytes(CtBufferObject* obj, uint32_t index, uint32_t n, uint8_t* bytes);


#endif // CT_LIB_BUFFER_H