
#ifndef ENGINE_MODULES_BUFFER_H
#define ENGINE_MODULES_BUFFER_H

#include "CuteModules.h"
#include "modulespec.h"

#ifdef CUTE_BUILTIN_MODULE_BUFFER


/*
Allocate a new buffer.

Arg (1) uint : Size of the buffer in bytes

Returns: container
*/
ctModuleResult 
ct_mbuffer_newBuffer(ctModuleArguments args);


/*
Get the buffer's size in bytes

Arg (1) container : Buffer object

Returns: uint
*/
ctModuleResult 
ct_mbuffer_getCapacity(ctModuleArguments args);



/*
Get a specific byte from the buffer

Arg (1) container : Buffer object

Arg (2) uint : Index

Returns: uint
*/
ctModuleResult 
ct_mbuffer_getByte(ctModuleArguments args);


/*
Set a specific byte in the buffer

Arg (1) container : Buffer object

Arg (2) uint : Index

Arg (3) uint : Byte, The uint is converted into a single byte. Truncation can occur and is on the user.
*/
ctModuleResult 
ct_mbuffer_setByte(ctModuleArguments args);

/*
Set a N byte in the buffer. Will raise error if Index + Count > Capacity

Arg (1) container : Buffer object

Arg (2) uint : Index

Arg (3) uint : Count N

Arg (4) uint : Byte, The uint is converted into a single byte. Truncation can occur and is on the user.
*/
ctModuleResult 
ct_mbuffer_setBytes(ctModuleArguments args);



static const ctModuleMethod ct_mbuffer_dispatchMap[] = {
	ct_mbuffer_newBuffer,
	ct_mbuffer_getCapacity,
	ct_mbuffer_getByte,
	ct_mbuffer_setByte,
	ct_mbuffer_setBytes
};

#define MODULE_BUFFER_METHODS ct_mbuffer_dispatchMap
#define MODULE_BUFFER_METHODS_COUNT (sizeof(ct_mbuffer_dispatchMap) / sizeof(ct_mbuffer_dispatchMap[0]))

#else

#define MODULE_BUFFER_METHODS NULL
#define MODULE_BUFFER_METHODS_COUNT 0

#endif // CUTE_BUILTIN_MODULE_BUFFER

#endif // ENGINE_MODULES_BUFFER_H
