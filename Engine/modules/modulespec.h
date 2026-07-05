#ifndef ENGINE_MODULESPEC_H
#define ENGINE_MODULESPEC_H

#include "CuteAtom.h"
#include "containers/container.h"
#include "engine/error.h"
#include <stdint.h>
#include <string.h>


typedef struct {
	const ctAtom*     atoms;
	const ctAtomTypeSize* types;
	const uint32_t    count;
	ctContainerManager* container_manager;
} ctModuleArguments;


typedef struct {
	ctAtom         returned_atom;
	ctAtomTypeSize returned_atom_type;
	bool           success;
	ctError        error;
} ctModuleResult;

typedef ctModuleResult (*ctModuleMethod)(ctModuleArguments);

#endif // ENGINE_MODULESPEC_H
