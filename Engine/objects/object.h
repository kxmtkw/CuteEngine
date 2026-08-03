#ifndef CUTE_OBJECT_H
#define CUTE_OBJECT_H

#include <stdint.h>

#include "common/atom.h"
#include "common/config.h"
#include "common/error.h"


struct CtObjectManager;
typedef struct CtObjectManager CtObjectManager;

struct CtObjectBucket;
typedef struct CtObjectBucket CtObjectBucket;

typedef void (*CtObjectDelete)(struct CtObjectManager*, struct CtObject*);

struct CtObject {
	uint32_t            bucket_id;
	uint32_t            bucket_index;
	uint32_t            ref_count;
	uint32_t            obj_size;
	uint32_t            obj_type;
	CtObjectDelete      obj_del_func;
	CtObjectBucket*     bucket;
}; 

#endif // CUTE_OBJECT_H