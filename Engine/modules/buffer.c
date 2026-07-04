
#include <stdbool.h>
#include <sys/types.h>

#include "CuteAtom.h"
#include "containers/container.h"
#include "engine/error.h"
#include "modules/modules.h"
#include "modulespec.h"
#include "utils/utils.h"
#include "buffer.h"



static inline uint32_t
_ct_mbuffer_getBufferCapacity(uint32_t con_size) {
	return con_size * 8;
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
ct_mbuffer_getCapacity(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(1, args.count, &result)) {
		return result;
	};

	ctAtom buffer = args.atoms[0];

	result.returned_atom.as_uint = _ct_mbuffer_getBufferCapacity(buffer.as_container->size);
	result.returned_atom_type = ctAtomType_UInt;
	result.success = true;

	return result;
};


ctModuleResult 
ct_mbuffer_getByte(ctModuleArguments args) {

	ctModuleResult result;

	if (!ct_modules_utils_areArgsEnough(2, args.count, &result)) {
		return result;
	};

	ctAtom buffer = args.atoms[0];
	ctAtom index = args.atoms[1];

	if (index.as_uint >= _ct_mbuffer_getBufferCapacity(buffer.as_container->size)) {
		result.returned_atom_type = ctAtomType_NoneType;
		result.returned_atom.raw = 0;
		result.success = false;
		result.error.code = ctErrorCode_IndexError;
		ct_utils_format(
			result.error.msg,
			sizeof(result.error.msg),
			"Accessed index %u (>=%u)", index.as_uint, _ct_mbuffer_getBufferCapacity(buffer.as_container->size)
		);
		return result;
	};

	uint8_t* raw_bytes = (uint8_t*) buffer.as_container->atoms;
	
	result.returned_atom.as_uint = raw_bytes[index.as_uint];
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

    ctAtom buffer = args.atoms[0];
    ctAtom index = args.atoms[1];
    ctAtom byte_val = args.atoms[2];

    uint32_t buffer_cap = _ct_mbuffer_getBufferCapacity(buffer.as_container->size);

	if (index.as_uint >= buffer_cap) {
        result.returned_atom_type = ctAtomType_NoneType;
        result.returned_atom.raw = 0;
        result.success = false;
        result.error.code = ctErrorCode_IndexError;
        ct_utils_format(
            result.error.msg,
            sizeof(result.error.msg),
            "Accessed index %u (>=%u)", index.as_uint, buffer_cap
        );
        return result;
    };

    uint8_t* raw_bytes = (uint8_t*) buffer.as_container->atoms;
    raw_bytes[index.as_uint] = (uint8_t) byte_val.as_uint;

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

    ctAtom buffer = args.atoms[0];
    ctAtom index = args.atoms[1];
    ctAtom count = args.atoms[2];
    ctAtom byte_val = args.atoms[3];

    uint32_t buffer_cap = _ct_mbuffer_getBufferCapacity(buffer.as_container->size);

    if (index.as_uint >= buffer_cap || (index.as_uint + count.as_uint) > buffer_cap) {
        result.returned_atom_type = ctAtomType_NoneType;
        result.returned_atom.raw = 0;
        result.success = false;
        result.error.code = ctErrorCode_IndexError;
        ct_utils_format(
            result.error.msg,
            sizeof(result.error.msg),
            "Accessed range %u to %u (>=%u)", 
            index.as_uint, (index.as_uint + count.as_uint), buffer_cap
        );
        return result;
    };

    uint8_t* raw_bytes = (uint8_t*) buffer.as_container->atoms;
    
    for (uint32_t i = 0; i < count.as_uint; i++) {
        raw_bytes[index.as_uint + i] = (uint8_t) byte_val.as_uint;
    }

    result.returned_atom_type = ctAtomType_NoneType;
    result.returned_atom.raw = 0;
    result.success = true;

    return result;
};