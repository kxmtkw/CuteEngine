#ifndef CT_MODULE_BUFFER_H
#define CT_MODULE_BUFFER_H

#include "common/atom.h"
#include "engine/context.h"
#include "modules/modulespec.h"

#include "lib/buffer.h"

static void
ct_module_buffer_new(CtModuleMethodArguments args, CtModuleMethodResult* result) {

	uint32_t size = args.argument_atoms[0].as_uint;

	CtBufferObject* buffer;

	result->success = buffer = ct_lib_buffer_new(ct_ctx_get_object_manager(args.context), size);
	
	result->returned_atom.as_object = (CtObject*) buffer;
	result->returned_type = CT_ATOM_OBJECT;
}


static void
ct_module_buffer_resize(CtModuleMethodArguments args, CtModuleMethodResult* result) {
	
	CtBufferObject* buffer = (CtBufferObject*) args.argument_atoms[0].as_object;
	uint32_t size = args.argument_atoms[1].as_uint;

	result->success = ct_lib_buffer_resize(buffer, size);

	result->returned_atom.as_uint = buffer->size;
	result->returned_type = CT_ATOM_PRIMITIVE;
}


static void
ct_module_buffer_copy(CtModuleMethodArguments args, CtModuleMethodResult* result) {

	CtBufferObject* buffer = (CtBufferObject*) args.argument_atoms[0].as_object;

	CtBufferObject* copy;
	result->success = copy = ct_lib_buffer_copy(ct_ctx_get_object_manager(args.context), buffer);

	result->returned_atom.as_object = (CtObject*) copy;
	result->returned_type = CT_ATOM_OBJECT;
}


static void
ct_module_buffer_get_byte(CtModuleMethodArguments args, CtModuleMethodResult* result) {

	CtBufferObject* buffer = (CtBufferObject*) args.argument_atoms[0].as_object;
	uint32_t index = args.argument_atoms[1].as_uint;

	uint8_t byte;
	result->success = ct_lib_buffer_get_byte(buffer, index, &byte);

	result->returned_atom.as_uint = (uint32_t) byte;
	result->returned_type = CT_ATOM_PRIMITIVE;
}


static void
ct_module_buffer_set_byte(CtModuleMethodArguments args, CtModuleMethodResult* result) {

	CtBufferObject* buffer = (CtBufferObject*) args.argument_atoms[0].as_object;
	uint32_t index = args.argument_atoms[1].as_uint;
	uint8_t byte = (uint8_t) args.argument_atoms[2].as_uint;

	result->success = ct_lib_buffer_set_byte(buffer, index, byte);

	result->returned_atom.as_uint = (uint32_t) byte;
	result->returned_type = CT_ATOM_PRIMITIVE;
}


static void
ct_module_buffer_extend(CtModuleMethodArguments args, CtModuleMethodResult* result) {
	
	CtBufferObject* buffer = (CtBufferObject*) args.argument_atoms[0].as_object;
	CtBufferObject* other_buffer = (CtBufferObject*) args.argument_atoms[1].as_object;

	result->success = ct_lib_buffer_extend(buffer, other_buffer);

	result->returned_atom.as_uint = (uint32_t) 0;
	result->returned_type = CT_ATOM_PRIMITIVE;
}



CT_MODULE_DEFINE(buffer) = {
	CT_MODULE_ENTRY(ct_module_buffer_new, 1),
	CT_MODULE_ENTRY(ct_module_buffer_resize, 2),
	CT_MODULE_ENTRY(ct_module_buffer_copy, 1),
	CT_MODULE_ENTRY(ct_module_buffer_get_byte, 2),
	CT_MODULE_ENTRY(ct_module_buffer_set_byte, 3),
	CT_MODULE_ENTRY(ct_module_buffer_extend, 2),
};



#endif // CT_MODULE_BUFFER_H