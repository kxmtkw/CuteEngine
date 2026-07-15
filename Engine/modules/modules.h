
#ifndef ENGINE_MODULES_H
#define ENGINE_MODULES_H

#include "CuteAtom.h"
#include "CuteModules.h"

#include <stdint.h>
#include <string.h>
#include "error/error.h"
#include "modules/modulespec.h"

#include "utils/utils.h"



/*
Module registration for the Cute Engine.
Each module must define these macros:
	CUTE_MODULE_****_METHODS
	CUTE_MODULE_****_METHODS_COUNT

How to use the dispatch map:
	map[index] -> module_map: gives you the dispatch table for the module at index.
	module_map maybe NULL if the module is not included in the build.
	module_map[index] gives you a ctModuleMethod which can be called. These can never be NULL.
*/


static const ctModuleMethod* ct_modules_dispatch_map[] = {
};

static const uint32_t ct_modules_method_count[] = {
};

static const uint32_t ct_modules_count = sizeof(ct_modules_dispatch_map) / sizeof(ct_modules_dispatch_map[0]);


typedef enum {
	ctModuleDispatchCode_Success = 0,
	ctModuleDispatchCode_ModuleNotFound = 1,
	ctModuleDispatchCode_MethodNotFound = 2
} ctModuleDispatchCode;


static inline ctModuleDispatchCode
ct_modules_getMethod(uint32_t module_id, uint32_t method_id, ctModuleMethod* method) {

	if (module_id >= ct_modules_count) {
		return ctModuleDispatchCode_ModuleNotFound;
	}

	const ctModuleMethod* module_map = ct_modules_dispatch_map[module_id];

	if (module_map == NULL) {
		return ctModuleDispatchCode_ModuleNotFound;
	};


	if (method_id >= ct_modules_method_count[module_id]) {
		return ctModuleDispatchCode_MethodNotFound;
	};

	*method = module_map[method_id];
	return ctModuleDispatchCode_Success;
};

#endif // ENGINE_MODULES_H
