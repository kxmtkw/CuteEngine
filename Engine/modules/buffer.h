
#ifndef ENGINE_MODULES_BUFFER_H
#define ENGINE_MODULES_BUFFER_H

#include "CuteModules.h"
#include "modulespec.h"

#ifdef CUTE_BUILTIN_MODULE_BUFFER

ctModuleResult ct_mbuffer_newBuffer(ctModuleArguments args);


static const ctModuleMethod ct_mbuffer_dispatchMap[] = {
	ct_mbuffer_newBuffer
};

#define MODULE_BUFFER_METHODS ct_mbuffer_dispatchMap
#define MODULE_BUFFER_METHODS_COUNT (sizeof(ct_mbuffer_dispatchMap) / sizeof(ct_mbuffer_dispatchMap[0]))

#else

#define MODULE_BUFFER_METHODS NULL
#define MODULE_BUFFER_METHODS_COUNT 0

#endif // CUTE_BUILTIN_MODULE_BUFFER

#endif // ENGINE_MODULES_BUFFER_H
