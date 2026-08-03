#ifndef CT_LIB_BUFFER_H
#define CT_LIB_BUFFER_H

#include "common/atom.h"
#include "common/error.h"

#include "objects/object.h"


typedef struct {
	CtObject object;
	uint8_t* data;
	uint32_t capacity;
	uint32_t size;
} CtBuffer;


// Initialize a buffer object.
void
ct_lib_buffer_init(CtBuffer* obj, uint32_t cap);

// Delete method for the buffer object.
void
ct_lib_buffer_del(CtObject* obj);

// Resize a buffer. Truncates if new cap < old cap.
void
ct_lib_buffer_resize(CtBuffer* obj, uint32_t new_cap);

// Get a byte at a specific index in the buffer
uint8_t
ct_lib_buffer_get_byte(CtBuffer* obj, uint32_t index);

// Set a byte at a specific index in the buffer.
void
ct_lib_buffer_set_byte(CtBuffer* obj, uint32_t index, uint8_t byte);

// Set n bytes starting from index
void
ct_lib_buffer_set_bytes(CtBuffer* obj, uint32_t index, uint32_t n, uint8_t* bytes);

// Push a byte to the end of the buffer.
void
ct_lib_buffer_push_byte(CtBuffer* obj, uint8_t byte);

// Push n bytes to the end of the buffer.
void
ct_lib_buffer_push_bytes(CtBuffer* obj, uint32_t count, uint8_t* bytes);

// Extend a buffer with another buffer's contents. The other buffer remains unchanged.
void
ct_lib_buffer_extend(CtBuffer* obj, CtBuffer* other);


#endif // CT_LIB_BUFFER_H