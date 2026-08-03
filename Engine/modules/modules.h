
#ifndef ENGINE_MODULES_H
#define ENGINE_MODULES_H

#include "modules/modulespec.h"




/*
Module registration for the Cute Engine.
Each module must define these macros:
	CUTE_MODULE_****_METHODS
	CUTE_MODULE_****_METHODS_COUNT

How to use the dispatch map:
	map[index] -> module_map: gives you the dispatch table for the module at index.
	module_map maybe NULL if the module is not included in the build.
	module_map[index] gives you a CtModuleMethod which can be called. These can never be NULL.
*/


static const CtModuleMethod* ct_modules_dispatch_map[] = {
};

static const uint32_t ct_modules_method_count[] = {
};

static const uint32_t ct_modules_count = sizeof(ct_modules_dispatch_map) / sizeof(ct_modules_dispatch_map[0]);



static inline int32_t
ct_modules_get_method(uint32_t module_id, uint32_t method_id, CtModuleMethod* method) {

	if (module_id >= ct_modules_count) {
		return 0;
	}

	const CtModuleMethod* module_map = ct_modules_dispatch_map[module_id];

	if (module_map == NULL) {
		return 0;
	};

	if (method_id >= ct_modules_method_count[module_id]) {
		return 0;
	};

	*method = module_map[method_id];
	
	return 1;
};

#endif // ENGINE_MODULES_H
