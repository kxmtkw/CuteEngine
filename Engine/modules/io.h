
#ifndef ENGINE_MODULES_IO_H
#define ENGINE_MODULES_IO_H



#ifdef CUTE_BUILTIN_MODULE_IO

#include "CuteAtom.h"
#include "CuteModules.h"
#include "modulespec.h"


/*
Print a whatever is given.

Arg (1) atom : Any

Returns: none
*/
ctModuleResult 
ct_mio_print(ctModuleArguments args);

static const ctModuleMethod ct_mio_dispatchMap[] = {
	ct_mio_print
};


#define CUTE_MODULE_IO_METHODS ct_mio_dispatchMap
#define CUTE_MODULE_IO_METHODS_COUNT (sizeof(ct_mio_dispatchMap) / sizeof(ct_mio_dispatchMap[0]))

#else

#define CUTE_MODULE_IO_METHODS NULL
#define CUTE_MODULE_IO_METHODS_COUNT 0

#endif // CUTE_BUILTIN_MODULE_IO

#endif // ENGINE_MODULES_IO_H
