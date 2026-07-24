
#ifndef CUTE_ENGINE_H
#define CUTE_ENGINE_H

#include <stdint.h>
#include "CuteAtom.h"
#include "CuteInstr.h"



struct _ctEngine;
typedef struct _ctEngine ctEngine;

struct _ctContext;
typedef struct _ctContext ctContext;

// Intialize the engine
ctEngine*
ct_engine_init();

// End the engine and free all resources
void
ct_engine_end(ctEngine* engine);

// Load an image file. For now, only one image can be loaded.
void
ct_engine_load(ctEngine* engine, const char* filepath);

// Make a new context
ctContext*
ct_engine_newCtx(ctEngine* engine, uint32_t procedure_id);

// Delete a context
void
ct_engine_delCtx(ctEngine* engine, ctContext* ctx);

// Run the engine with a custom context
void
ct_engine_runCtx(ctEngine* engine, ctContext* ctx);

// Run the engine with default context parameters
void
ct_engine_run(ctEngine* engine);


#endif // CUTE_ENGINE_H