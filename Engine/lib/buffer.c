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

	buffer->size = size;
	buffer->data = (uint8_t*) malloc(size);

	return buffer;
}


void
ct_lib_buffer_del(CtObjectManager* manager, CtObject* obj) {

	CtBufferObject* buffer = (CtBufferObject*) obj;
	free(buffer->data);
	buffer->data = NULL;
	buffer->size = 0;

}


void
ct_lib_buffer_resize(CtBufferObject* obj, uint32_t new_size) {

	if (new_size == obj->size) {
		return;
	}

	uint8_t* new_data = realloc(obj->data, new_size);
	obj->data = new_data;
	obj->size = new_size;
}


uint8_t
ct_lib_buffer_get_byte(CtBufferObject* obj, uint32_t index) {

	if (index > obj->size) {
		// Fail
		exit(6);
	}

	return obj->data[index];
}


void
ct_lib_buffer_set_byte(CtBufferObject* obj, uint32_t index, uint8_t byte) {

	if (index > obj->size) {
		// Fail
		exit(6);
	}

	obj->data[index] = byte;
}


void
ct_lib_buffer_set_bytes(CtBufferObject* obj, uint32_t index, uint32_t n, uint8_t* bytes) {

	if (index + n > obj->size) {
		// Fail
		exit(6);
	}

	memcpy(&obj->data[index], bytes, n);
}