
#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include "CuteConfig.h"
#include "error/error.h"
#include <stdint.h>

struct _ctObjectManager;
typedef struct _ctObjectManager ctObjectManager;

typedef void (*ctObjectDelete)(struct _ctObjectManager*, struct _ctObject*);

struct _ctObject {
	uint32_t            bucket_id;
	uint32_t            bucket_index;
	uint32_t            ref_count;
	uint32_t            obj_size;
	uint64_t            obj_type;
	ctObjectDelete      obj_del_func;
}; 
typedef struct _ctObject ctObject;


typedef struct {
	uint32_t  id;
	uint64_t  bitmask;
	ctObject* objects[64];
} ctObjectBucket;


// Start up the Object manager 
ctObjectManager*
ct_objects_init();

// End the Object manager, Also frees the object manager
void
ct_objects_end(ctObjectManager** manager_ptr);


// Check if the object manager failed.
bool
ct_objects_checkError(ctObjectManager* manager_ptr, ctError* error);


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
ct_objects_newObject(ctObjectManager* manager, uint32_t obj_size, uint64_t obj_type, ctObjectDelete del_func);

// Delete a Object, keeping in mind sub Objects
void
ct_objects_delObject(ctObjectManager* manager, ctObject* obj);

static inline void
ct_objects_incRef(ctObjectManager* manager, ctObject* obj) {
	obj->ref_count++;
	CUTE_LOG("objects", "Object (%u.%u) [%p] referenced. References: %u\n", obj->bucket_id, obj->bucket_index, obj, obj->ref_count);
}

// Decrease the ref count of an object. Returns true if the object is deleted.
static inline bool
ct_objects_decRef(ctObjectManager* manager, ctObject* obj) {
	obj->ref_count--;
	CUTE_LOG("objects", "Object (%u.%u) [%p] dereferenced. References: %u\n", obj->bucket_id, obj->bucket_index, obj, obj->ref_count);
	if (obj->ref_count == 0) {
		ct_objects_delObject(manager, obj);
	}
}


#endif // OBJECT_OBJECT_H