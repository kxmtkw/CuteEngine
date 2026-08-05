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


// -- Lifetime Methods --

// Initialize a buffer object.
CtBufferObject*
ct_lib_buffer_new(CtObjectManager* manager, uint32_t size);

// Delete method for the buffer object.
bool
ct_lib_buffer_del(CtObjectManager* manager, CtObject* obj);

// -- Data / Buffer Manipulation --

// Resize a buffer. Truncates if new size is less.
bool
ct_lib_buffer_resize(CtBufferObject* obj, uint32_t new_size);

// Get a byte at a specific index in the buffer
bool
ct_lib_buffer_get_byte(CtBufferObject* obj, uint32_t index, uint8_t* out);

// Set a byte at a specific index in the buffer.
bool
ct_lib_buffer_set_byte(CtBufferObject* obj, uint32_t index, uint8_t byte);

// Set n bytes starting from index. For internal use.
bool
ct_lib_buffer_set_bytes(CtBufferObject* obj, uint32_t index, uint32_t n, uint8_t* bytes);

// Set the buffer's data with another buffer starting from index. Fails if first buffer is too small
bool
ct_lib_buffer_set_buffer(CtBufferObject* obj, uint32_t index, CtBufferObject* other);

// Sets every byte in the buffer to a specific value.
bool
ct_lib_buffer_fill(CtBufferObject* obj, uint8_t byte);

// Clears the data for a buffer. Sets everything to zero.
bool
ct_lib_buffer_clear(CtBufferObject* obj);

// Extend a buffer with the contents of another. Impliclity resizes the first buffer so the 
// contents of the second buffer can fit.
bool
ct_lib_buffer_extend(CtBufferObject* obj, CtBufferObject* other);

// Get a slice from the buffer
CtBufferObject*
ct_lib_buffer_slice(CtBufferObject* obj, uint32_t index, uint32_t length);

// Create a copy of a buffer.
CtBufferObject*
ct_lib_buffer_copy(CtBufferObject* obj);

#endif // CT_LIB_BUFFER_H