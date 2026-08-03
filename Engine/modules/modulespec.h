#ifndef ENGINE_MODULESPEC_H
#define ENGINE_MODULESPEC_H

#include <stdint.h>

#include "common/atom.h"

#include "engine/context.h"


typedef struct {
	const CtContext* const       context;
	const CtAtom* const          argument_atoms;
	const CtAtomTypeSize* const  argument_types;
	const uint32_t               argument_count;
} CtModuleMethodArguments;


typedef struct {
	CtAtom         returned_atom;
	CtAtomTypeSize returned_type;
	bool           success;
} CtModuleMethodResult;

typedef void (*CtModuleMethod)(CtModuleMethodArguments, CtModuleMethodResult*);

typedef struct {
	CtModuleMethod method;
	uint32_t       argument_count;
} CtModuleMethodEntry;

#define CT_MODULE_ENTRY(METHOD, ARG_COUNT) (CtModuleMethodEntry) {METHOD, ARG_COUNT}
#define CT_MODULE_DEFINE(NAME) CtModuleMethodEntry _ct_module_ ## NAME ## _method_map[]
#define CT_MODULE_REFER(NAME) _ct_module_ ## NAME ## _method_map
#define CT_MODULE_COUNT_METHODS(NAME) sizeof(CT_MODULE_REFER(NAME)) / sizeof(CT_MODULE_REFER(NAME)[0]) 

#endif // ENGINE_MODULESPEC_H
