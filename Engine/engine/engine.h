#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <stdint.h>

#include "CuteInstr.h"

#include "context.h"


struct CtEngine {
	CtImage*            image;
	uint8_t             exit_code;
};

typedef struct CtEngine CtEngine;

// Intialize the engine
CtEngine*
ct_engine_init(void);

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


#endif // ENGINE_ENGINE_H