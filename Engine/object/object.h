
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


// Startup the object manager. Pushes the first object bucket as well.
ctObjectManager*
ct_objects_init();

// End the Object manager and all its resources. Sets the pointer to NULL for convenience.
void
ct_objects_end(ctObjectManager** manager_ptr);

// Check if the object manager failed.
bool
ct_objects_checkError(ctObjectManager* manager_ptr, ctError* error);

// Allocate a new empty bucket and pushes it to the stack. Returns it's id.
uint32_t
ct_objects_newBucket(ctObjectManager* manager);

// Push a bucket to the empty stack. THe bucket SHOULD be empty (not filled) before it is pushed.
void
ct_objects_pushEmptyBucket(ctObjectManager* manager, ctObjectBucket* bucket);

// Pop an empty bucket from the stack. If the bucket is full after the allocation, do not push it back.
ctObjectBucket*
ct_objects_popEmptyBucket(ctObjectManager* manager);

// Allocate a new object.
ctObject*
ct_objects_newObject(ctObjectManager* manager, uint32_t obj_size, uint64_t obj_type, ctObjectDelete del_func);

// Delete an object, sub objects are NOT considered. The deleter if it exists, is also called.
void
ct_objects_delObject(ctObjectManager* manager, ctObject* obj);

// Increase the refcount of the object.
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
		return true;
	}
	return false;
}


#endif // OBJECT_OBJECT_H