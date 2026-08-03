#ifndef CT_MODULE_BUFFER_H
#define CT_MODULE_BUFFER_H

#include "common/atom.h"
#include "modules/modulespec.h"

#include "lib/buffer.h"

static void
ct_module_buffer_new(CtModuleMethodArguments args, CtModuleMethodResult* result) {

	uint32_t size = args.argument_atoms[0].as_uint;
	CtBufferObject* buffer = ct_lib_buffer_new(args.context->objects, size);
	
	result->returned_atom.as_object = (CtObject*) buffer;
	result->returned_type = CT_ATOM_PRIMITIVE;
}

CT_MODULE_DEFINE(buffer) = {
	CT_MODULE_ENTRY(ct_module_buffer_new, 1),
};



#endif // CT_MODULE_BUFFER_H