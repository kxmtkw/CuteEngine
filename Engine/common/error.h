
#ifndef ENGINE_ERROR_H
#define ENGINE_ERROR_H

#include <stdint.h>
#include <stdio.h>


typedef enum {

    ctErrorCode_None            = 0x00,
    ctErrorCode_Engine          = 0x01,
    ctErrorCode_Procedure       = 0x02,
    ctErrorCode_Module          = 0x03,
	ctErrorCode_OutOfBounds     = 0x04,

    ctErrorCode_Type            = 0x10,
    ctErrorCode_Value           = 0x11,
    ctErrorCode_ZeroDivision    = 0x12,
    ctErrorCode_Overflow        = 0x13,
    ctErrorCode_Underflow       = 0x14,
    ctErrorCode_Index           = 0x15,
    ctErrorCode_Recursion       = 0x16,

} ctErrorCode;

static const char* ct_error_stringforms[] = {

    [ctErrorCode_None]            = "NoError",
    [ctErrorCode_Engine]          = "EngineError",
    [ctErrorCode_Procedure]       = "ProcedureError",
    [ctErrorCode_Module]          = "ModuleError",
	[ctErrorCode_OutOfBounds]     = "OutOfBounds",

    [ctErrorCode_Type]            = "TypeError",
    [ctErrorCode_Value]           = "ValueError",
    [ctErrorCode_ZeroDivision]    = "ZeroDivisionError",
    [ctErrorCode_Overflow]        = "OverflowError",
    [ctErrorCode_Underflow]       = "UnderflowError",
    [ctErrorCode_Index]           = "IndexError",
    [ctErrorCode_Recursion]       = "RecursionError"
};


typedef struct {
	ctErrorCode code;
	char        msg[256];
} ctError;


static inline void
ct_error_print(ctError err) {
	printf("[%s] %s\n", ct_error_stringforms[err.code], err.msg);
}

#define CT_ERROR(ERR, CODE, MSG, ...) \
ERR->code = CODE; \
ct_utils_format( \
	ERR->msg,  \
	sizeof(ERR->msg), \
	MSG, __VA_ARGS__ \
); \


#endif // ENGINE_ERROR_H