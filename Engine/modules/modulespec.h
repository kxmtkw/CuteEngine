#ifndef ENGINE_MODULESPEC_H
#define ENGINE_MODULESPEC_H

#include <stdint.h>

#include "common/atom.h"

#include "engine/context.h"


typedef struct {
	CtContext*                   context;
	const CtAtom* const          argument_atoms;
	const CtAtomTypeSize* const  argument_types;
} CtModuleMethodArguments;


typedef struct {
	CtAtom         returned_atom;
	CtAtomTypeSize returned_type;
	bool           success;
} CtModuleMethodResult;

typedef void (*CtModuleMethod)(CtModuleMethodArguments, CtModuleMethodResult*);

typedef struct {
	char*          name;
	CtModuleMethod method;
	uint32_t       argument_count;
} CtModuleMethodEntry;

#define CT_MODULE_NAME_DEFINE(NAME, STRING) static const char* _ct_module_ ## NAME ## _name = STRING;
#define CT_MODULE_METHOD_MAP_DEFINE(NAME) CtModuleMethodEntry _ct_module_ ## NAME ## _method_map[]
#define CT_MODULE_METHOD_MAP(NAME) _ct_module_ ## NAME ## _method_map
#define CT_MODULE_ENTRY(NAME, METHOD, ARG_COUNT) (CtModuleMethodEntry) {NAME, METHOD, ARG_COUNT}
#define CT_MODULE_COUNT_METHODS(NAME) sizeof(CT_MODULE_METHOD_MAP(NAME)) / sizeof(CT_MODULE_METHOD_MAP(NAME)[0]) 

#endif // ENGINE_MODULESPEC_H
