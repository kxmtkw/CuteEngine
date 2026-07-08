

#ifdef CUTE_BUILTIN_MODULE_IO

#include <stdbool.h>
#include <sys/types.h>

#include "CuteAtom.h"
#include "containers/container.h"
#include "engine/error.h"
#include "modules/buffer.h"
#include "modules/modules.h"
#include "modulespec.h"
#include "utils/utils.h"
#include "io.h"


ctModuleResult 
ct_mio_print(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(1, args.count, &result)) {
		return result;
	};

	ctAtom atom = args.atoms[0];
	ctAtomTypeSize type = args.types[0];


	if (!ct_modules_utils_isContainer(type, &result)) {
		return result;
	};

	ctBuffer buffer = ct_mbuffer_makeBuffer(atom.as_container);

	printf("%.*s", buffer.size, buffer.bytes);

	result.returned_atom_type = ctAtomType_NoneType;
	result.success = true;

	return result;
};

#endif // CUTE_BUILTIN_MODULE_IO