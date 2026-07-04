

#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <stdint.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"
#include "containers/container.h"
#include "engine/error.h"


typedef struct {
	ctAtom         atoms[CUTE_CONF_SLOT_COUNT];
	ctAtomTypeSize types[CUTE_CONF_SLOT_COUNT];
} ctAtomFile;



typedef struct {
	uint32_t            procedure_id;
	uint32_t            container_field_count;
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


typedef struct {
	ctImage*            image;
	ctContainerManager* containers;
	uint64_t            ip;
	ctCallStack         callstack;
	ctCallFrame*        current_frame;
	double              cmp_diff;
	bool                running;
	bool                has_error;
	ctError             error;
	uint8_t            exit_code;
} ctContext;


// Create a new context. Requires the image to be ran and the starting procedure.
ctContext*
ct_ctx_new(ctImage* img, ctContainerManager* containers, uint32_t procedure_id);

// Free the context and its resources.
void
ct_ctx_del(ctContext* ctx);

// Setup a callframe and allocated local variables for a procedure.
void
ct_ctx_callProcedure(ctContext* ctx, uint32_t procedure_id, uint32_t arg_count, uint8_t arg_start_slot, uint8_t return_slot);

// Return from the last called procedure.
void
ct_ctx_returnProcedure(ctContext* ctx, ctAtom returned_atom, ctAtomType returned_atom_type);


static inline void
ct_ctx_storeAtom(ctContext* ctx, uint8_t slot, ctAtom atom, ctAtomType type) {

	if (ctx->current_frame->file.types[slot] == ctAtomType_Container) {
		ct_containers_decRef(ctx->containers, ctx->current_frame->file.atoms[slot].as_container);
		ctx->current_frame->container_field_count--;
	};

	ctx->current_frame->file.atoms[slot] = atom;
	ctx->current_frame->file.types[slot] = type;

	if (type == ctAtomType_Container) {
		ct_containers_incRef(ctx->containers, atom.as_container);
		ctx->current_frame->container_field_count++;
	};
};


static inline void
ct_ctx_loadAtom(ctContext* ctx, uint8_t slot, ctAtom* atom, ctAtomType* type) {
	*atom = ctx->current_frame->file.atoms[slot];
	*type = ctx->current_frame->file.types[slot];
};


static inline void
ct_ctx_moveAtom(ctContext* ctx, uint8_t src_slot, uint8_t dest_slot) {

	if (ctx->current_frame->file.types[dest_slot] == ctAtomType_Container) {
		ct_containers_decRef(ctx->containers, ctx->current_frame->file.atoms[dest_slot].as_container);
		ctx->current_frame->container_field_count--;
	};

	ctx->current_frame->file.atoms[dest_slot] = ctx->current_frame->file.atoms[src_slot];
	ctx->current_frame->file.types[dest_slot] = ctx->current_frame->file.types[src_slot];

	if (ctx->current_frame->file.types[src_slot] == ctAtomType_Container) {
		ct_containers_incRef(ctx->containers, ctx->current_frame->file.atoms[src_slot].as_container);
		ctx->current_frame->container_field_count++;
	};
};


// Throw an internal error
void
ct_ctx_throwError(ctContext* ctx, ctError error);

// Call a module method
void
ct_ctx_modcall(ctContext* ctx, uint32_t module_id, uint32_t method_id, uint32_t arg_count, uint8_t arg_start_slot, uint8_t return_slot);

#endif // ENGINE_CONTEXT_H