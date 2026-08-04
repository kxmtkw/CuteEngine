#ifndef CUTE_ENGINE_H
#define CUTE_ENGINE_H

#include <stdint.h>

#include "CuteInstr.h"

#include "common/error.h"
#include "context.h"


typedef struct {
	CtImage             image;
	CtError             error;
	uint8_t             exit_code;
} CtEngine;


// Intialize the engine
void
ct_engine_init(CtEngine* engine);

// End the engine and free all resources
void
ct_engine_end(CtEngine* engine);

// Load an image file. For now, only one image can be loaded.
void
ct_engine_load(CtEngine* engine, const char* filepath);

// Run the engine with a custom context
void
ct_engine_run_context(CtEngine* engine, CtContext* ctx);

// Executes a context, the heart of the engine.
void
ct_engine_exec(CtEngine* engine, CtContext* ctx);


#endif // CUTE_ENGINE_H