
#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <stdint.h>
#include <stdio.h>

#include "CuteEngine.h"


struct _ctEngine {
	ctImage*            image;
	uint8_t             exit_code;
};

typedef struct _ctEngine ctEngine;

// Executes a context, the heart of the engine.
void
ct_exec(ctContext* ctx);

#endif // ENGINE_ENGINE_H