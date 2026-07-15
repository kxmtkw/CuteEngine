
#ifndef ENGINE_ERROR_H
#define ENGINE_ERROR_H

#include <stdint.h>
#include <stdio.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"

typedef enum {

    ctErrorCode_None            = 0x00,
    ctErrorCode_Engine          = 0x01,
    ctErrorCode_Procedure       = 0x02,
    ctErrorCode_Module          = 0x03,
    ctErrorCode_Memory          = 0x04,
    ctErrorCode_Type            = 0x05,
    ctErrorCode_Value           = 0x06,
    ctErrorCode_ZeroDivision    = 0x07,
    ctErrorCode_Overflow        = 0x08,
    ctErrorCode_Underflow       = 0x09,
    ctErrorCode_Index           = 0x0a,
    ctErrorCode_Recursion       = 0x0b,
    ctErrorCode_AssertionFailed = 0x0c,
    ctErrorCode_File            = 0x0d,
	ctErrorCode_OutOfBounds     = 0x0f,

} ctErrorCode;

static const char* ct_error_stringforms[] = {

    [ctErrorCode_None]            = "NoError",
    [ctErrorCode_Engine]          = "EngineError",
    [ctErrorCode_Procedure]       = "ProcedureError",
    [ctErrorCode_Module]          = "ModuleError",
    [ctErrorCode_Memory]          = "MemoryError",
    [ctErrorCode_Type]            = "TypeError",
    [ctErrorCode_Value]           = "ValueError",
    [ctErrorCode_ZeroDivision]    = "ZeroDivisionError",
    [ctErrorCode_Overflow]        = "OverflowError",
    [ctErrorCode_Underflow]       = "UnderflowError",
    [ctErrorCode_Index]           = "IndexError",
    [ctErrorCode_Recursion]       = "RecursionError",
    [ctErrorCode_AssertionFailed] = "AssertionFailedError",
    [ctErrorCode_File]            = "FileError"

};


typedef struct {
	ctErrorCode code;
	char        msg[256];
} ctError;


static inline void
ct_error_print(ctError err) {
	printf("[%s] %s\n", ct_error_stringforms[err.code], err.msg);
}


#define CUTE_ERROR(ERR, CODE, MSG, ...) \
ERR->code = ctErrorCode_Memory; \
ct_utils_format( \
	ERR->msg,  \
	sizeof(ERR->msg), \
	MSG, __VA_ARGS__ \
); \

#endif // ENGINE_ERROR_H