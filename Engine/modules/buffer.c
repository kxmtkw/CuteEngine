
#include <stdbool.h>
#include <sys/types.h>

#include "CuteAtom.h"
#include "containers/container.h"
#include "engine/error.h"
#include "modules/modules.h"
#include "modulespec.h"
#include "utils/utils.h"
#include "buffer.h"


#ifdef CUTE_BUILTIN_MODULE_BUFFER

static inline uint32_t
_ct_mbuffer_getBufferCapacity(ctContainer* con) {
	return con->size * 8;
}

ctBuffer
ct_mbuffer_makeBuffer(ctContainer* con) {
	return (ctBuffer) {(uint8_t*) con->atoms, con->size * 8};;
};


ctModuleResult 
ct_mbuffer_newBuffer(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(1, args.count, &result)) {
		return result;
	};

	ctAtom capacity = args.atoms[0];

	uint32_t bytes_as_atoms = (capacity.as_uint + 7) / 8;
	ctContainer* buffer = ct_containers_newContainer(args.container_manager, bytes_as_atoms);

	result.returned_atom.as_container = buffer;
	result.returned_atom_type = ctAtomType_Container;
	result.success = true;

	return result;
};


ctModuleResult 
ct_mbuffer_getSize(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(1, args.count, &result)) {
		return result;
	};

	ctAtom buffer = args.atoms[0];

	if (!ct_modules_utils_isContainer(args.types[0], &result)) {
		return result;
	};

	result.returned_atom.as_uint = _ct_mbuffer_getBufferCapacity(buffer.as_container);
	result.returned_atom_type = ctAtomType_UInt;
	result.success = true;

	return result;
};


ctModuleResult 
ct_mbuffer_resize(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(2, args.count, &result)) {
		return result;
	};

	ctAtom buffer = args.atoms[0];
	ctAtom new_capacity = args.atoms[1];

	if (!ct_modules_utils_isContainer(args.types[0], &result)) {
		return result;
	};

	uint32_t bytes_as_atoms = (new_capacity.as_uint + 7) / 8;
	ct_containers_conResize(args.container_manager, buffer.as_container, bytes_as_atoms);

	result.returned_atom_type = ctAtomType_NoneType;
	result.success = true;

	return result;
}


ctModuleResult 
ct_mbuffer_getByte(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(2, args.count, &result)) {
		return result;
	};

	ctAtom buffer_atom = args.atoms[0];
	ctAtom index = args.atoms[1];

	if (!ct_modules_utils_isContainer(args.types[0], &result)) {
		return result;
	};

	ctBuffer buffer = ct_mbuffer_makeBuffer(buffer_atom.as_container);

	if (index.as_uint >= buffer.size) {
		result.returned_atom_type = ctAtomType_NoneType;
		result.returned_atom.raw = 0;
		result.success = false;
		result.error.code = ctErrorCode_IndexError;
		ct_utils_format(
			result.error.msg,
			sizeof(result.error.msg),
			"Accessed index %u (>=%u)", index.as_uint, buffer.size
		);
		return result;
	};

	
	result.returned_atom.as_uint = buffer.bytes[index.as_uint];
	result.returned_atom_type = ctAtomType_UInt;
	result.success = true;

	return result;
};


ctModuleResult 
ct_mbuffer_setByte(ctModuleArguments args) {

    ctModuleResult result;

    if (!ct_modules_utils_areArgsEnough(3, args.count, &result)) {
        return result;
    };

    ctAtom buffer_atom = args.atoms[0];
    ctAtom index = args.atoms[1];
    ctAtom byte_val = args.atoms[2];

	if (!ct_modules_utils_isContainer(args.types[0], &result)) {
		return result;
	};

	ctBuffer buffer = ct_mbuffer_makeBuffer(buffer_atom.as_container);

	if (index.as_uint >= buffer.size) {
        result.returned_atom_type = ctAtomType_NoneType;
        result.returned_atom.raw = 0;
        result.success = false;
        result.error.code = ctErrorCode_IndexError;
        ct_utils_format(
            result.error.msg,
            sizeof(result.error.msg),
            "Accessed index %u (>=%u)", index.as_uint, buffer.size
        );
        return result;
    };

    buffer.bytes[index.as_uint] = (uint8_t) byte_val.as_uint;

    result.returned_atom_type = ctAtomType_NoneType;
    result.returned_atom.raw = 0;
    result.success = true;

    return result;
};


ctModuleResult 
ct_mbuffer_setBytes(ctModuleArguments args) {

    ctModuleResult result;

    if (!ct_modules_utils_areArgsEnough(4, args.count, &result)) {
        return result;
    };

    ctAtom buffer_atom = args.atoms[0];
    ctAtom index = args.atoms[1];
    ctAtom count = args.atoms[2];
    ctAtom byte_val = args.atoms[3];

	if (!ct_modules_utils_isContainer(args.types[0], &result)) {
		return result;
	};

	ctBuffer buffer = ct_mbuffer_makeBuffer(buffer_atom.as_container);

    if (index.as_uint >= buffer.size || (index.as_uint + count.as_uint) > buffer.size) {
        result.returned_atom_type = ctAtomType_NoneType;
        result.returned_atom.raw = 0;
        result.success = false;
        result.error.code = ctErrorCode_IndexError;
        ct_utils_format(
            result.error.msg,
            sizeof(result.error.msg),
            "Accessed range %u to %u (>=%u)", 
            index.as_uint, (index.as_uint + count.as_uint), buffer.size
        );
        return result;
    };

    
    for (uint32_t i = 0; i < count.as_uint; i++) {
        buffer.bytes[index.as_uint + i] = (uint8_t) byte_val.as_uint;
    }

    result.returned_atom_type = ctAtomType_NoneType;
    result.returned_atom.raw = 0;
    result.success = true;

    return result;
};

#endif // CUTE_BUILTIN_MODULE_BUFFER