#ifndef ENGINE_MODULESPEC_H
#define ENGINE_MODULESPEC_H

#include <stdint.h>

#include "common/atom.h"
#include "objects/manager.h"
#include "common/error.h"



typedef struct {
	const CtAtom*     atoms;
	const CtAtomTypeSize* types;
	const uint32_t    count;
	CtObjectManager* container_manager;
} ctModuleArguments;


typedef struct {
	CtAtom         returned_atom;
	CtAtomTypeSize returned_atom_type;
	bool           success;
	ctError        error;
} ctModuleResult;

typedef ctModuleResult (*ctModuleMethod)(ctModuleArguments);

#endif // ENGINE_MODULESPEC_H
