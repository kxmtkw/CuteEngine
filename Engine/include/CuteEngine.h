
#ifndef CUTE_ENGINE_H
#define CUTE_ENGINE_H

#include <stdint.h>
#include "CuteAtom.h"
#include "CuteInstr.h"



struct _ctEngine;
typedef struct _ctEngine ctEngine;


// Intialize the engine
ctEngine*
ct_engine_init();

// End the engine and free all resources
void
ct_engine_end(ctEngine* engine);

// Load an image file. For now, only one image can be loaded.
void
ct_engine_loadFile(ctEngine* engine, const char* filepath);

// Run the engine with the loaded image file
void
ct_engine_run(ctEngine* engine);


#endif // CUTE_ENGINE_H