

#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <stdint.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteEngine.h"
#include "CuteInstr.h"
#include "object/object.h"
#include "error/error.h"


typedef struct {
	ctAtom         atoms[CUTE_CONF_SLOT_COUNT];
	ctAtomTypeSize types[CUTE_CONF_SLOT_COUNT];
} ctAtomFile;


typedef struct {
	uint32_t            procedure_id;
	uint32_t            object_field_count;
	uint64_t            return_ip;
	uint8_t             args_count;
	uint8_t             return_value_slot;
	ctAtomFile          file;
} ctCallFrame;


typedef struct {
	ctAtomFile   file;
	ctCallFrame  frames[CUTE_CONF_CALLSTACK_SIZE];
	uint32_t     size;
	uint32_t     capacity;
} ctCallStack;


struct _ctContext {
	ctImage*            image;
	ctObjectManager*    objects;
	uint64_t            ip;
	ctCallStack         callstack;
	ctCallFrame*        current_frame;
	double              cmp_diff;
	bool                running;
	ctError             error;
	uint8_t             exit_code;
};

typedef struct _ctContext ctContext;

// Create a new context. Requires the image to be ran and the starting procedure.
ctContext*
ct_ctx_new(ctImage* img, ctObjectManager* objects, uint32_t procedure_id);

// Free the context and its resources.
void
ct_ctx_del(ctContext* ctx);

// Setup a callframe, copy the specified arguments from the caller's frame and return value to the specified return slot.
void
ct_ctx_callProcedure(ctContext* ctx, uint32_t procedure_id, uint8_t arg_start_slot, uint8_t return_slot);

// Return from the last called procedure.
void
ct_ctx_returnProcedure(ctContext* ctx, ctAtom returned_atom, ctAtomType returned_atom_type);

// Store an atom at a specified slot in the CURRENT call frame.
static inline void
ct_ctx_storeAtom(ctContext* ctx, uint8_t slot, ctAtom atom, ctAtomType type) {

	if (ctx->current_frame->file.types[slot] == ctAtomType_Object) {
		ct_objects_decRef(ctx->objects, ctx->current_frame->file.atoms[slot].as_object);
		ctx->current_frame->object_field_count--;
	};

	ctx->current_frame->file.atoms[slot] = atom;
	ctx->current_frame->file.types[slot] = type;

	if (type == ctAtomType_Object) {
		ct_objects_incRef(ctx->objects, atom.as_object);
		ctx->current_frame->object_field_count++;
	};
};

// Load an atom from a specified index from the CURRENT call frame.
static inline void
ct_ctx_loadAtom(ctContext* ctx, uint8_t slot, ctAtom* atom, ctAtomType* type) {
	*atom = ctx->current_frame->file.atoms[slot];
	*type = ctx->current_frame->file.types[slot];
};

// Move an atom from one slot to another.
static inline void
ct_ctx_moveAtom(ctContext* ctx, uint8_t src_slot, uint8_t dest_slot) {

	if (ctx->current_frame->file.types[dest_slot] == ctAtomType_Object) {
		ct_objects_decRef(ctx->objects, ctx->current_frame->file.atoms[dest_slot].as_object);
		ctx->current_frame->object_field_count--;
	};

	ctx->current_frame->file.atoms[dest_slot] = ctx->current_frame->file.atoms[src_slot];
	ctx->current_frame->file.types[dest_slot] = ctx->current_frame->file.types[src_slot];

	if (ctx->current_frame->file.types[src_slot] == ctAtomType_Object) {
		ct_objects_incRef(ctx->objects, ctx->current_frame->file.atoms[src_slot].as_object);
		ctx->current_frame->object_field_count++;
	};
};


// Throw an internal error
void
ct_ctx_throwError(ctContext* ctx, ctError error);

// Call a module method
void
ct_ctx_modcall(ctContext* ctx, uint32_t module_id, uint32_t method_id, uint32_t arg_count, uint8_t arg_start_slot, uint8_t return_slot);

#endif // ENGINE_CONTEXT_H