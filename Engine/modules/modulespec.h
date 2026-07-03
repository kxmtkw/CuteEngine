#ifndef ENGINE_MODULESPEC_H
#define ENGINE_MODULESPEC_H

#include "CuteAtom.h"
#include <stdint.h>
#include <string.h>


typedef struct {
	ctAtom*     atom;
	ctAtomType* type;
	uint32_t    count;
} ctModuleArguments;


typedef struct {
	ctAtom     returned_atom;
	ctAtomType returned_atom_type;
	bool       success;
	char*	   error_message;
} ctModuleResult;

typedef ctModuleResult (*ctModuleMethodDispatcher)(uint32_t, ctModuleArguments);
typedef ctModuleResult (*ctModuleMethod)(ctModuleArguments);

#endif // ENGINE_MODULESPEC_H
