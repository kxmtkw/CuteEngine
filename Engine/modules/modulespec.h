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

#endif // ENGINE_MODULESPEC_H
