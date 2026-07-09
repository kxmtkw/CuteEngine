
#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include "CuteConfig.h"
#include "engine/error.h"
#include <stdint.h>

struct _ctObjectManager;

typedef void (*ctObjectDelete)(struct _ctObjectManager*, struct _ctObject*);

struct _ctObject {
	uint32_t            bucket_id;
	uint32_t            bucket_index;
	uint32_t            ref_count;
	uint32_t            data_size;
	uint64_t            data_type;
	ctObjectDelete      data_del_func;
	void*               data;
}; 
typedef struct _ctObject ctObject;


typedef struct {
	uint32_t id;
	uint64_t bitmask;
	ctObject objects[64];
} ctObjectBucket;


struct _ctObjectManager{
	ctObjectBucket**    buckets;
	uint32_t            bucket_count;
	uint32_t            bucket_capacity;
	ctObjectBucket**    empty_buckets;
	uint32_t            empty_bucket_count;
	uint32_t            empty_bucket_capacity;
	ctError             error;
};
typedef struct _ctObjectManager ctObjectManager;


// Start up the Object manager
void
ct_objects_init(ctObjectManager* manager);

// End the Object manager
void
ct_objects_end(ctObjectManager* manager);

// Allocate a new bucket, return its id
uint32_t
ct_objects_newBucket(ctObjectManager* manager);

// Mark a bucket as empty and push it to the empty buckets stack
void
ct_objects_pushEmptyBucket(ctObjectManager* manager, ctObjectBucket* bucket);

// Mark a bucket as not empty and remove it from the empty buckets stack
ctObjectBucket*
ct_objects_popEmptyBucket(ctObjectManager* manager);

// Allocate a new Object
ctObject*
ct_objects_newObject(ctObjectManager* manager, uint32_t data_size, uint64_t data_type, ctObjectDelete del_func);

// Delete a Object, keeping in mind sub Objects
void
ct_objects_delObject(ctObjectManager* manager, ctObject* con);

static inline void
ct_objects_incRef(ctObjectManager* manager, ctObject* con) {
	con->ref_count++;
	CUTE_LOG("objects", "Object (%u.%u) [%p] referenced. References: %u\n", con->bucket_id, con->bucket_index, con, con->ref_count);
}

static inline void
ct_objects_decRef(ctObjectManager* manager, ctObject* con) {
	con->ref_count--;
	CUTE_LOG("objects", "Object (%u.%u) [%p] dereferenced. References: %u\n", con->bucket_id, con->bucket_index, con, con->ref_count);
	if (con->ref_count == 0) {
		ct_objects_delObject(manager, con);
	}
}


#endif // OBJECT_OBJECT_H