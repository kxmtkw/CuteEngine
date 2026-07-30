

#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <stdint.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"

#include "object/object.h"
#include "error/error.h"


// Array of atoms and their types. For use in call frames
typedef struct {
	CtAtom         atoms[CT_CONF_SLOT_COUNT];
	CtAtomTypeSize types[CT_CONF_SLOT_COUNT];
} CtAtomFile;


// a call frame
typedef struct {
	uint32_t            procedure_id;
	uint64_t            return_ip;
	uint32_t            object_field_count;
	uint8_t             args_count;
	uint8_t             return_value_slot;
	CtAtomFile          file;
} ctCallFrame;


// call stack, statically sized
typedef struct {
	CtAtomFile   file;
	ctCallFrame  frames[CT_CONF_CALLSTACK_SIZE];
	uint32_t     size;
} ctCallStack;


// context specifies the state of execution. 
struct CtContext {
	const CtImage*      image;
	CtObjectManager*    objects;
	uint64_t            ip;
	ctCallStack         callstack;
	ctCallFrame*        current_frame;
	double              cmp_diff;
	bool                running;
	ctError             error;
	uint8_t             exit_code;
};

typedef struct CtContext CtContext;

// Create a new context. Requires the image to be ran and the starting procedure.
CtContext*
ct_ctx_new(CtImage* img, CtObjectManager* objects, uint32_t procedure_id);

// Free the context and its resources.
void
ct_ctx_del(CtContext* ctx);

// Setup a callframe, copy the specified arguments from the caller's frame and return value to the specified return slot.
void
ct_ctx_call_procedure(CtContext* ctx, uint32_t procedure_id, uint8_t arg_start_slot, uint8_t return_slot);

// Return from the last called procedure.
void
ct_ctx_return_procedure(CtContext* ctx, CtAtom returned_atom, CtAtomType returned_atom_type);

// Store an atom at a specified slot in the CURRENT call frame.
static inline void
ct_ctx_store_atom(CtContext* ctx, uint8_t slot, CtAtom atom, CtAtomType type) {

	if (ctx->current_frame->file.types[slot] == CT_ATOM_OBJECT) {
		ct_objects_dec_ref(ctx->objects, ctx->current_frame->file.atoms[slot].as_object);
		ctx->current_frame->object_field_count--;
	};

	ctx->current_frame->file.atoms[slot] = atom;
	ctx->current_frame->file.types[slot] = type;

	if (type == CT_ATOM_OBJECT) {
		ct_objects_inc_ref(ctx->objects, atom.as_object);
		ctx->current_frame->object_field_count++;
	};
};

// Load an atom from a specified index from the CURRENT call frame.
static inline void
ct_ctx_load_atom(CtContext* ctx, uint8_t slot, CtAtom* atom, CtAtomType* type) {
	*atom = ctx->current_frame->file.atoms[slot];
	*type = ctx->current_frame->file.types[slot];
};

// Move an atom from one slot to another.
static inline void
ct_ctx_move_atom(CtContext* ctx, uint8_t src_slot, uint8_t dest_slot) {

	if (ctx->current_frame->file.types[dest_slot] == CT_ATOM_OBJECT) {
		ct_objects_dec_ref(ctx->objects, ctx->current_frame->file.atoms[dest_slot].as_object);
		ctx->current_frame->object_field_count--;
	};

	ctx->current_frame->file.atoms[dest_slot] = ctx->current_frame->file.atoms[src_slot];
	ctx->current_frame->file.types[dest_slot] = ctx->current_frame->file.types[src_slot];

	if (ctx->current_frame->file.types[src_slot] == CT_ATOM_OBJECT) {
		ct_objects_inc_ref(ctx->objects, ctx->current_frame->file.atoms[src_slot].as_object);
		ctx->current_frame->object_field_count++;
	};
};

// Throw an internal error
void
ct_ctx_throw_error(CtContext* ctx, ctError error);

// Call a module method
void
ct_ctx_modcall(CtContext* ctx, uint32_t module_id, uint32_t method_id, uint32_t arg_count, uint8_t arg_start_slot, uint8_t return_slot);

#endif // ENGINE_CONTEXT_H