#include "common/config.h"
#include "engine/context.h"
#include "stdlib.h"

#include "common/atom.h"
#include "common/error.h"

#include "objects/manager.h"
#include "objects/object.h"
#include <string.h>
#include <threads.h>
#include <time.h>

#include "lib/buffer.h"


CtBufferObject*
ct_lib_buffer_new(CtObjectManager* manager, uint32_t size) {

	CtBufferObject* buffer = (CtBufferObject*) ct_objects_new_object(manager, sizeof(CtBufferObject), 1, ct_lib_buffer_del);

	if (!buffer) {
		return NULL;
	}

	buffer->size = size;
	buffer->data = (uint8_t*) malloc(size);

	if (!buffer->data) {
		CT_ERROR_LIB(
			ct_thread_error, 
			"Buffer", 
			"Allocation", 
			"Failed to allocate data (%u bytes) for buffer.",
			size
		)
	}

	memset(buffer->data, 0, buffer->size);

	CT_LOG("lib/buffer", "Buffer [%p] of size %u bytes allocated.\n", buffer, size);
	
	return buffer;
}


bool
ct_lib_buffer_del(CtObjectManager* manager, CtObject* obj) {

	CtBufferObject* buffer = (CtBufferObject*) obj;
	free(buffer->data);
	buffer->data = NULL;
	buffer->size = 0;

	CT_LOG("lib/buffer", "Freed data for Buffer [%p]\n", buffer);

	return true;
}


CtBufferObject*
ct_lib_buffer_copy(CtObjectManager* manager, CtBufferObject* obj) {
	
	CtBufferObject* buffer_copy = ct_lib_buffer_new(manager, obj->size);
	
	if (!buffer_copy) {
		return NULL;
	}
	
	memcpy(buffer_copy->data, obj->data, obj->size);

	CT_LOG("lib/buffer", "Copied Buffer [%p] from Buffer [%p].\n", buffer_copy, obj);

	return buffer_copy;
}


bool
ct_lib_buffer_resize(CtBufferObject* obj, uint32_t new_size) {

	if (new_size == obj->size) {
		return true;
	}

	uint8_t* new_data = realloc(obj->data, new_size);

	if (!new_data) {
		CT_ERROR_LIB(
			ct_thread_error, 
			"Buffer", 
			"Allocation", 
			"Failed to resize data (%u bytes -> %u bytes) for buffer.",
			obj->size, new_size
		)
		return false;
	}

	CT_LOG("lib/buffer", "Resized data for Buffer [%p] %u bytes -> %u bytes.\n", obj, obj->size, new_size);

	obj->data = new_data;
	obj->size = new_size;

	return true;
}


bool
ct_lib_buffer_get_byte(CtBufferObject* obj, uint32_t index, uint8_t* out) {

	if (index > obj->size) {
		CT_ERROR_LIB(
			ct_thread_error, 
			"Buffer", 
			"OutOfRange",
			"Index %u falls out of range [0-%u]",
			index, obj->size-1
		)
		return false;
	}

	*out = obj->data[index];

	return true;
}


bool
ct_lib_buffer_set_byte(CtBufferObject* obj, uint32_t index, uint8_t byte) {

	if (index > obj->size) {
		CT_ERROR_LIB(
			ct_thread_error, 
			"Buffer", 
			"OutOfRange",
			"Index %u falls out of range [0-%u]",
			index, obj->size-1
		);
		return false;
	}

	obj->data[index] = byte;
	return true;
}


bool
ct_lib_buffer_set_bytes(CtBufferObject* obj, uint32_t index, uint32_t n, uint8_t* bytes) {

	if (index + n > obj->size) {
		CT_ERROR_LIB(
			ct_thread_error, 
			"Buffer", 
			"OutOfRange",
			"Range [%u-%u] falls out of range [0-%u]",
			index, index + n
		)
		return false;
	}

	memcpy(&obj->data[index], bytes, n);
	return true;
}


bool
ct_lib_buffer_extend(CtBufferObject* obj, CtBufferObject* other) {

	if (!ct_lib_buffer_resize(obj, obj->size + other->size)) {
		return false;
	};

	memcpy(&obj->data[obj->size], other->data, other->size);	

	CT_LOG("lib/buffer", "Extended Buffer [%p] with the data of Buffer [%p]\n", obj, other);

	return true;
}