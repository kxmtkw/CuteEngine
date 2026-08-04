
#ifndef CT_ERROR_H
#define CT_ERROR_H

#include "utils/utils.h"
#include <stdint.h>
#include <stdio.h>

typedef enum {
	CT_ERROR_LEVEL_ENGINE = 0x00,
	CT_ERROR_LEVEL_LIB = 0x01,
	CT_ERROR_LEVEL_USER = 0x02,
} CtErrorLevel;

typedef struct {
	bool          raised;
	CtErrorLevel  level;
	char*         where;
	char*         what;
	char          details[256];
} CtError;


#define CT_ERROR(ERROR, LEVEL, WHERE, WHAT, DETAILS, ...) \
ERROR.where = WHERE; ERROR.what = WHAT; \
ct_utils_format(ERROR.details, sizeof(ERROR.details), DETAILS, __VA_ARGS__); \
ERROR.raised = true; \
ERROR.level = LEVEL;

#define CT_ERROR_ENGINE(ERROR, WHERE, WHAT, DETAILS, ...) \
CT_ERROR(ERROR, CT_ERROR_LEVEL_ENGINE, WHERE, WHAT, DETAILS, __VA_ARGS__)

static inline void
ct_error_print(CtError* err) {
	printf("%s.%s :: %s\n", err->where, err->what, err->details);
}


#endif // CT_ERROR_H