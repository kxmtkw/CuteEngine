#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"
#include "object/object.h"
#include "error/error.h"
#include "modules/modules.h"
#include "modules/modulespec.h"
#include "utils/utils.h"

#include "context.h"




// Call Stack helpers

static inline void
ct_ctx_initStack(ctCallStack* s) {
    s->size     = 0;
    s->capacity = CUTE_CONF_CALLSTACK_SIZE;
}

static inline void 
ct_ctx_delStack(ctCallStack* s) {
    s->size    = 0;
    s->capacity = 0;
}

static inline ctCallFrame*
ct_ctx_getFrame(ctCallStack* s) {		
    return &s->frames[s->size++];
}

static inline ctCallFrame* 
ct_ctx_popFrame(ctCallStack* s) {
    return &s->frames[--s->size];
}

static inline ctCallFrame* 
ct_ctx_peekFrame(ctCallStack* s) {
    return &s->frames[s->size-1];
}


// Context methods

ctContext*
ct_ctx_new(ctImage* img, ctObjectManager* objects, uint32_t procedure_id) {
	ctContext* ctx = (ctContext*) malloc(sizeof(ctContext));
	ctx->image = img;
	ctx->objects = objects;
	ctx->running = true;
	ctx->current_frame = NULL;
	ct_ctx_initStack(&ctx->callstack);
	ct_ctx_callProcedure(ctx, procedure_id, 0, 0);
	return ctx;
}


void
ct_ctx_del(ctContext* ctx) {
	ct_ctx_delStack(&ctx->callstack);
}


void
ct_ctx_callProcedure(ctContext* ctx, uint32_t procedure_id, uint8_t arg_start_slot, uint8_t return_slot) {

	if (ctx->callstack.size >= ctx->callstack.capacity) {
		
		CUTE_ERROR(
			(&ctx->error), 
			ctErrorCode_Recursion, 
			"Recursion depth reached. (%u calls)", ctx->callstack.capacity
		);

		ct_ctx_throwError(
			ctx, 
			ctx->error
		);
		return;
	};

	if (procedure_id >= ctx->image->header.procedure_count) {

		CUTE_ERROR(
			(&ctx->error), 
			ctErrorCode_Procedure, 
			"Invalid procedure ID '%u' called. [%u-%u]", procedure_id, 0, ctx->image->header.procedure_count-1
		);

		ct_ctx_throwError(
			ctx, 
			ctx->error
		);
		return;
	}

	ctImageProcedure proc = ctx->image->procedure_table[procedure_id];
	uint32_t arg_count = procedure_id == 0 ? 0 : proc.arg_count;

	if (arg_count >= CUTE_CONF_SLOT_COUNT) {
		
		CUTE_ERROR(
			(&ctx->error), 
			ctErrorCode_Procedure, 
			"Too many arguments requested by procedure(%u): '%u' (>=%u)", procedure_id, arg_count, CUTE_CONF_SLOT_COUNT
		);

		ctx->error = (ctError){.code=ctErrorCode_Procedure};
		ct_ctx_throwError(ctx, ctx->error);
		return;
	};

	ctCallFrame* frame = ct_ctx_getFrame(&ctx->callstack);

	frame->procedure_id = procedure_id;
	frame->object_field_count = 0;
	frame->return_ip = ctx->ip;
	frame->return_value_slot = return_slot;
	frame->args_count = arg_count;
	memset(frame->file.types, 0, CUTE_CONF_SLOT_COUNT);

	for (size_t i = 0; i < arg_count; i++) {
		frame->file.atoms[i] = ctx->current_frame->file.atoms[arg_start_slot + i];
		frame->file.types[i] = ctx->current_frame->file.types[arg_start_slot + i];

		if (frame->file.types[i] == ctAtomType_Object) {
			ct_objects_incRef(ctx->objects, frame->file.atoms[i].as_object);
			frame->object_field_count++;
		}
	};
	
	

	ctx->ip = proc.bytecode_index;
	ctx->current_frame = ct_ctx_peekFrame(&ctx->callstack);

	CUTE_LOG(
		"context", 
		"Called procedure(%u) with %u arguments passed from previous frame's slot %d\n", 
		procedure_id, arg_count, arg_start_slot
	);
}


void
ct_ctx_returnProcedure(ctContext* ctx, ctAtom returned_atom, ctAtomType returned_atom_type) {

	ctCallFrame* frame = ct_ctx_popFrame(&ctx->callstack);
	ctx->current_frame = ct_ctx_peekFrame(&ctx->callstack);
	
	ctx->ip = frame->return_ip;
	ct_ctx_storeAtom(ctx, frame->return_value_slot, returned_atom, returned_atom_type);

	for (size_t i = 0; i < CUTE_CONF_SLOT_COUNT && frame->object_field_count; i++) {
		if (frame->file.types[i] == ctAtomType_Object) {
			ct_objects_decRef(ctx->objects, frame->file.atoms[i].as_object);
			frame->object_field_count--;
		}
	};

	CUTE_LOG("context", "Returned from procedure(%u) with return value: 0x%lx\n", frame->procedure_id, returned_atom.raw);
}


inline void
ct_ctx_throwError(ctContext* ctx, ctError error) {
	ctx->running = false;
	ctx->error = error;
}


void
ct_ctx_modcall(ctContext* ctx, uint32_t module_id, uint32_t method_id, uint32_t arg_count, uint8_t arg_start_slot, uint8_t return_slot) {

	ctModuleMethod method;

	ctModuleDispatchCode code = ct_modules_getMethod(module_id, method_id, &method);

	if (code != ctModuleDispatchCode_Success) {
		CUTE_ERROR((&ctx->error), ctErrorCode_Module, "Can not access container slot #%u (>= %u)", "Unknown module method: %u.%u", module_id, method_id);
		ct_ctx_throwError(ctx, ctx->error);
		return;
	};

	CUTE_LOG("context", "Calling module method: %u.%u with %u arguments starting from slot %u. Returning to slot %u.\n", module_id, method_id, arg_count, arg_start_slot, return_slot);

	ctModuleArguments args = {
		.atoms = &ctx->current_frame->file.atoms[arg_start_slot],
		.types = &ctx->current_frame->file.types[arg_start_slot],
		.container_manager = ctx->objects,
		.count = arg_count
	};

	ctModuleResult result = method(args);

	if (!result.success) {
		ct_ctx_throwError(ctx, result.error);
		return;
	};

	ctx->current_frame->file.atoms[return_slot] = result.returned_atom;
	ctx->current_frame->file.types[return_slot] = result.returned_atom_type;
};