#ifndef CUTE_runtime_H
#define CUTE_runtime_H

#include <stdint.h>

#include "CuteInstr.h"

#include "common/error.h"
#include "context.h"


typedef struct {
	CtImage             image;
	CtError             error;
	uint8_t             exit_code;
} CtRuntime;


// Intialize the runtime
void
ct_runtime_init(CtRuntime* runtime);

// End the runtime and free all resources
void
ct_runtime_end(CtRuntime* runtime);

// Load an image file. For now, only one image can be loaded.
void
ct_runtime_load(CtRuntime* runtime, const char* filepath);

// Run the runtime with a custom context
void
ct_runtime_run_context(CtRuntime* runtime, CtContext* ctx);

// Executes a context, the heart of the runtime.
void
ct_runtime_exec(CtRuntime* runtime, CtContext* ctx);


#endif // CUTE_runtime_H